/*
 * NettyProtocol.h
 *
 *  Created on: Jul 17, 2017
 *      Author: pls
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
	uint32_t readU32(uint32_t* u32);
	uint64_t readU64(uint64_t* u64);

protected:
	boost::shared_ptr<transport::NettyFramedTransport>	trans_;
};

}	// namespace apache::thrift::server
}
}

#endif /* NETTYPROTOCOL_H_ */