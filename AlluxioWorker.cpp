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
#include <boost/make_shared.hpp>

#include "thrift/transport/TSocket.h"
#include "thrift/transport/TBufferTransports.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/protocol/TMultiplexedProtocol.h"
#include "thrift/TOutput.h"
#include "thrift/processor/TMultiplexedProcessor.h"
#include "thrift/server/TSimpleServer.h"
#include "thrift/server/TThreadedServer.h"
#include "thrift/server/TNonblockingServer.h"
#include "thrift/concurrency/ThreadManager.h"

#include "BlockMasterWorkerService.h"
#include "exception_types.h"
#include "FileSystemWorker.h"
#include "BlockWorker.h"
#include "FileSystemWorker.h"
#include "FileSystemWorkerClientService.h"
#include "FileSystemWorkerClientServiceHandler.h"
#include "BlockWorkerClientServiceHandler.h"
#include "DebugFSWCSP.h"
#include "DebugFramedTransportFactory.h"
#include "DebugServerSocket.h"
#include "DualThreadedServer.h"
#include "NonblockingServerSocket.h"
#include "NettyProcessor.h"
#include "NettyProtocol.h"
#include "NettyFramedTransport.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift::server;
using namespace apache::thrift::concurrency;

void createThriftServer()
	{
	// netty setup
	boost::shared_ptr<TServerTransport>	nettyServerTransport( new NonblockingServerSocket( 29999 ));
	boost::shared_ptr<NonblockingServerSocket> nettySocket = boost::dynamic_pointer_cast<NonblockingServerSocket>(nettyServerTransport);
	nettySocket->setAcceptTimeout(1);	// wait for just 1 ms
	boost::shared_ptr<NettyProcessor> nettyProc(new NettyProcessor());

	// setup for FileWorker and BlockWorker
	boost::shared_ptr<FileSystemWorker> theFileSystemWorker(new FileSystemWorker());
	boost::shared_ptr<BlockWorker> aBlockWorker(new BlockWorker());
	boost::shared_ptr<TServerTransport>	aTransport( new NonblockingServerSocket( 29998 ));
	boost::shared_ptr<NonblockingServerSocket> serverSocket = boost::dynamic_pointer_cast<NonblockingServerSocket>(aTransport);
	TServerSocket::socket_func_t cb = &(FileSystemWorker::callback);
	serverSocket->setAcceptCallback(cb);
	serverSocket->setAcceptTimeout(1);	// wait for just 1 ms
//	serverSocket->setAcceptTimeout(-1);	// block till accept (default)
	boost::shared_ptr<TMultiplexedProcessor> RPCProc( new TMultiplexedProcessor );
	boost::shared_ptr<DebugFSWCSP> fsProc(new DebugFSWCSP( boost::make_shared<FileSystemWorkerClientServiceHandler>(theFileSystemWorker)));
	RPCProc->registerProcessor("FileSystemWorkerClient", fsProc);
	boost::shared_ptr<BlockWorkerClientServiceProcessor> bwProc(new BlockWorkerClientServiceProcessor( boost::make_shared<BlockWorkerClientServiceHandler>()));
	RPCProc->registerProcessor("BlockMasterClient", bwProc);
//	boost::shared_ptr<ThreadManager> stm(ThreadManager::newSimpleThreadManager());
//	TNonblockingServer server( RPCProc,
//			boost::make_shared<DebugFramedTransportFactory>(),
//			boost::make_shared<DebugFramedTransportFactory>(),
//			boost::make_shared<TBinaryProtocolFactory>(),
//			boost::make_shared<TBinaryProtocolFactory>(),
//			29998, stm);
	// TODO:  pass netty factories
	DualThreadedServer server( RPCProc,serverSocket,
			boost::make_shared<DebugFramedTransportFactory>(),
			boost::make_shared<TBinaryProtocolFactory>(),
			nettyProc, nettySocket);
//	TSimpleServer server( fsmProc,serverSocket,
//			boost::make_shared<DebugFramedTransportFactory>(),
//			boost::make_shared<TBinaryProtocolFactory>() );
//	TSimpleServer server( fsmProc,serverSocket,
//			boost::make_shared<TBufferedTransportFactory>(),
//			boost::make_shared<TBinaryProtocolFactory>() );
//	TSimpleServer server( fsProc,serverSocket,
//			boost::make_shared<TFramedTransportFactory>(),
//			boost::make_shared<TBinaryProtocolFactory>() );
//	TSimpleServer server( boost::make_shared<FileSystemWorkerClientServiceProcessor>(boost::make_shared<FileSystemWorkerClientServiceHandler>()),serverSocket,
//			boost::make_shared<TFramedTransportFactory>(),
//			boost::make_shared<TBinaryProtocolFactory>() );

	cout << "Serving\n";
	server.serve();
	cout << "Terminating\n";
	}

int main()
	{
	unsigned char		msgBuf[1024];

	boost::shared_ptr<TTransport> socket(new TSocket("192.168.1.137", 19998));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	boost::shared_ptr<TProtocol> mprotocol(new TMultiplexedProtocol(protocol, "BlockMasterWorker"));

//  Create a client to use the protocol encoder
	GlobalOutput.printf( "Starting main\n" );
	BlockMasterWorkerServiceClient client(mprotocol);

	try
		{
//		Connect!
		cout << "Open transport" << endl;
	    transport->open();

	    cout << "Sending PLAIN" << endl;
	    memcpy( msgBuf, "\x01\x00\x00\x00\x05PLAIN", 10);
	    socket->write(msgBuf,10);

	    cout << "Sending noPassword" << endl;
	    memcpy( msgBuf, "\x05\x00\x00\x00\x0f\x00pls\x00noPassword", 20);
	    socket->write(msgBuf,20);

	    cout<< "Waiting for read" << endl;
	    socket->read(msgBuf,1024);
	    cout << msgBuf << endl;

	    cout << "Requesting version" << endl;
	    int sv = client.getServiceVersion();
	    cout << "Got version " << sv << endl;

	    cout << "Requesting ID" << endl;
	    WorkerNetAddress wna = WorkerNetAddress();
	    // TODO:  use a getHostID call
	    wna.__set_host("192.168.1.69");
	    wna.__set_rpcPort(29998);
	    wna.__set_dataPort(29999);
//	    int64_t theID = client.getWorkerId(WorkerNetAddress("192.168.1.129",0,0,0));
	    int64_t theID = client.getWorkerId(wna);
	    cout << "Received " << theID << endl;

	    cout << "Registering..." << endl;
	    vector<string> storageTiers(1,"one");
	    map<string,int64_t> totalBytesOnTiers;
	    totalBytesOnTiers["one"] = 876000;
	    map<string,int64_t> usedBytesOnTiers;
	    usedBytesOnTiers["one"] = 65000;
	    map<string,vector<int64_t>> currentBlocksOnTiers;
	    currentBlocksOnTiers["one"] = vector<int64_t>();
	    client.registerWorker(theID, storageTiers, totalBytesOnTiers,
	        usedBytesOnTiers, currentBlocksOnTiers);

//	    transport->close();
		}
	catch (TException& tx)
		{
		cout << "ERROR: " << tx.what() << endl;
		}

	// start up FileSystemWorker & BlockWorker
	createThriftServer();
	}
