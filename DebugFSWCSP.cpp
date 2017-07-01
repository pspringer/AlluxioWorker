/*
 * TestFSWCSP.cpp
 *
 *  Created on: Jun 21, 2017
 *      Author: pls
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

