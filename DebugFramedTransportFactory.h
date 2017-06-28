/*
 * DebugFramedTransportFactory.h
 *
 *  Created on: Jun 25, 2017
 *      Author: pls
 */

#ifndef DEBUGFRAMEDTRANSPORTFACTORY_H_
#define DEBUGFRAMEDTRANSPORTFACTORY_H_

#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransport.h>
#include "DebugFramedTransport.h"

using namespace apache::thrift::transport;

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
