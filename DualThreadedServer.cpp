/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <stdexcept>
#include <stdint.h>
#include <thrift/concurrency/ThreadManager.h>

#include "DualThreadedServer.h"
#include "NettyFramedTransport.h"

namespace apache {
namespace thrift {
namespace server {

using apache::thrift::concurrency::Synchronized;
using apache::thrift::concurrency::Thread;
using apache::thrift::concurrency::ThreadFactory;
using apache::thrift::transport::TServerTransport;
using apache::thrift::transport::TTransport;
using apache::thrift::transport::TTransportException;
using apache::thrift::transport::TTransportFactory;
using apache::thrift::transport::NettyFramedTransport;
using apache::thrift::protocol::TProtocol;
using apache::thrift::protocol::TProtocolFactory;
using apache::thrift::protocol::NettyProtocol;
using boost::bind;
using boost::shared_ptr;
using boost::make_shared;
using std::string;

DualThreadedServer::DualThreadedServer(const shared_ptr<TProcessorFactory>& processorFactory,
                                   const shared_ptr<TServerTransport>& serverTransport,
                                   const shared_ptr<TTransportFactory>& transportFactory,
                                   const shared_ptr<TProtocolFactory>& protocolFactory,
                                   const shared_ptr<ThreadFactory>& threadFactory)
  : TServer(processorFactory, serverTransport, transportFactory, protocolFactory),
    threadFactory_(threadFactory),
    clients_(0),
    hwm_(0),
    limit_(INT64_MAX) {
}

DualThreadedServer::DualThreadedServer(const shared_ptr<TProcessor>& processor,
                                   const shared_ptr<TServerTransport>& serverTransport,
                                   const shared_ptr<TTransportFactory>& transportFactory,
                                   const shared_ptr<TProtocolFactory>& protocolFactory,
                                   const shared_ptr<NettyProcessor>& nettyProcessor,
                                   const shared_ptr<TServerTransport>& nettyServerTransport,
                                   const shared_ptr<ThreadFactory>& threadFactory)
  : TServer(processor, serverTransport, transportFactory, protocolFactory),
    nettyProcessor_(nettyProcessor),
    nettyServerTransport_(nettyServerTransport),
    threadFactory_(threadFactory),
    clients_(0),
    hwm_(0),
    limit_(INT64_MAX) {
}

DualThreadedServer::DualThreadedServer(const shared_ptr<TProcessorFactory>& processorFactory,
                                   const shared_ptr<TServerTransport>& serverTransport,
                                   const shared_ptr<TTransportFactory>& inputTransportFactory,
                                   const shared_ptr<TTransportFactory>& outputTransportFactory,
                                   const shared_ptr<TProtocolFactory>& inputProtocolFactory,
                                   const shared_ptr<TProtocolFactory>& outputProtocolFactory,
                                   const shared_ptr<ThreadFactory>& threadFactory)
  : TServer(processorFactory,
            serverTransport,
            inputTransportFactory,
            outputTransportFactory,
            inputProtocolFactory,
            outputProtocolFactory),
    threadFactory_(threadFactory),
    clients_(0),
    hwm_(0),
    limit_(INT64_MAX) {
}

DualThreadedServer::DualThreadedServer(const shared_ptr<TProcessor>& processor,
                                   const shared_ptr<TServerTransport>& serverTransport,
                                   const shared_ptr<TTransportFactory>& inputTransportFactory,
                                   const shared_ptr<TTransportFactory>& outputTransportFactory,
                                   const shared_ptr<TProtocolFactory>& inputProtocolFactory,
                                   const shared_ptr<TProtocolFactory>& outputProtocolFactory,
                                   const shared_ptr<ThreadFactory>& threadFactory)
  : TServer(processor,
            serverTransport,
            inputTransportFactory,
            outputTransportFactory,
            inputProtocolFactory,
            outputProtocolFactory),
    threadFactory_(threadFactory),
    clients_(0),
    hwm_(0),
    limit_(INT64_MAX) {
}

DualThreadedServer::~DualThreadedServer() {
}

template <typename T>
static void releaseOneDescriptor(const string& name, T& pTransport) {
  if (pTransport) {
    try {
      pTransport->close();
    } catch (const TTransportException& ttx) {
      string errStr = string("TServerFramework " + name + " close failed: ") + ttx.what();
      GlobalOutput(errStr.c_str());
    }
  }
}

void DualThreadedServer::serve() {
  shared_ptr<TTransport> client;
  shared_ptr<TTransport> inputTransport;
  shared_ptr<TTransport> outputTransport;
  shared_ptr<TProtocol> inputProtocol;
  shared_ptr<TProtocol> outputProtocol;

  shared_ptr<NettyFramedTransport> inputNettyTransport;
  shared_ptr<NettyProtocol> inputNettyProtocol;
  shared_ptr<TTransport> nettyClient;

  // Start the RPC server listening
  serverTransport_->listen();

  // Start the netty server listening
  nettyServerTransport_->listen();

  // Run the preServe event to indicate server is now listening
  // and that it is safe to connect.
  if (eventHandler_) {
    eventHandler_->preServe();
  }

  // Fetch client from server
  for (;;)
    {
    try {
      // Dereference any resources from any previous client creation
      // such that a blocking accept does not hold them indefinitely.
      outputProtocol.reset();
      inputProtocol.reset();
      outputTransport.reset();
      inputTransport.reset();
      client.reset();

      // If we have reached the limit on the number of concurrent
      // clients allowed, wait for one or more clients to drain before
      // accepting another.
      {
        Synchronized sync(mon_);
        while (clients_ >= limit_) {
          mon_.wait();
        }
      }
      client = serverTransport_->accept();

      inputTransport = inputTransportFactory_->getTransport(client);
      outputTransport = outputTransportFactory_->getTransport(client);
      if (!outputProtocolFactory_) {
        inputProtocol = inputProtocolFactory_->getProtocol(inputTransport, outputTransport);
        outputProtocol = inputProtocol;
      } else {
        inputProtocol = inputProtocolFactory_->getProtocol(inputTransport);
        outputProtocol = outputProtocolFactory_->getProtocol(outputTransport);
      }

      newlyConnectedClient(shared_ptr<TConnectedClient>(
          new TConnectedClient(getProcessor(inputProtocol, outputProtocol, client),
                               inputProtocol,
                               outputProtocol,
                               eventHandler_,
                               client),
          bind(&DualThreadedServer::disposeConnectedClient, this, _1)));

    } catch (TTransportException& ttx) {
      releaseOneDescriptor("inputTransport", inputTransport);
      releaseOneDescriptor("outputTransport", outputTransport);
      releaseOneDescriptor("client", client);
      if (ttx.getType() == TTransportException::TIMED_OUT) {
        // Accept timeout - continue processing.
        // continue;
    	// just fall through the try statement on timeout
      } else if (ttx.getType() == TTransportException::END_OF_FILE
                 || ttx.getType() == TTransportException::INTERRUPTED) {
        // Server was interrupted.  This only happens when stopping.
        break;
      } else
        {
        // All other transport exceptions are logged.
        // State of connection is unknown.  Done.
        string errStr = string("TServerTransport died: ") + ttx.what();
        GlobalOutput.printf(errStr.c_str());
        break;
        }
      }	// try...catch
    // if we got here, either we connected, or timed out
    // now let's try accepting a netty connection
    try
      {
    	// TODO:  reset the netty items
    	// do the resets again
        outputProtocol.reset();
        inputProtocol.reset();
        outputTransport.reset();
        inputTransport.reset();
        client.reset();

        // and again make sure we haven't passed our limit
        {
          Synchronized sync(mon_);
          while (clients_ >= limit_) {
            mon_.wait();
          }
        }
      // now try accepting a netty connection
      nettyClient = nettyServerTransport_->accept();

      // we got one!  spin off a thread to handle it
      printf("Connected on netty port\n");

      inputNettyTransport = shared_ptr<NettyFramedTransport>(new NettyFramedTransport(nettyClient));
      inputNettyProtocol = shared_ptr<NettyProtocol>(new NettyProtocol(inputNettyTransport));

      newlyNettyConnectedClient(shared_ptr<NettyConnectedClient>(
          new NettyConnectedClient(nettyProcessor_,
                               inputNettyProtocol,
                               inputNettyProtocol,
                               nettyClient),
          bind(&DualThreadedServer::nettyDisposeConnectedClient, this, _1)));

      }
    catch (TTransportException& ttx)
      {
      if (ttx.getType() == TTransportException::TIMED_OUT)
        continue;	// loop back and wait for connection on either port
      else if (ttx.getType() == TTransportException::END_OF_FILE
                 || ttx.getType() == TTransportException::INTERRUPTED) {
        // Server was interrupted.  This only happens when stopping.
        break;
      } else
        {
        // All other transport exceptions are logged.
        // State of connection is unknown.  Done.
        string errStr = string("TServerTransport died: ") + ttx.what();
        GlobalOutput.printf(errStr.c_str());
        break;
        }
      }	// try...catch
    }	// for

  releaseOneDescriptor("serverTransport", serverTransport_);
  releaseOneDescriptor("nettyServerTransport", nettyServerTransport_);

  // Ensure post-condition of no active clients
  Synchronized s(clientMonitor_);
  while (!activeClientMap_.empty()) {
    clientMonitor_.wait();
  }

  drainDeadClients();
}	// serve()

void DualThreadedServer::drainDeadClients() {
  // we're in a monitor here
	  while (!deadClientMap_.empty()) {
	    ClientMap::iterator it = deadClientMap_.begin();
	    it->second->join();
	    deadClientMap_.erase(it);
	  }
	  while (!nettyDeadClientMap_.empty()) {
	    NettyClientMap::iterator it = nettyDeadClientMap_.begin();
	    it->second->join();
	    nettyDeadClientMap_.erase(it);
	  }
}

void DualThreadedServer::onClientConnected(const shared_ptr<TConnectedClient>& pClient) {
  Synchronized sync(clientMonitor_);
  boost::shared_ptr<TConnectedClientRunner> pRunnable = boost::make_shared<TConnectedClientRunner>(pClient);
  boost::shared_ptr<Thread> pThread = threadFactory_->newThread(pRunnable);
  pRunnable->thread(pThread);
  activeClientMap_.insert(ClientMap::value_type(pClient.get(), pThread));
  pThread->start();
}

void DualThreadedServer::onNettyClientConnected(const shared_ptr<NettyConnectedClient>& pClient) {
  Synchronized sync(clientMonitor_);
  boost::shared_ptr<NettyConnectedClientRunner> pRunnable = boost::make_shared<NettyConnectedClientRunner>(pClient);
  boost::shared_ptr<Thread> pThread = threadFactory_->newThread(pRunnable);
  pRunnable->thread(pThread);
  nettyActiveClientMap_.insert(NettyClientMap::value_type(pClient.get(), pThread));
  pThread->start();
}

void DualThreadedServer::onClientDisconnected(TConnectedClient* pClient) {
  Synchronized sync(clientMonitor_);
  drainDeadClients(); // use the outgoing thread to do some maintenance on our dead client backlog
  ClientMap::iterator it = activeClientMap_.find(pClient);
  ClientMap::iterator end = it;
  deadClientMap_.insert(it, ++end);
  activeClientMap_.erase(it);
  if (activeClientMap_.empty()) {
    clientMonitor_.notify();
  }
}

void DualThreadedServer::onNettyClientDisconnected(NettyConnectedClient* pClient) {
  Synchronized sync(clientMonitor_);
  drainDeadClients(); // use the outgoing thread to do some maintenance on our dead client backlog
  NettyClientMap::iterator it = nettyActiveClientMap_.find(pClient);
  NettyClientMap::iterator end = it;
  nettyDeadClientMap_.insert(it, ++end);
  nettyActiveClientMap_.erase(it);
  if (nettyActiveClientMap_.empty()) {
    clientMonitor_.notify();
  }
}

DualThreadedServer::TConnectedClientRunner::TConnectedClientRunner(const boost::shared_ptr<TConnectedClient>& pClient)
  : pClient_(pClient) {
}

DualThreadedServer::NettyConnectedClientRunner::NettyConnectedClientRunner(const boost::shared_ptr<NettyConnectedClient>& pClient)
  : pClient_(pClient) {
}

DualThreadedServer::TConnectedClientRunner::~TConnectedClientRunner() {
}

DualThreadedServer::NettyConnectedClientRunner::~NettyConnectedClientRunner() {
}

void DualThreadedServer::TConnectedClientRunner::run() /* override */ {
  pClient_->run();  // Run the client
  pClient_.reset(); // The client is done - release it here rather than in the destructor for safety
}

void DualThreadedServer::NettyConnectedClientRunner::run() /* override */ {
  pClient_->run();  // Run the client
  pClient_.reset(); // The client is done - release it here rather than in the destructor for safety
}

int64_t DualThreadedServer::getConcurrentClientLimit() const {
  Synchronized sync(mon_);
  return limit_;
}

int64_t DualThreadedServer::getConcurrentClientCount() const {
  Synchronized sync(mon_);
  return clients_;
}

int64_t DualThreadedServer::getConcurrentClientCountHWM() const {
  Synchronized sync(mon_);
  return hwm_;
}

void DualThreadedServer::setConcurrentClientLimit(int64_t newLimit) {
  if (newLimit < 1) {
    throw std::invalid_argument("newLimit must be greater than zero");
  }
  Synchronized sync(mon_);
  limit_ = newLimit;
  if (limit_ - clients_ > 0) {
    mon_.notify();
  }
}

void DualThreadedServer::stop() {
  // Order is important because serve() releases serverTransport_ when it is
  // interrupted, which closes the socket that interruptChildren uses.
  serverTransport_->interruptChildren();
  serverTransport_->interrupt();
}

void DualThreadedServer::newlyConnectedClient(const boost::shared_ptr<TConnectedClient>& pClient) {
  {
    Synchronized sync(mon_);
    ++clients_;
    hwm_ = (std::max)(hwm_, clients_);
  }

  onClientConnected(pClient);
}

void DualThreadedServer::newlyNettyConnectedClient(const boost::shared_ptr<NettyConnectedClient>& pClient) {
  {
    Synchronized sync(mon_);
    ++clients_;
    hwm_ = (std::max)(hwm_, clients_);
  }

  onNettyClientConnected(pClient);
}

void DualThreadedServer::disposeConnectedClient(TConnectedClient* pClient) {
  onClientDisconnected(pClient);
  delete pClient;

  Synchronized sync(mon_);
  if (limit_ - --clients_ > 0) {
    mon_.notify();
  }
}

void DualThreadedServer::nettyDisposeConnectedClient(NettyConnectedClient* pClient) {
  onNettyClientDisconnected(pClient);
  delete pClient;

  Synchronized sync(mon_);
  if (limit_ - --clients_ > 0) {
    mon_.notify();
  }
}

}
}
} // apache::thrift::server
