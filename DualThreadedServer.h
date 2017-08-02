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

#ifndef _THRIFT_SERVER_DUALTHREADEDSERVER_H_
#define _THRIFT_SERVER_DUALTHREADEDSERVER_H_ 1

#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include <thrift/TProcessor.h>
#include <thrift/concurrency/Monitor.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/server/TConnectedClient.h>
#include <thrift/server/TServer.h>
#include <thrift/transport/TServerTransport.h>
#include <thrift/transport/TTransport.h>

#include "NettyProtocol.h"
#include "NettyProcessor.h"
#include "NettyConnectedClient.h"
#include "NettyFramedTransport.h"

namespace apache {
namespace thrift {
namespace server {

/**
 * TServerFramework provides a single consolidated processing loop for
 * servers.  By having a single processing loop, behavior between servers
 * is more predictable and maintenance cost is lowered.  Implementations
 * of TServerFramework must provide a method to deal with a client that
 * connects and one that disconnects.
 *
 * While this functionality could be rolled directly into TServer, and
 * probably should be, it would break the TServer interface contract so
 * to maintain backwards compatibility for third party servers, no TServers
 * were harmed in the making of this class.
 */

/**
 * Manage clients using threads - threads are created one for each client and are
 * released when the client disconnects.  This server is used to make a dynamically
 * scalable server up to the concurrent connection limit.
 */

class DualThreadedServer : public TServer {
public:
	DualThreadedServer(
      const boost::shared_ptr<apache::thrift::TProcessorFactory>& processorFactory,
      const boost::shared_ptr<apache::thrift::transport::TServerTransport>& serverTransport,
      const boost::shared_ptr<apache::thrift::transport::TTransportFactory>& transportFactory,
      const boost::shared_ptr<apache::thrift::protocol::TProtocolFactory>& protocolFactory,
    const boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>& threadFactory
    = boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>(
        new apache::thrift::concurrency::PlatformThreadFactory(false)));

	DualThreadedServer(
      const boost::shared_ptr<apache::thrift::TProcessor>& processor,
      const boost::shared_ptr<apache::thrift::transport::TServerTransport>& serverTransport,
      const boost::shared_ptr<apache::thrift::transport::TTransportFactory>& transportFactory,
      const boost::shared_ptr<apache::thrift::protocol::TProtocolFactory>& protocolFactory,
      const boost::shared_ptr<apache::thrift::NettyProcessor>& nettyProcessor,
      const boost::shared_ptr<apache::thrift::transport::TServerTransport>& nettyServerTransport,
    const boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>& threadFactory
    = boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>(
        new apache::thrift::concurrency::PlatformThreadFactory(false)));

	DualThreadedServer(
      const boost::shared_ptr<apache::thrift::TProcessorFactory>& processorFactory,
      const boost::shared_ptr<apache::thrift::transport::TServerTransport>& serverTransport,
      const boost::shared_ptr<apache::thrift::transport::TTransportFactory>& inputTransportFactory,
      const boost::shared_ptr<apache::thrift::transport::TTransportFactory>& outputTransportFactory,
      const boost::shared_ptr<apache::thrift::protocol::TProtocolFactory>& inputProtocolFactory,
      const boost::shared_ptr<apache::thrift::protocol::TProtocolFactory>& outputProtocolFactory,
    const boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>& threadFactory
    = boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>(
        new apache::thrift::concurrency::PlatformThreadFactory(false)));

	DualThreadedServer(
      const boost::shared_ptr<apache::thrift::TProcessor>& processor,
      const boost::shared_ptr<apache::thrift::transport::TServerTransport>& serverTransport,
      const boost::shared_ptr<apache::thrift::transport::TTransportFactory>& inputTransportFactory,
      const boost::shared_ptr<apache::thrift::transport::TTransportFactory>& outputTransportFactory,
      const boost::shared_ptr<apache::thrift::protocol::TProtocolFactory>& inputProtocolFactory,
      const boost::shared_ptr<apache::thrift::protocol::TProtocolFactory>& outputProtocolFactory,
    const boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>& threadFactory
    = boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>(
        new apache::thrift::concurrency::PlatformThreadFactory(false)));

  virtual ~DualThreadedServer();

  /**
   * Accept clients from the TServerTransport and add them for processing.
   * Call stop() on another thread to interrupt processing
   * and return control to the caller.
   * Post-conditions (return guarantees):
   *   The serverTransport will be closed.
   *   There will be no clients connected.
   */
  virtual void serve();

  /**
   * Interrupt serve() so that it meets post-conditions and returns.
   */
  virtual void stop();

  /**
   * Get the concurrent client limit.
   * \returns the concurrent client limit
   */
  virtual int64_t getConcurrentClientLimit() const;

  /**
   * Get the number of currently connected clients.
   * \returns the number of currently connected clients
   */
  virtual int64_t getConcurrentClientCount() const;

