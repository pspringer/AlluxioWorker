/*
 * NettyFramedTransport.cpp
 *
 *  Created on: Jun 25, 2017
 *      Author: pls
 */

#include "NettyFramedTransport.h"

#include <thrift/protocol/TProtocol.h>

namespace apache {
namespace thrift {
namespace transport {

uint64_t NettyFramedTransport::readAll(uint8_t* buf, uint64_t len)
{
	uint8_t* new_rBase = rBase_ + len;
	if (new_rBase <= rBound_) {
	  std::memcpy(buf, rBase_, len);
	  rBase_ = new_rBase;
	  GlobalOutput.printf( "Read %d bytes now at %x", len, rBase_ );
	  if (len == 8)
	  {
		  uint64_t temp;
		  std::memcpy(&temp,buf,len);
		  printf( "Got this in hex %llx\n", temp );
	  }
	  return len;
	}
	return readMed(buf, len);
}

// From TTransport, the readAll() routine
uint64_t NettyFramedTransport::readMed(uint8_t* buf, uint64_t len) {
	uint64_t have = 0;
	uint64_t get = 0;

	while (have < len) {
		get = read(buf + have, len - have);
		if (get <= 0) {
			throw TTransportException(TTransportException::END_OF_FILE, "No more data to read.");
		}
		have += get;
	}
	return have;
}

// from TBufferBase
uint64_t NettyFramedTransport::read(uint8_t* buf, uint64_t len) {
	uint8_t* new_rBase = rBase_ + len;
	if (new_rBase <= rBound_) {
		std::memcpy(buf, rBase_, len);
		rBase_ = new_rBase;
		return len;
	}
	return readSlow(buf, len);
}

uint64_t NettyFramedTransport::readSlow(uint8_t* buf, uint64_t len) {
  uint64_t want = len;
  uint64_t have = static_cast<uint64_t>(rBound_ - rBase_);

  // We should only take the slow path if we can't satisfy the read
  // with the data already in the buffer.
  assert(have < want);

  // If we have some data in the buffer, copy it out and return it.
  // We have to return it without attempting to read more, since we aren't
  // guaranteed that the underlying transport actually has more data, so
  // attempting to read from it could block.
  if (have > 0) {
    memcpy(buf, rBase_, have);
    setReadBuffer(rBuf_.get(), 0);
    return have;
  }

  // Read another frame.
  if (!readFrame()) {
    // EOF.  No frame available.
    return 0;
  }

  // TODO(dreiss): Should we warn when reads cross frames?

  // Hand over whatever we have.
  uint64_t give = (std::min)(want, static_cast<uint64_t>(rBound_ - rBase_));
  memcpy(buf, rBase_, give);
  rBase_ += give;
  want -= give;

  return (len - want);
}

bool NettyFramedTransport::readFrame() {
  // TODO(dreiss): Think about using readv here, even though it would
  // result in (gasp) read-ahead.

  // Read the size of the next frame.
  // We can't use readAll(&sz, sizeof(sz)), since that always throws an
  // exception on EOF.  We want to throw an exception only if EOF occurs after
  // partial size data.
  int64_t sz = -1;
  uint64_t size_bytes_read = 0;
  while (size_bytes_read < sizeof(sz)) {
    uint8_t* szp = reinterpret_cast<uint8_t*>(&sz) + size_bytes_read;
    uint64_t bytes_read
        = transport_->read(szp, static_cast<uint64_t>(sizeof(sz)) - size_bytes_read);
    if (bytes_read == 8)
    	printf( "Netty frame size is %llx\n", sz );
    if (bytes_read == 0) {
      if (size_bytes_read == 0) {
    	  GlobalOutput.printf( "readFrame() returning false" );
        // EOF before any data was read.
        return false;
      } else {
        // EOF after a partial frame header.  Raise an exception.
        throw TTransportException(TTransportException::END_OF_FILE,
                                  "No more data to read after "
                                  "partial frame header.");
      }
    }
    size_bytes_read += bytes_read;
  }

//  sz = ntohl(sz);
  sz = protocol::TNetworkBigEndian::fromWire64(sz);
  printf( "Netty frame size after conversion is %llx\n", sz );

  if (sz < 0) {
    throw TTransportException("Frame size has negative value");
  }

  // Check for oversized frame
  if (sz > static_cast<int64_t>(maxFrameSize_))
    throw TTransportException(TTransportException::CORRUPTED_DATA, "Received an oversized frame");

  // Read the frame payload, and reset markers.
  GlobalOutput.printf( "Reading frame of %d bytes", sz );
  if (sz > static_cast<int64_t>(rBufSize_)) {
    rBuf_.reset(new uint8_t[sz]);
    rBufSize_ = sz;
  }
//  transport_->readAll(rBuf_.get(), sz);
  // TODO:  sockets can maybe only handle 32 bit lengths, so fix this
  transport_->read(rBuf_.get(), sz);
  setReadBuffer(rBuf_.get(), sz);
  printf( "Read frame into %x\n", rBuf_.get() );
  return true;
}

void NettyFramedTransport::writeSlow(const uint8_t* buf, uint64_t len) {
  // Double buffer size until sufficient.
  uint64_t have = static_cast<uint64_t>(wBase_ - wBuf_.get());
  uint64_t new_size = wBufSize_;
  if (len + have < have /* overflow */ || len + have > 0x7fffffff) {
    throw TTransportException(TTransportException::BAD_ARGS,
                              "Attempted to write over 2 GB to NettyFramedTransport.");
  }
  while (new_size < len + have) {
    new_size = new_size > 0 ? new_size * 2 : 1;
  }

  // TODO(dreiss): Consider modifying this class to use malloc/free
  // so we can use realloc here.

  // Allocate new buffer.
  uint8_t* new_buf = new uint8_t[new_size];

  // Copy the old buffer to the new one.
  memcpy(new_buf, wBuf_.get(), have);

  // Now point buf to the new one.
  wBuf_.reset(new_buf);
  wBufSize_ = new_size;
  wBase_ = wBuf_.get() + have;
  wBound_ = wBuf_.get() + wBufSize_;

  // Copy the data into the new buffer.
  memcpy(wBase_, buf, len);
  wBase_ += len;
}

void NettyFramedTransport::flush() {
  int64_t sz_hbo, sz_nbo;
  assert(wBufSize_ > sizeof(sz_nbo));

  // Slip the frame size into the start of the buffer.
  sz_hbo = static_cast<uint64_t>(wBase_ - (wBuf_.get() + sizeof(sz_nbo)));
  sz_nbo = (int64_t)htonl((uint64_t)(sz_hbo));
  memcpy(wBuf_.get(), (uint8_t*)&sz_nbo, sizeof(sz_nbo));

  if (sz_hbo > 0) {
    // Note that we reset wBase_ (with a pad for the frame size)
    // prior to the underlying write to ensure we're in a sane state
    // (i.e. internal buffer cleaned) if the underlying write throws
    // up an exception
    wBase_ = wBuf_.get() + sizeof(sz_nbo);

    // Write size and frame body.
    transport_->write(wBuf_.get(), static_cast<uint64_t>(sizeof(sz_nbo)) + sz_hbo);
  }

  // Flush the underlying transport.
  transport_->flush();

  // reclaim write buffer
  if (wBufSize_ > bufReclaimThresh_) {
    wBufSize_ = DEFAULT_BUFFER_SIZE;
    wBuf_.reset(new uint8_t[wBufSize_]);
    setWriteBuffer(wBuf_.get(), wBufSize_);

    // reset wBase_ with a pad for the frame size
    int64_t pad = 0;
    wBase_ = wBuf_.get() + sizeof(pad);
  }
}

uint64_t NettyFramedTransport::writeEnd() {
  return static_cast<uint64_t>(wBase_ - wBuf_.get());
}

const uint8_t* NettyFramedTransport::borrowSlow(uint8_t* buf, uint64_t* len) {
  (void)buf;
  (void)len;
  // Don't try to be clever with shifting buffers.
  // If the fast path failed let the protocol use its slow path.
  // Besides, who is going to try to borrow across messages?
  return NULL;
}

uint64_t NettyFramedTransport::readEnd() {
  // include framing bytes
  uint64_t bytes_read = static_cast<uint64_t>(rBound_ - rBuf_.get() + sizeof(uint64_t));

  if (rBufSize_ > bufReclaimThresh_) {
    rBufSize_ = 0;
    rBuf_.reset();
    setReadBuffer(rBuf_.get(), rBufSize_);
  }

  return bytes_read;
}
}	// namespaces
}
}

