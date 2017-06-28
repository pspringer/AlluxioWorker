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
#include "thrift/processor/TMultiplexedProcessor.h"
#include "thrift/server/TSimpleServer.h"
#include "FileSystemWorker.h"
#include "FileSystemWorkerClientService.h"
#include "FileSystemWorkerClientServiceHandler.h"
#include "DebugFSWCSP.h"
#include "DebugFramedTransportFactory.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;
using namespace apache::thrift::server;

void FileSystemWorker::callback(THRIFT_SOCKET client)
	{
	struct sockaddr_in	sin;
	unsigned int sinlen = sizeof( sin );

	cout << "Arrived at accept callback\n";
	printf( "About to read in callback for socket %d\n", client );
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
	int retgsn = getsockname( client, (sockaddr*)&sin, &sinlen);
	if (retgsn != 0)
		cout << "Error is " << errno << endl;
	else
		printf( "client returns code %d for %d\n", retgsn, ntohs( sin.sin_port ));
	if (client > 10)
		{
		retgsn = getsockname( client-1, (sockaddr*)&sin, &sinlen);
		if (retgsn != 0)
			cout << "Error is " << errno << endl;
		else
			printf( "client-1 returns code %d for %d\n", retgsn, ntohs( sin.sin_port ));
		}
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
	boost::shared_ptr<DebugFSWCSP> fsProc(new DebugFSWCSP( boost::make_shared<FileSystemWorkerClientServiceHandler>()));
	boost::shared_ptr<TMultiplexedProcessor> fsmProc( new TMultiplexedProcessor );
	fsmProc->registerProcessor("FileSystemWorkerClient", fsProc);
	TSimpleServer server( fsmProc,serverSocket,
			boost::make_shared<DebugFramedTransportFactory>(),
			boost::make_shared<TBinaryProtocolFactory>() );
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
	};
