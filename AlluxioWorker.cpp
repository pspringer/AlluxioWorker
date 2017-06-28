/*
 * main.cpp
 *
 *  Created on: Jun 3, 2017
 *      Author: pls
 */

#include <iostream>
#include "thrift/transport/TSocket.h"
#include "thrift/transport/TBufferTransports.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/protocol/TMultiplexedProtocol.h"
#include "thrift/TOutput.h"
#include "BlockMasterWorkerService.h"
#include "exception_types.h"
#include "FileSystemWorker.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

int main()
	{
	FileSystemWorker*	aFileSystemWorker;
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
	    wna.__set_host("192.168.1.129");
	    wna.__set_rpcPort(29998);
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

	// start up FileSystemWorker
	aFileSystemWorker = new FileSystemWorker();
	cout << "Starting loop\n";
	while (1)
		{
		}
	return 0;
	}
