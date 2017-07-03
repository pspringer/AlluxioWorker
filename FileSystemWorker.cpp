/*
 * FileSystemWorker.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: pls
 */

#include <iostream>

#include "thrift/transport/TServerSocket.h"
#include "thrift/transport/TServerTransport.h"
#include "thrift/transport/TBufferTransports.h"
#include "thrift/protocol/TBinaryProtocol.h"

#include "FileSystemWorker.h"

using namespace std;

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
	send( client, buf, 5, 0);
	cout << "Connection complete\n";
	int retgsn = getsockname( client, (sockaddr*)&sin, &sinlen);
	if (retgsn != 0)
		cout << "Error is " << errno << endl;
	else
		printf( "client returns code %d for %d\n", retgsn, ntohs( sin.sin_port ));
//	if (client > 10)
//		{
//		retgsn = getsockname( client-1, (sockaddr*)&sin, &sinlen);
//		if (retgsn != 0)
//			cout << "Error is " << errno << endl;
//		else
//			printf( "client-1 returns code %d for %d\n", retgsn, ntohs( sin.sin_port ));
//		}
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
	};
