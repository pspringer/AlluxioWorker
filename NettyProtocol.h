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

#ifndef NETTYPROTOCOL_H_
#define NETTYPROTOCOL_H_

#include "NettyFramedTransport.h"

namespace apache {
namespace thrift {
namespace protocol {

class NettyProtocol {
public:
	NettyProtocol(boost::shared_ptr<transport::NettyFramedTransport>transport);
	boost::shared_ptr<transport::NettyFramedTransport> getTransport();

	uint64_t readMessageBegin(uint32_t* status, uint64_t* fileID, uint64_t* segStart, uint64_t* segSize);
	uint64_t readU32(uint32_t* u32);
	uint64_t readU64(uint64_t* u64);

	uint64_t writeU16(const uint16_t u16);
	uint64_t writeU32(const uint32_t u32);
	uint64_t writeU64(const uint64_t u64);
	uint64_t writeString(const std::string& str);

protected:
	boost::shared_ptr<transport::NettyFramedTransport>	trans_;
};

}	// namespace apache::thrift::server
}
}

#endif /* NETTYPROTOCOL_H_ */
