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
#include "BlockMasterWorkerService.h"
#include "exception_types.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

int main()
	{
	boost::shared_ptr<TTransport> socket(new TSocket("192.168.1.137", 19998));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	boost::shared_ptr<TProtocol> mprotocol(new TMultiplexedProtocol(protocol, "BlockMasterWorker"));

//  Create a client to use the protocol encoder
	BlockMasterWorkerServiceClient client(mprotocol);

	try
		{
//		Connect!
		cout << "Open transport" << endl;
	    transport->open();

	    cout << "Requesting version" << endl;
	    int sv = client.getServiceVersion();
	    cout << "Got version " << sv << endl;

	    cout << "Requesting ID" << endl;
	    long theID = client.getWorkerId(WorkerNetAddress());
	    cout << "Received " << theID << endl;

	    transport->close();
		}
		catch (TException& tx)
			{
			cout << "ERROR: " << tx.what() << endl;
			}
	return 0;
	}
