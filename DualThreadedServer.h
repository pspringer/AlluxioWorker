/*
 * DualThreadedServer.h
 *
 *  Created on: Jul 7, 2017
 *      Author: pls
 */

#ifndef DUALTHREADEDSERVER_H_
#define DUALTHREADEDSERVER_H_

#include <thrift/server/TThreadedServer.h>

namespace apache {
namespace thrift {
namespace server {

class DualThreadedServer: public apache::thrift::server::TThreadedServer
{
public:
	DualThreadedServer(
	      const boost::shared_ptr<apache::thrift::TProcessor>& processor,
	      const boost::shared_ptr<apache::thrift::transport::TServerTransport>& serverTransport,
	      const boost::shared_ptr<apache::thrift::transport::TTransportFactory>& transportFactory,
	      const boost::shared_ptr<apache::thrift::protocol::TProtocolFactory>& protocolFactory,
	      const boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>& threadFactory
	      = boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>(
	          new apache::thrift::concurrency::PlatformThreadFactory(false)));
	void serve();
};

}	// namespace
}
}
#endif /* DUALTHREADEDSERVER_H_ */
