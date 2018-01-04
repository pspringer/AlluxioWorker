/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Modified by Paul Springer
 * Modifications copyright 2018 by Paul Springer
 * Modifications licensed under the Apache License, Version 2.0 (the "License");
 * you may not use the modifications except in compliance with the License.
 */

#include "DebugFramedTransport.h"

namespace apache {
namespace thrift {
namespace transport {

uint32_t DebugFramedTransport::readAll(uint8_t* buf, uint32_t len)
{
	uint8_t* new_rBase = rBase_ + len;
	if (TDB_LIKELY(new_rBase <= rBound_)) {
	  std::memcpy(buf, rBase_, len);
	  rBase_ = new_rBase;
	  GlobalOutput.printf( "Read %d bytes now at %x", len, rBase_ );
	  return len;
	}
	return apache::thrift::transport::readAll(*this, buf, len);
}

uint32_t DebugFramedTransport::readSlow(uint8_t* buf, uint32_t len) {
  uint32_t want = len;
  uint32_t have = static_cast<uint32_t>(rBound_ - rBase_);

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
  uint32_t give = (std::min)(want, static_cast<uint32_t>(rBound_ - rBase_));
  memcpy(buf, rBase_, give);
  rBase_ += give;
  want -= give;

  return (len - want);
}

bool DebugFramedTransport::readFrame() {
  // TODO(dreiss): Think about using readv here, even though it would
  // result in (gasp) read-ahead.

  // Read the size of the next frame.
  // We can't use readAll(&sz, sizeof(sz)), since that always throws an
  // exception on EOF.  We want to throw an exception only if EOF occurs after
  // partial size data.
  int32_t sz = -1;
  uint32_t size_bytes_read = 0;
  while (size_bytes_read < sizeof(sz)) {
    uint8_t* szp = reinterpret_cast<uint8_t*>(&sz) + size_bytes_read;
    uint32_t bytes_read
        = transport_->read(szp, static_cast<uint32_t>(sizeof(sz)) - size_bytes_read);
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

  sz = ntohl(sz);

  if (sz < 0) {
    throw TTransportException("Frame size has negative value");
  }

  // Check for oversized frame
  if (sz > static_cast<int32_t>(maxFrameSize_))
    throw TTransportException(TTransportException::CORRUPTED_DATA, "Received an oversized frame");

  // Read the frame payload, and reset markers.
  GlobalOutput.printf( "Reading frame of %d bytes", sz );
  if (sz > static_cast<int32_t>(rBufSize_)) {
    rBuf_.reset(new uint8_t[sz]);
    rBufSize_ = sz;
  }
  transport_->readAll(rBuf_.get(), sz);
  setReadBuffer(rBuf_.get(), sz);
  return true;
}

void DebugFramedTransport::writeSlow(const uint8_t* buf, uint32_t len) {
  // Double buffer size until sufficient.
  uint32_t have = static_cast<uint32_t>(wBase_ - wBuf_.get());
  uint32_t new_size = wBufSize_;
  if (len + have < have /* overflow */ || len + have > 0x7fffffff) {
    throw TTransportException(TTransportException::BAD_ARGS,
                              "Attempted to write over 2 GB to DebugFramedTransport.");
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

void DebugFramedTransport::flush() {
  int32_t sz_hbo, sz_nbo;
  assert(wBufSize_ > sizeof(sz_nbo));

  // Slip the frame size into the start of the buffer.
  sz_hbo = static_cast<uint32_t>(wBase_ - (wBuf_.get() + sizeof(sz_nbo)));
  sz_nbo = (int32_t)htonl((uint32_t)(sz_hbo));
  memcpy(wBuf_.get(), (uint8_t*)&sz_nbo, sizeof(sz_nbo));

  if (sz_hbo > 0) {
    // Note that we reset wBase_ (with a pad for the frame size)
    // prior to the underlying write to ensure we're in a sane state
    // (i.e. internal buffer cleaned) if the underlying write throws
    // up an exception
    wBase_ = wBuf_.get() + sizeof(sz_nbo);

    // Write size and frame body.
    transport_->write(wBuf_.get(), static_cast<uint32_t>(sizeof(sz_nbo)) + sz_hbo);
  }

  // Flush the underlying transport.
  transport_->flush();

  // reclaim write buffer
  if (wBufSize_ > bufReclaimThresh_) {
    wBufSize_ = DEFAULT_BUFFER_SIZE;
    wBuf_.reset(new uint8_t[wBufSize_]);
    setWriteBuffer(wBuf_.get(), wBufSize_);

    // reset wBase_ with a pad for the frame size
    int32_t pad = 0;
    wBase_ = wBuf_.get() + sizeof(pad);
  }
}

uint32_t DebugFramedTransport::writeEnd() {
  return static_cast<uint32_t>(wBase_ - wBuf_.get());
}

const uint8_t* DebugFramedTransport::borrowSlow(uint8_t* buf, uint32_t* len) {
  (void)buf;
  (void)len;
  // Don't try to be clever with shifting buffers.
  // If the fast path failed let the protocol use its slow path.
  // Besides, who is going to try to borrow across messages?
  return NULL;
}

uint32_t DebugFramedTransport::readEnd() {
  // include framing bytes
  uint32_t bytes_read = static_cast<uint32_t>(rBound_ - rBuf_.get() + sizeof(uint32_t));

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

