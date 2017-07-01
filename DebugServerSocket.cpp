/*
 * DebugServerSocket.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: pls
 */

#include <boost/shared_ptr.hpp>
#include <thrift/TOutput.h>

#include "DebugServerSocket.h"

using boost::shared_ptr;
using namespace apache::thrift;

//class TSocket;

DebugServerSocket::DebugServerSocket(int port):TServerSocket(port)
{
	// TODO Auto-generated constructor stub

}

shared_ptr<TSocket> DebugServerSocket::createSocket(THRIFT_SOCKET clientSocket) {
  if (interruptableChildren_) {
	  GlobalOutput.printf("Creating socket with interrupts");
    return shared_ptr<TSocket>(new DebugSocket(clientSocket, pChildInterruptSockReader_));
  } else {
    return shared_ptr<TSocket>(new DebugSocket(clientSocket));
  }
}
