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
	// compare thrift/FileSystemWorkerClientService
	uint32_t		status;
	uint64_t		fileID;
	uint64_t		segStart;
	uint64_t		segSize;

	in->readMessageBegin(&status, &fileID, &segStart, &segSize);
	printf( "status %d, file %lld, start %lld, size %lld\n", status, fileID, segStart, segSize);

//	  oprot->writeMessageBegin("openUfsFile", ::apache::thrift::protocol::T_REPLY, seqid);
//	  result.write(oprot);
//	  oprot->writeMessageEnd();
//	  bytes = oprot->getTransport()->writeEnd();
//	  oprot->getTransport()->flush();

	uint32_t		buf = 0x6;
	uint16_t		replyStatus = 0;
	std::string		fileContent = "Now\n";

	// TODO:  make this actually work
	// right now it is canned to return "Now\n" and eof (no contents)
	out->writeU32(buf);
	out->writeU64(fileID);
	out->writeU64(segStart);
	if (segStart == 0)			// if start of file contents
		out->writeU64(4);		// size of reply segment
	else
		out->writeU64(0);		// effectively eof
	out->writeU16(replyStatus);
	if (segStart == 0)			// first response of file contents
		out->writeString(fileContent);
	out->getTransport()->flush();

	return true;
	}


}
}	// namespace apache::thrift
