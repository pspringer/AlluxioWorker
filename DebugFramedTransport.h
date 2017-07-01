/*
 * DebugFramedTransport.h
 *
 *  Created on: Jun 25, 2017
 *      Author: pls
 */

#ifndef DEBUGFRAMEDTRANSPORT_H_
#define DEBUGFRAMEDTRANSPORT_H_

#include <boost/scoped_array.hpp>

#include <thrift/transport/TTransport.h>
#include <thrift/transport/TVirtualTransport.h>
#include <thrift/transport/TBufferTransports.h>

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
class DebugFramedTransport : public TVirtualTransport<DebugFramedTransport, TBufferBase> {
public:
  static const int DEFAULT_BUFFER_SIZE = 512;
  static const int DEFAULT_MAX_FRAME_SIZE = 256 * 1024 * 1024;

  /// Use default buffer sizes.
  DebugFramedTransport()
    : transport_(),
      rBufSize_(0),
      wBufSize_(DEFAULT_BUFFER_SIZE),
      rBuf_(),
      wBuf_(new uint8_t[wBufSize_]),
      bufReclaimThresh_((std::numeric_limits<uint32_t>::max)()) {
    initPointers();
  }

  DebugFramedTransport(boost::shared_ptr<TTransport> transport)
    : transport_(transport),
      rBufSize_(0),
      wBufSize_(DEFAULT_BUFFER_SIZE),
      rBuf_(),
      wBuf_(new uint8_t[wBufSize_]),
      bufReclaimThresh_((std::numeric_limits<uint32_t>::max)()),
      maxFrameSize_(DEFAULT_MAX_FRAME_SIZE) {
    initPointers();
  }

  DebugFramedTransport(boost::shared_ptr<TTransport> transport,
                   uint32_t sz,
                   uint32_t bufReclaimThresh = (std::numeric_limits<uint32_t>::max)())
    : transport_(transport),
      rBufSize_(0),
      wBufSize_(sz),
      rBuf_(),
      wBuf_(new uint8_t[wBufSize_]),
      bufReclaimThresh_(bufReclaimThresh),
      maxFrameSize_(DEFAULT_MAX_FRAME_SIZE) {
    initPointers();
  }

  void open() { transport_->open(); }

  bool isOpen() { return transport_->isOpen(); }

  bool peek() { return (rBase_ < rBound_) || transport_->peek(); }

  void close() {
    flush();
    transport_->close();
  }

  virtual uint32_t readSlow(uint8_t* buf, uint32_t len);

  virtual void writeSlow(const uint8_t* buf, uint32_t len);

  virtual void flush();

  uint32_t readEnd();

  uint32_t writeEnd();

  const uint8_t* borrowSlow(uint8_t* buf, uint32_t* len);

  boost::shared_ptr<TTransport> getUnderlyingTransport() { return transport_; }

  /*
   * TVirtualTransport provides a default implementation of readAll().
   * We want to use the TBufferBase version instead.
   */
//  using TBufferBase::readAll;
  uint32_t readAll(uint8_t* buf, uint32_t len);

  /**
   * Returns the origin of the underlying transport
   */
  virtual const std::string getOrigin() { return transport_->getOrigin(); }

  /**
   * Set the maximum size of the frame at read
   */
  void setMaxFrameSize(uint32_t maxFrameSize) { maxFrameSize_ = maxFrameSize; }

  /**
   * Get the maximum size of the frame at read
   */
  uint32_t getMaxFrameSize() { return maxFrameSize_; }

protected:
  /**
   * Reads a frame of input from the underlying stream.
   *
   * Returns true if a frame was read successfully, or false on EOF.
   * (Raises a TTransportException if EOF occurs after a partial frame.)
   */
  virtual bool readFrame();

  void initPointers() {
    setReadBuffer(NULL, 0);
    setWriteBuffer(wBuf_.get(), wBufSize_);

    // Pad the buffer so we can insert the size later.
    int32_t pad = 0;
    this->write((uint8_t*)&pad, sizeof(pad));
  }

  boost::shared_ptr<TTransport> transport_;

  uint32_t rBufSize_;
  uint32_t wBufSize_;
  boost::scoped_array<uint8_t> rBuf_;
  boost::scoped_array<uint8_t> wBuf_;
  uint32_t bufReclaimThresh_;
  uint32_t maxFrameSize_;
};


}	// namespaces
}
}
#endif /* DEBUGFRAMEDTRANSPORT_H_ */
