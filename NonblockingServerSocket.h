/*
 * NonblockingServerSocket.h
 *
 *  Created on: Jul 7, 2017
 *      Author: pls
 */

#ifndef NONBLOCKINGSERVERSOCKET_H_
#define NONBLOCKINGSERVERSOCKET_H_

#include <thrift/transport/TServerSocket.h>

class NonblockingServerSocket: public apache::thrift::transport::TServerSocket {
public:
	NonblockingServerSocket(int port);
};

#endif /* NONBLOCKINGSERVERSOCKET_H_ */
