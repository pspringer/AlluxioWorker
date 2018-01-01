/*
 * NettyFramedTransport.h
 *
 *  Created on: Jun 25, 2017
 *      Author: pls
 */

#ifndef NettyFramedTransport_H_
#define NettyFramedTransport_H_

#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

#include <thrift/transport/TTransport.h>

namespace apache {
namespace thrift {
namespace transport {

/**
 * Framed transport. All writes go into an in-memory buffer until flush is
 * called, at which point the transport writes the length of the entire
 * binary chunk followed by the data payload. This allows the receiver on the
 * other end to always do fixed-length reads.
 *
 */
class NettyFramedTransport{
public:
  static const int DEFAULT_BUFFER_SIZE = 512;
  static const int DEFAULT_MAX_FRAME_SIZE = 256 * 1024 * 1024;

  /// Use default buffer sizes.
//  NettyFramedTransport()
//    : transport_(),
//      rBufSize_(0),
//      wBufSize_(DEFAULT_BUFFER_SIZE),
//      rBuf_(),
//      wBuf_(new uint8_t[wBufSize_]),
//      bufReclaimThresh_((std::numeric_limits<uint64_t>::max)()) {
//    initPointers();
//  }

  NettyFramedTransport(boost::shared_ptr<TTransport> transport)
    : transport_(transport),
      rBufSize_(0),
      wBufSize_(DEFAULT_BUFFER_SIZE),
      rBuf_(),
      wBuf_(new uint8_t[wBufSize_]),
      bufReclaimThresh_((std::numeric_limits<uint64_t>::max)()),
      maxFrameSize_(DEFAULT_MAX_FRAME_SIZE) {
    initPointers();
  }

//  NettyFramedTransport(boost::shared_ptr<TTransport> transport,
//                   uint64_t sz,
//                   uint64_t bufReclaimThresh = (std::numeric_limits<uint64_t>::max)())
//    : transport_(transport),
//      rBufSize_(0),
//      wBufSize_(sz),
//      rBuf_(),
//      wBuf_(new uint8_t[wBufSize_]),
//      bufReclaimThresh_(bufReclaimThresh),
//      maxFrameSize_(DEFAULT_MAX_FRAME_SIZE) {
//    initPointers();
//  }

  void open() { transport_->open(); }

  bool isOpen() { return transport_->isOpen(); }

  bool peek() { return (rBase_ < rBound_) || transport_->peek(); }

  void close() {
    flush();
    transport_->close();
  }

  uint64_t read(uint8_t* buf, uint64_t len);
  uint64_t readMed(uint8_t* buf, uint64_t len);
  uint64_t readSlow(uint8_t* buf, uint64_t len);

  void write(const uint8_t* buf, uint64_t len);
  void writeSlow(const uint8_t* buf, uint64_t len);

  void flush();

  uint64_t readEnd();

  uint64_t writeEnd();

  const uint8_t* borrowSlow(uint8_t* buf, uint64_t* len);

  boost::shared_ptr<TTransport> getUnderlyingTransport() { return transport_; }

  /*
   * TVirtualTransport provides a default implementation of readAll().
   * We want to use the TBufferBase version instead.
   */
//  using TBufferBase::readAll;
  uint64_t readAll(uint8_t* buf, uint64_t len);

  /**
   * Returns the origin of the underlying transport
   */
  const std::string getOrigin() { return transport_->getOrigin(); }

  /**
   * Set the maximum size of the frame at read
   */
  void setMaxFrameSize(uint64_t maxFrameSize) { maxFrameSize_ = maxFrameSize; }

  /**
   * Get the maximum size of the frame at read
   */
  uint64_t getMaxFrameSize() { return maxFrameSize_; }

protected:
  /**
   * Reads a frame of input from the underlying stream.
   *
   * Returns true if a frame was read successfully, or false on EOF.
   * (Raises a TTransportException if EOF occurs after a partial frame.)
   */
  bool readFrame();

  void setReadBuffer(uint8_t* buf, uint64_t len) {
	  rBase_ = buf;
	  rBound_ = buf + len;
  }

  void setWriteBuffer(uint8_t* buf, uint32_t len) {
	  wBase_ = buf;
	  wBound_ = buf + len;
  }
  void initPointers() {
	// read buffer is initially 0, then dynamically resized as needed
    setReadBuffer(NULL, 0);
    setWriteBuffer(wBuf_.get(), wBufSize_);

    // Pad the buffer so we can insert the size later.
    int64_t pad = 0;
    this->write((uint8_t*)&pad, sizeof(pad));
  }

  boost::shared_ptr<TTransport> transport_;

  uint64_t rBufSize_;
  uint64_t wBufSize_;
  boost::scoped_array<uint8_t> rBuf_;
  boost::scoped_array<uint8_t> wBuf_;
  uint64_t bufReclaimThresh_;
  uint64_t maxFrameSize_;

  uint8_t* rBase_;
  uint8_t* rBound_;

  uint8_t* wBase_;
  uint8_t* wBound_;
};


}	// namespaces
}
}
#endif /* NettyFramedTransport_H_ */
