/*
 * TestSimpleServer.cpp
 *
 *  Created on: Jun 20, 2017
 *      Author: pls
 */

#include "DebugSimpleServer.h"

DebugSimpleServer::DebugSimpleServer(
		const boost::shared_ptr<TProcessor>& processor,
		const boost::shared_ptr<TServerTransport>& serverTransport,
		const boost::shared_ptr<TTransportFactory>& transportFactory,
		const boost::shared_ptr<TProtocolFactory>& protocolFactory):
		TSimpleServer(processor, serverTransport,
		transportFactory, protocolFactory)
{

}

