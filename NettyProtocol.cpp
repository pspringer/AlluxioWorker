/*
 * NettyProtocol.cpp
 *
 *  Created on: Jul 17, 2017
 *      Author: pls
 */

#include "NettyProtocol.h"
#include <thrift/protocol/TProtocol.h>

namespace apache {
namespace thrift {
namespace protocol {

using boost::shared_ptr;
using apache::thrift::transport::NettyFramedTransport;

/*
 * Thrift-like protocol for netty.
 * Can't inherit from TProtocol, because it uses int32's
 * in the message sizes, and so on.  And it requires too many
 * virtual functions.
 */
NettyProtocol::NettyProtocol(shared_ptr<NettyFramedTransport>transport):
	trans_(transport)
{
	// TODO Auto-generated constructor stub

}

shared_ptr<NettyFramedTransport> NettyProtocol::getTransport()
	{
	return trans_;
	}

uint64_t NettyProtocol::readMessageBegin(uint32_t* status, uint64_t* fileID, uint64_t* segStart, uint64_t* segSize)
	{
	uint64_t result = 0;
	uint64_t sz;

	result += readU32(status);
	result += readU64(fileID);
	result += readU64(segStart);
	result += readU64(segSize);

	return result;
	}

uint32_t NettyProtocol::readU32(uint32_t* u32)
	{
	this->trans_->readAll(reinterpret_cast<uint8_t*>(u32), 4);
	*u32 = protocol::TNetworkBigEndian::fromWire32(*u32);
	return 4;
	}

uint64_t NettyProtocol::readU64(uint64_t* u64)
	{
	this->trans_->readAll(reinterpret_cast<uint8_t*>(u64), 8);
	*u64 = protocol::TNetworkBigEndian::fromWire64(*u64);
	return 8;
	}

}	// apache::thrift::protocol
}
}
