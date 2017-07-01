/*
 * DebugSocket.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: pls
 */

#include <thrift/thrift-config.h>

#include <cstring>
#include <sstream>
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_SYS_UN_H
#include <sys/un.h>
#endif
#ifdef HAVE_SYS_POLL_H
#include <sys/poll.h>
#endif
#include <sys/types.h>
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>

#include <thrift/concurrency/Monitor.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportException.h>
#include <thrift/transport/PlatformSocket.h>

#ifndef SOCKOPT_CAST_T
#ifndef _WIN32
#define SOCKOPT_CAST_T void
#else
#define SOCKOPT_CAST_T char
#endif // _WIN32
#endif

#if defined(_WIN32) && (_WIN32_WINNT < 0x0600)
  #define AI_ADDRCONFIG 0x0400
#endif

template <class T>
inline const SOCKOPT_CAST_T* const_cast_sockopt(const T* v) {
  return reinterpret_cast<const SOCKOPT_CAST_T*>(v);
}

template <class T>
inline SOCKOPT_CAST_T* cast_sockopt(T* v) {
  return reinterpret_cast<SOCKOPT_CAST_T*>(v);
}

#include "DebugSocket.h"

using namespace apache::thrift::transport;
using namespace apache::thrift;

DebugSocket::DebugSocket(THRIFT_SOCKET socket):TSocket(socket)
{
	if (interruptListener_)
		GlobalOutput.printf("Interrupt set, but not in constructor");
}

DebugSocket::DebugSocket(THRIFT_SOCKET socket, boost::shared_ptr<THRIFT_SOCKET> interruptListener):
		TSocket(socket,interruptListener)
{
	GlobalOutput.printf("Interrupt set in constructor");
}

uint32_t DebugSocket::read(uint8_t* buf, uint32_t len)
{
	GlobalOutput.printf("Reading %d bytes", len);
	  if (socket_ == THRIFT_INVALID_SOCKET) {
	    throw TTransportException(TTransportException::NOT_OPEN, "Called read on non-open socket");
	  }

	  int32_t retries = 0;

	  // THRIFT_EAGAIN can be signalled both when a timeout has occurred and when
	  // the system is out of resources (an awesome undocumented feature).
	  // The following is an approximation of the time interval under which
	  // THRIFT_EAGAIN is taken to indicate an out of resources error.
	  uint32_t eagainThresholdMicros = 0;
	  if (recvTimeout_) {
	    // if a readTimeout is specified along with a max number of recv retries, then
	    // the threshold will ensure that the read timeout is not exceeded even in the
	    // case of resource errors
	    eagainThresholdMicros = (recvTimeout_ * 1000) / ((maxRecvRetries_ > 0) ? maxRecvRetries_ : 2);
	  }

	try_again:
	  // Read from the socket
	  struct timeval begin;
	  if (recvTimeout_ > 0) {
	    THRIFT_GETTIMEOFDAY(&begin, NULL);
	  } else {
	    // if there is no read timeout we don't need the TOD to determine whether
	    // an THRIFT_EAGAIN is due to a timeout or an out-of-resource condition.
	    begin.tv_sec = begin.tv_usec = 0;
	  }

	  int got = 0;

	  if (interruptListener_) {
	    struct THRIFT_POLLFD fds[2];
	    GlobalOutput.printf("Interruptible");
	    std::memset(fds, 0, sizeof(fds));
	    fds[0].fd = socket_;
	    fds[0].events = THRIFT_POLLIN;
	    fds[1].fd = *(interruptListener_.get());
	    fds[1].events = THRIFT_POLLIN;

	    int ret = THRIFT_POLL(fds, 2, (recvTimeout_ == 0) ? -1 : recvTimeout_);
	    int errno_copy = THRIFT_GET_SOCKET_ERROR;
	    if (ret < 0) {
	      // error cases
	      if (errno_copy == THRIFT_EINTR && (retries++ < maxRecvRetries_)) {
	        goto try_again;
	      }
	      GlobalOutput.perror("TSocket::read() THRIFT_POLL() ", errno_copy);
	      throw TTransportException(TTransportException::UNKNOWN, "Unknown", errno_copy);
	    } else if (ret > 0) {
	      // Check the interruptListener
	      if (fds[1].revents & THRIFT_POLLIN) {
	        throw TTransportException(TTransportException::INTERRUPTED, "Interrupted");
	      }
	      else
	      	  {
	    	  GlobalOutput.printf("ret was %d, %x for [0] and %x for [1]", ret, fds[0].revents, fds[1].revents);
	      	  }
	    } else /* ret == 0 */ {
	      throw TTransportException(TTransportException::TIMED_OUT, "THRIFT_EAGAIN (timed out)");
	    }

	    // falling through means there is something to recv and it cannot block
	  }
	  got = static_cast<int>(recv(socket_, cast_sockopt(buf), len, 0));
	  // THRIFT_GETTIMEOFDAY can change THRIFT_GET_SOCKET_ERROR
	  int errno_copy = THRIFT_GET_SOCKET_ERROR;

	  // Check for error on read
	  if (got < 0) {
	    if (errno_copy == THRIFT_EAGAIN) {
	      // if no timeout we can assume that resource exhaustion has occurred.
	      if (recvTimeout_ == 0) {
	        throw TTransportException(TTransportException::TIMED_OUT,
	                                  "THRIFT_EAGAIN (unavailable resources)");
	      }
	      // check if this is the lack of resources or timeout case
	      struct timeval end;
	      THRIFT_GETTIMEOFDAY(&end, NULL);
	      uint32_t readElapsedMicros = static_cast<uint32_t>(((end.tv_sec - begin.tv_sec) * 1000 * 1000)
	                                                         + (end.tv_usec - begin.tv_usec));

	      if (!eagainThresholdMicros || (readElapsedMicros < eagainThresholdMicros)) {
	        if (retries++ < maxRecvRetries_) {
	          THRIFT_SLEEP_USEC(50);
	          goto try_again;
	        } else {
	          throw TTransportException(TTransportException::TIMED_OUT,
	                                    "THRIFT_EAGAIN (unavailable resources)");
	        }
	      } else {
	        // infer that timeout has been hit
	        throw TTransportException(TTransportException::TIMED_OUT, "THRIFT_EAGAIN (timed out)");
	      }
	    }

	    // If interrupted, try again
	    if (errno_copy == THRIFT_EINTR && retries++ < maxRecvRetries_) {
	      goto try_again;
	    }

	    if (errno_copy == THRIFT_ECONNRESET) {
	      return 0;
	    }

	    // This ish isn't open
	    if (errno_copy == THRIFT_ENOTCONN) {
	      throw TTransportException(TTransportException::NOT_OPEN, "THRIFT_ENOTCONN");
	    }

	    // Timed out!
	    if (errno_copy == THRIFT_ETIMEDOUT) {
	      throw TTransportException(TTransportException::TIMED_OUT, "THRIFT_ETIMEDOUT");
	    }

	    // Now it's not a try again case, but a real probblez
	    GlobalOutput.perror("TSocket::read() recv() " + getSocketInfo(), errno_copy);

	    // Some other error, whatevz
	    throw TTransportException(TTransportException::UNKNOWN, "Unknown", errno_copy);
	  }

	  return got;
	}
