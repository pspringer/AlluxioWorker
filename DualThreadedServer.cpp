/*
 * DualThreadedServer.cpp
 *
 *  Created on: Jul 7, 2017
 *      Author: pls
 */

#include "DualThreadedServer.h"

namespace apache {
namespace thrift {
namespace server {

using apache::thrift::concurrency::ThreadFactory;
using boost::shared_ptr;

DualThreadedServer::DualThreadedServer(
		const shared_ptr<TProcessor>& processor,
		const shared_ptr<TServerTransport>& serverTransport,
		const shared_ptr<TTransportFactory>& transportFactory,
		const shared_ptr<TProtocolFactory>& protocolFactory,
		const shared_ptr<ThreadFactory>& threadFactory)
: TThreadedServer(processor, serverTransport, transportFactory, protocolFactory, threadFactory)
  {

  }

void DualThreadedServer::serve()
{
	TThreadedServer::serve();
}

}	// namespace
}
}
