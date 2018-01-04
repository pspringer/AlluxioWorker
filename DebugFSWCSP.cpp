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

#include <iostream>

#include "thrift/TOutput.h"
#include "DebugFSWCSP.h"

using namespace std;
using namespace apache::thrift;
using apache::thrift::transport::TTransportException;

DebugFSWCSP::DebugFSWCSP(boost::shared_ptr<FileSystemWorkerClientServiceIf> iface):FileSystemWorkerClientServiceProcessor(iface)
	{
	}

bool DebugFSWCSP::dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext)
	{
	string 					myfname;
	protocol::TMessageType	mtype;
	bool 					retVal = true;

	myfname = fname;
	GlobalOutput.printf( "Entering my dispatchCall with %s, seqid %d", myfname.c_str(), seqid );
//	while (retVal)
//	{
	try
		{
		retVal = FileSystemWorkerClientServiceProcessor::dispatchCall(iprot, oprot, myfname, seqid, callContext);
		}
	catch (const TTransportException& ttx)
		{
		string errStr = string("TConnectedClient died: ") + ttx.what();
		GlobalOutput(errStr.c_str());
		throw;
		}
	if (!retVal)
		GlobalOutput.printf( "False return from dispatch\n" );
	else
		GlobalOutput.printf( "True return from dispatch\n" );
//	try
//		{
//		iprot->readMessageBegin(myfname,mtype,seqid);
//		GlobalOutput.printf( "readMessageBegin returned with %s, %d, %d", myfname.c_str(), mtype, seqid );
//		}
//	catch (const TTransportException& ttx)
//		{
//		string errStr = string("readMessage died: ") + ttx.what();
//		GlobalOutput(errStr.c_str());
//		throw;
//		}
//	}	// while retVal
	return retVal;
	}

