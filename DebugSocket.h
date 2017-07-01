/*
 * DebugSocket.h
 *
 *  Created on: Jun 29, 2017
 *      Author: pls
 */

#ifndef DEBUGSOCKET_H_
#define DEBUGSOCKET_H_

#include <thrift/transport/TSocket.h>
#include <thrift/transport/PlatformSocket.h>

class DebugSocket: public apache::thrift::transport::TSocket
{
public:
	DebugSocket(THRIFT_SOCKET socket);
	DebugSocket(THRIFT_SOCKET socket, boost::shared_ptr<THRIFT_SOCKET> interruptListener);
	uint32_t read(uint8_t* buf, uint32_t len);
};

#endif /* DEBUGSOCKET_H_ */
