/*
 * NettyProcessor.cpp
 *
 *  Created on: Jul 17, 2017
 *      Author: pls
 */

#include "NettyProcessor.h"

namespace apache {
namespace thrift {


NettyProcessor::NettyProcessor() {
	// TODO Auto-generated constructor stub
}
bool NettyProcessor::process(boost::shared_ptr<protocol::NettyProtocol> in,
		boost::shared_ptr<protocol::NettyProtocol> out)
	{
	uint32_t		status;
	uint64_t		fileID;
	uint64_t		segStart;
	uint64_t		segSize;

	in->readMessageBegin(&status, &fileID, &segStart, &segSize);
	printf( "status %d, file %lld, start %lld, size %lld\n", status, fileID, segStart, segSize);
	return true;
	}


}
}	// namespace apache::thrift
