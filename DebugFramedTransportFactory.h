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

#ifndef DEBUGFRAMEDTRANSPORTFACTORY_H_
#define DEBUGFRAMEDTRANSPORTFACTORY_H_

#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransport.h>
#include "DebugFramedTransport.h"

using namespace apache::thrift::transport;

/**
 * Wraps a transport into a framed one.
 *
 */
class DebugFramedTransportFactory: public apache::thrift::transport::TFramedTransportFactory {
public:
	DebugFramedTransportFactory() {}
	virtual ~DebugFramedTransportFactory() {}

	  /**
	   * Wraps the transport into a framed one.
	   */
	  virtual boost::shared_ptr<TTransport> getTransport(boost::shared_ptr<TTransport> trans) {
	    return boost::shared_ptr<TTransport>(new DebugFramedTransport(trans));
	  }
};

#endif /* DEBUGFRAMEDTRANSPORTFACTORY_H_ */
