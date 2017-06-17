/*
 * FileSystemWorker.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: pls
 */

#include <iostream>
#include <boost/make_shared.hpp>

#include "thrift/transport/TServerSocket.h"
#include "thrift/transport/TServerTransport.h"
#include "thrift/transport/TBufferTransports.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/server/TSimpleServer.h"
#include "FileSystemWorker.h"
#include "FileSystemWorkerClientService.h"
#include "FileSystemWorkerClientServiceHandler.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift::server;

void FileSystemWorker::callback(THRIFT_SOCKET client)
	{
	cout << "Arrived at accept callback\n";
	cout << "About to read in callback\n";
	unsigned char	buf[1000];
	int count = recv(client, buf, 1000, 0);
	if (count < 0)
		cout << "Error in receiving " << errno << "\n";
	cout << "Count is " << count << endl;
	for (int i = 0; i < count; i++)
		{
		if (buf[i] < 16)
			cout << "\\x0" << hex << (int)buf[i];
		else
			if (buf[i] >= 48)
				cout << buf[i];
			else
				cout << '\\' << 'x' << hex << (int)buf[i];
		}
	// see if we need a second read
	if (count < 30)
		{
		cout << "\nAbout to do second read in callback\n";
		int count = recv(client, buf, 1000, 0);
		if (count < 0)
			cout << "Error in receiving " << errno << "\n";
		cout << "Count is " << count << endl;
		for (int i = 0; i < count; i++)
			{
			if (buf[i] < 16)
				cout << "\\x0" << hex << (int)buf[i];
			else
				if (buf[i] >= 48)
					cout << buf[i];
				else
					cout << '\\' << 'x' << hex << (int)buf[i];
			}
		}	// if count < 30
	// now send back a response
	cout << "\nAbout to send\n";
	bzero(buf, sizeof(buf));
	buf[0] = 5;
	// or use write()
	send( client, buf, 6, 0);
	cout << "Connection complete\n";

//	cout << "About to read command\n";
//	count = recv(client, buf, 1000, 0);
//	if (count < 0)
//		cout << "Error in receiving " << errno << "\n";
//	cout << "Count is " << count << endl;
//	for (int i = 0; i < count; i++)
//		{
//		if (buf[i] < 16)
//			cout << "\\x0" << hex << (int)buf[i];
//		else
//			if (buf[i] >= 48)
//				cout << buf[i];
//			else
//				cout << '\\' << 'x' << hex << (int)buf[i];
//		}
	}

FileSystemWorker::FileSystemWorker()
	{
	cout << "Starting FileSystemWorker\n";
	boost::shared_ptr<TServerTransport> 	aTransport( new TServerSocket( 29998 ));
	boost::shared_ptr<TServerSocket> serverSocket = boost::dynamic_pointer_cast<TServerSocket>(aTransport);
	TServerSocket::socket_func_t cb = &(FileSystemWorker::callback);
	serverSocket->setAcceptCallback(cb);
	TSimpleServer server( boost::make_shared<FileSystemWorkerClientServiceProcessor>(boost::make_shared<FileSystemWorkerClientServiceHandler>()),serverSocket,
			boost::make_shared<TFramedTransportFactory>(),
			boost::make_shared<TBinaryProtocolFactory>() );

	cout << "Serving\n";
	server.serve();
	cout << "Terminating\n";
	};
