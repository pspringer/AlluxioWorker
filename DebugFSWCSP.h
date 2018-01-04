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

#ifndef TESTFSWCSP_H_
#define TESTFSWCSP_H_

#include "FileSystemWorkerClientService.h"
#include "BlockWorkerClientService.h"

class DebugFSWCSP: public FileSystemWorkerClientServiceProcessor {
public:
	DebugFSWCSP(boost::shared_ptr<FileSystemWorkerClientServiceIf> iface);
protected:
	virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);

};

#endif /* TESTFSWCSP_H_ */
