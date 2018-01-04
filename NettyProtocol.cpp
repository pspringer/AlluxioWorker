/*
 * Copyright 2018 by Paul Springer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

	result += readU32(status);
	result += readU64(fileID);
	result += readU64(segStart);
	result += readU64(segSize);

	return result;
	}

uint64_t NettyProtocol::readU32(uint32_t* u32)
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

uint64_t NettyProtocol::writeU16(const uint16_t u16)
	{
	uint16_t net = protocol::TNetworkBigEndian::toWire16(u16);
	this->trans_->write((uint8_t*)&net, 2);
	return 2;
	}

uint64_t NettyProtocol::writeU32(const uint32_t u32)
	{
	uint32_t net = protocol::TNetworkBigEndian::toWire32(u32);
	this->trans_->write((uint8_t*)&net, 4);
	return 4;
	}

uint64_t NettyProtocol::writeU64(const uint64_t u64)
	{
	uint64_t net = protocol::TNetworkBigEndian::toWire64(u64);
	this->trans_->write((uint8_t*)&net, 8);
	return 8;
	}

uint64_t NettyProtocol::writeString(const std::string& str)
	{
	if (str.size() > static_cast<size_t>((std::numeric_limits<int32_t>::max)()))
		throw TProtocolException(TProtocolException::SIZE_LIMIT);
	uint64_t size = static_cast<uint64_t>(str.size());
	if (size > 0)
		this->trans_->write((uint8_t*)str.data(), size);
	return size;
	}

}	// apache::thrift::protocol
}
}