  /**
   * Get the highest number of concurrent clients.
   * \returns the highest number of concurrent clients
   */
  virtual int64_t getConcurrentClientCountHWM() const;

  /**
   * Set the concurrent client limit.  This can be changed while
   * the server is serving however it will not necessarily be
   * enforced until the next client is accepted and added.  If the
   * limit is lowered below the number of connected clients, no
   * action is taken to disconnect the clients.
   * The default value used if this is not called is INT64_MAX.
   * \param[in]  newLimit  the new limit of concurrent clients
   * \throws std::invalid_argument if newLimit is less than 1
   */
  virtual void setConcurrentClientLimit(int64_t newLimit);

protected:
  /**
   * Drain recently connected clients by joining their threads - this is done lazily because
   * we cannot do it inside the thread context that is disconnecting.
   */
  virtual void drainDeadClients();

  /**
   * A client has connected.  The implementation is responsible for managing the
   * lifetime of the client object.  This is called during the serve() thread,
   * therefore a failure to return quickly will result in new client connection
   * delays.
   *
   * \param[in]  pClient  the newly connected client
   */
  virtual void onClientConnected(const boost::shared_ptr<TConnectedClient>& pClient);
  virtual void onNettyClientConnected(const boost::shared_ptr<NettyConnectedClient>& pClient);

  /**
   * A client has disconnected.
   * When called:
   *   The server no longer tracks the client.
   *   The client TTransport has already been closed.
   *   The implementation must not delete the pointer.
   *
   * \param[in]  pClient  the disconnected client
   */
  virtual void onClientDisconnected(TConnectedClient* pClient);
  virtual void onNettyClientDisconnected(NettyConnectedClient* pClient);

  /**
   * The processor for the netty I/O side of this server
   */
  boost::shared_ptr<apache::thrift::NettyProcessor> nettyProcessor_;
  boost::shared_ptr<TServerTransport> nettyServerTransport_;
  boost::shared_ptr<apache::thrift::concurrency::ThreadFactory> threadFactory_;

  /**
   * A helper wrapper used to wrap the client in something we can use to maintain
   * the lifetime of the connected client within a detached thread.  We cannot simply
   * track the threads because a shared_ptr<Thread> hangs on to the Runnable it is
   * passed, and TServerFramework requires the runnable (TConnectedClient) to be
   * destroyed in order to work properly.
   */
  class TConnectedClientRunner : public apache::thrift::concurrency::Runnable
  {
  public:
    TConnectedClientRunner(const boost::shared_ptr<TConnectedClient>& pClient);
    virtual ~TConnectedClientRunner();
    void run() /* override */;
  private:
    boost::shared_ptr<TConnectedClient> pClient_;
  };

  class NettyConnectedClientRunner : public apache::thrift::concurrency::Runnable
  {
  public:
    NettyConnectedClientRunner(const boost::shared_ptr<NettyConnectedClient>& pClient);
    virtual ~NettyConnectedClientRunner();
    void run() /* override */;
  private:
    boost::shared_ptr<NettyConnectedClient> pClient_;
  };

  typedef std::map<TConnectedClient *, boost::shared_ptr<apache::thrift::concurrency::Thread> > ClientMap;
  typedef std::map<NettyConnectedClient *, boost::shared_ptr<apache::thrift::concurrency::Thread> > NettyClientMap;

  /**
   * A map of active clients
   */
  ClientMap activeClientMap_;
  NettyClientMap nettyActiveClientMap_;

  /**
   * A map of clients that have disconnected but their threads have not been joined
   */
  ClientMap deadClientMap_;
  NettyClientMap nettyDeadClientMap_;


private:
  /**
   * Common handling for new connected clients.  Implements concurrent
   * client rate limiting after onClientConnected returns by blocking the
   * serve() thread if the limit has been reached.
   */
  void newlyConnectedClient(const boost::shared_ptr<TConnectedClient>& pClient);
  void newlyNettyConnectedClient(const boost::shared_ptr<NettyConnectedClient>& pClient);

  /**
   * Smart pointer client deletion.
   * Calls onClientDisconnected and then deletes pClient.
   */
  void disposeConnectedClient(TConnectedClient* pClient);
  void nettyDisposeConnectedClient(NettyConnectedClient* pClient);

  /**
   * Monitor for limiting the number of concurrent clients.
   */
  apache::thrift::concurrency::Monitor mon_;
  apache::thrift::concurrency::Monitor clientMonitor_;

  /**
   * The number of concurrent clients.
   */
  int64_t clients_;

  /**
   * The high water mark of concurrent clients.
   */
  int64_t hwm_;

  /**
   * The limit on the number of concurrent clients.
   */
  int64_t limit_;
};
}
}
} // apache::thrift::server

#endif // #ifndef _THRIFT_SERVER_DUALTHREADEDSERVER_H_
