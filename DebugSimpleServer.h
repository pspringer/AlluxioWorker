/*
 * TestSimpleServer.h
 *
 *  Created on: Jun 20, 2017
 *      Author: pls
 */

#ifndef TESTSIMPLESERVER_H_
#define TESTSIMPLESERVER_H_

#include <thrift/server/TSimpleServer.h>

using namespace ::apache::thrift::server;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift;

class DebugSimpleServer: public TSimpleServer
	{
	public:
	DebugSimpleServer(
		const boost::shared_ptr<TProcessor>& processor,
		const boost::shared_ptr<TServerTransport>& serverTransport,
		const boost::shared_ptr<TTransportFactory>& transportFactory,
		const boost::shared_ptr<TProtocolFactory>& protocolFactory);
	};

#endif /* TESTSIMPLESERVER_H_ */
