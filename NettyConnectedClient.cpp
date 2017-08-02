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

#include "NettyConnectedClient.h"

namespace apache {
namespace thrift {
namespace server {

using apache::thrift::TProcessor;
using apache::thrift::protocol::NettyProtocol;
using apache::thrift::protocol::TProtocol;
using apache::thrift::server::TServerEventHandler;
using apache::thrift::transport::TTransport;
using apache::thrift::transport::TTransportException;
using boost::shared_ptr;
using std::string;

NettyConnectedClient::NettyConnectedClient(const shared_ptr<NettyProcessor>& processor,
                                   const shared_ptr<NettyProtocol>& inputProtocol,
                                   const shared_ptr<NettyProtocol>& outputProtocol,
                                   const shared_ptr<TTransport>& client)

  : processor_(processor),
    inputProtocol_(inputProtocol),
    outputProtocol_(outputProtocol),
    client_(client),
    opaqueContext_(0) {
}

NettyConnectedClient::~NettyConnectedClient() {
}

void NettyConnectedClient::run() {
  for (bool done = false; !done;) {
    try {
      if (!processor_->process(inputProtocol_, outputProtocol_)) {
        break;
      }
    } catch (const TTransportException& ttx) {
      switch (ttx.getType()) {
        case TTransportException::END_OF_FILE:
        case TTransportException::INTERRUPTED:
        case TTransportException::TIMED_OUT:
          // Client disconnected or was interrupted or did not respond within the receive timeout.
          // No logging needed.  Done.
          done = true;
          break;

        default: {
          // All other transport exceptions are logged.
          // State of connection is unknown.  Done.
          string errStr = string("NettyConnectedClient died: ") + ttx.what();
          GlobalOutput(errStr.c_str());
          done = true;
          break;
        }
      }
    } catch (const TException& tex) {
      string errStr = string("NettyConnectedClient processing exception: ") + tex.what();
      GlobalOutput(errStr.c_str());
      // Disconnect from client, because we could not process the message.
      done = true;
    }
  }

  cleanup();
}

void NettyConnectedClient::cleanup() {
  try {
    inputProtocol_->getTransport()->close();
  } catch (const TTransportException& ttx) {
    string errStr = string("NettyConnectedClient input close failed: ") + ttx.what();
    GlobalOutput(errStr.c_str());
  }

  try {
    outputProtocol_->getTransport()->close();
  } catch (const TTransportException& ttx) {
    string errStr = string("NettyConnectedClient output close failed: ") + ttx.what();
    GlobalOutput(errStr.c_str());
  }

  try {
    client_->close();
  } catch (const TTransportException& ttx) {
    string errStr = string("NettyConnectedClient client close failed: ") + ttx.what();
    GlobalOutput(errStr.c_str());
  }
}
}
}
} // apache::thrift::server
