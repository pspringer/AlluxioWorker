/*
 * DebugServerSocket.h
 *
 *  Created on: Jun 29, 2017
 *      Author: pls
 */

#ifndef DEBUGSERVERSOCKET_H_
#define DEBUGSERVERSOCKET_H_

#include <thrift/transport/TServerSocket.h>

#include "DebugSocket.h"

using namespace apache::thrift::transport;

class DebugServerSocket: public TServerSocket
{
public:
	DebugServerSocket(int port);
protected:
	virtual boost::shared_ptr<TSocket> createSocket(THRIFT_SOCKET client);
};

#endif /* DEBUGSERVERSOCKET_H_ */
