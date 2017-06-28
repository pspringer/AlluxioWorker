/*
 * DebugFramedTransport.cpp
 *
 *  Created on: Jun 25, 2017
 *      Author: pls
 */

#include "DebugFramedTransport.h"

namespace apache {
namespace thrift {
namespace transport {

DebugFramedTransport::DebugFramedTransport() : TFramedTransport()
{

}

DebugFramedTransport::DebugFramedTransport(boost::shared_ptr<TTransport> transport) :
		TFramedTransport(transport)
{

}

uint32_t DebugFramedTransport::readAll(uint8_t* buf, uint32_t len)
{
	uint8_t* new_rBase = rBase_ + len;
	if (TDB_LIKELY(new_rBase <= rBound_)) {
	  std::memcpy(buf, rBase_, len);
	  rBase_ = new_rBase;
	  GlobalOutput.printf( "Read %d bytes now at %x", len, rBase_ );
	  return len;
	}
	GlobalOutput.printf( "Using slower read for %d bytes", len );
	return apache::thrift::transport::readAll(*this, buf, len);
}

}
}
}
