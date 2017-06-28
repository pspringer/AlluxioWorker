/*
 * DebugFramedTransport.h
 *
 *  Created on: Jun 25, 2017
 *      Author: pls
 */

#ifndef DEBUGFRAMEDTRANSPORT_H_
#define DEBUGFRAMEDTRANSPORT_H_

#include <thrift/transport/TBufferTransports.h>
namespace apache {
namespace thrift {
namespace transport {

class DebugFramedTransport: public TFramedTransport
{
public:

	DebugFramedTransport();
	DebugFramedTransport(boost::shared_ptr<TTransport> transport);
	uint32_t readAll(uint8_t* buf, uint32_t len);
};
}
}
}
#endif /* DEBUGFRAMEDTRANSPORT_H_ */
