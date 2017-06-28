/*
 * TestFSWCSP.h
 *
 *  Created on: Jun 21, 2017
 *      Author: pls
 */

#ifndef TESTFSWCSP_H_
#define TESTFSWCSP_H_

#include "FileSystemWorkerClientService.h"

class DebugFSWCSP: public FileSystemWorkerClientServiceProcessor {
public:
	DebugFSWCSP(boost::shared_ptr<FileSystemWorkerClientServiceIf> iface);
protected:
	virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);

};

#endif /* TESTFSWCSP_H_ */
