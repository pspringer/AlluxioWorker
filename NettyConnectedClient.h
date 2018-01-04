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
 *
 * Modified by Paul Springer
 * Modifications copyright 2018 by Paul Springer
 * Modifications licensed under the Apache License, Version 2.0 (the "License");
 * you may not use the modifications except in compliance with the License.
 */

#ifndef _THRIFT_SERVER_NettyConnectedClient_H_
#define _THRIFT_SERVER_NettyConnectedClient_H_ 1

#include <boost/shared_ptr.hpp>
#include <thrift/TProcessor.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/server/TServer.h>
#include <thrift/transport/TTransport.h>

#include "NettyProcessor.h"
#include "NettyProtocol.h"

namespace apache {
namespace thrift {
namespace server {

/**
 * This represents a client connected to a TServer.  The
 * processing loop for a client must provide some required
 * functionality common to all implementations so it is
 * encapsulated here.
 */

class NettyConnectedClient : public apache::thrift::concurrency::Runnable {
public:
  /**
   * Constructor.
   *
   * @param[in] processor      the TProcessor
   * @param[in] inputProtocol  the input TProtocol
   * @param[in] outputProtocol the output TProtocol
   * @param[in] eventHandler   the server event handler
   * @param[in] client         the TTransport representing the client
   */
  NettyConnectedClient(
      const boost::shared_ptr<apache::thrift::NettyProcessor>& processor,
      const boost::shared_ptr<apache::thrift::protocol::NettyProtocol>& inputProtocol,
      const boost::shared_ptr<apache::thrift::protocol::NettyProtocol>& outputProtocol,
      const boost::shared_ptr<apache::thrift::transport::TTransport>& client);

  /**
   * Destructor.
   */
  virtual ~NettyConnectedClient();

  /**
   * Drive the client until it is done.
   * The client processing loop is:
   *
   * [optional] call eventHandler->createContext once
   * [optional] call eventHandler->processContext per request
   *            call processor->process per request
   *              handle expected transport exceptions:
   *                END_OF_FILE means the client is gone
   *                INTERRUPTED means the client was interrupted
   *                            by TServerTransport::interruptChildren()
   *              handle unexpected transport exceptions by logging
   *              handle standard exceptions by logging
   *              handle unexpected exceptions by logging
   *            cleanup()
   */
  virtual void run() /* override */;

protected:
  /**
   * Cleanup after a client.  This happens if the client disconnects,
   * or if the server is stopped, or if an exception occurs.
   *
   * The cleanup processing is:
   * [optional] call eventHandler->deleteContext once
   *            close the inputProtocol's TTransport
   *            close the outputProtocol's TTransport
   *            close the client
   */
  virtual void cleanup();

private:
  boost::shared_ptr<apache::thrift::NettyProcessor> processor_;
  boost::shared_ptr<apache::thrift::protocol::NettyProtocol> inputProtocol_;
  boost::shared_ptr<apache::thrift::protocol::NettyProtocol> outputProtocol_;
  boost::shared_ptr<apache::thrift::transport::TTransport> client_;

  /**
   * Context acquired from the eventHandler_ if one exists.
   */
  void* opaqueContext_;
};
}
}
}

#endif // #ifndef _THRIFT_SERVER_NettyConnectedClient_H_
