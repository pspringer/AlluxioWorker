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
#include "block_master.h"

using namespace std;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

int main()
	{
	BlockMasterWorkerService	client;
	boost::shared_ptr<TTransport> socket(new TSocket("192.168.1.137", 19998));
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	boost::shared_ptr<TProtocol> mprotocol(new TMultiplexedProtocol(protocol, "BlockMasterWorker"));

//  Create a client to use the protocol encoder
	BlockMasterWorkerService client(mprotocol);
	return 0;
	}
