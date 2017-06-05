/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef AlluxioService_H
#define AlluxioService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "common_types.h"



#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class AlluxioServiceIf {
 public:
  virtual ~AlluxioServiceIf() {}

  /**
   * Returns the version of the master service.
   * NOTE: The version should be updated every time a backwards incompatible API change occurs.
   */
  virtual int64_t getServiceVersion() = 0;
};

class AlluxioServiceIfFactory {
 public:
  typedef AlluxioServiceIf Handler;

  virtual ~AlluxioServiceIfFactory() {}

  virtual AlluxioServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(AlluxioServiceIf* /* handler */) = 0;
};

class AlluxioServiceIfSingletonFactory : virtual public AlluxioServiceIfFactory {
 public:
  AlluxioServiceIfSingletonFactory(const boost::shared_ptr<AlluxioServiceIf>& iface) : iface_(iface) {}
  virtual ~AlluxioServiceIfSingletonFactory() {}

  virtual AlluxioServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(AlluxioServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<AlluxioServiceIf> iface_;
};

class AlluxioServiceNull : virtual public AlluxioServiceIf {
 public:
  virtual ~AlluxioServiceNull() {}
  int64_t getServiceVersion() {
    int64_t _return = 0;
    return _return;
  }
};


class AlluxioService_getServiceVersion_args {
 public:

  AlluxioService_getServiceVersion_args(const AlluxioService_getServiceVersion_args&);
  AlluxioService_getServiceVersion_args& operator=(const AlluxioService_getServiceVersion_args&);
  AlluxioService_getServiceVersion_args() {
  }

  virtual ~AlluxioService_getServiceVersion_args() throw();

  bool operator == (const AlluxioService_getServiceVersion_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const AlluxioService_getServiceVersion_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AlluxioService_getServiceVersion_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class AlluxioService_getServiceVersion_pargs {
 public:


  virtual ~AlluxioService_getServiceVersion_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _AlluxioService_getServiceVersion_result__isset {
  _AlluxioService_getServiceVersion_result__isset() : success(false) {}
  bool success :1;
} _AlluxioService_getServiceVersion_result__isset;

class AlluxioService_getServiceVersion_result {
 public:

  AlluxioService_getServiceVersion_result(const AlluxioService_getServiceVersion_result&);
  AlluxioService_getServiceVersion_result& operator=(const AlluxioService_getServiceVersion_result&);
  AlluxioService_getServiceVersion_result() : success(0) {
  }

  virtual ~AlluxioService_getServiceVersion_result() throw();
  int64_t success;

  _AlluxioService_getServiceVersion_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const AlluxioService_getServiceVersion_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const AlluxioService_getServiceVersion_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AlluxioService_getServiceVersion_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _AlluxioService_getServiceVersion_presult__isset {
  _AlluxioService_getServiceVersion_presult__isset() : success(false) {}
  bool success :1;
} _AlluxioService_getServiceVersion_presult__isset;

class AlluxioService_getServiceVersion_presult {
 public:


  virtual ~AlluxioService_getServiceVersion_presult() throw();
  int64_t* success;

  _AlluxioService_getServiceVersion_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class AlluxioServiceClient : virtual public AlluxioServiceIf {
 public:
  AlluxioServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  AlluxioServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  int64_t getServiceVersion();
  void send_getServiceVersion();
  int64_t recv_getServiceVersion();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class AlluxioServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<AlluxioServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (AlluxioServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_getServiceVersion(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  AlluxioServiceProcessor(boost::shared_ptr<AlluxioServiceIf> iface) :
    iface_(iface) {
    processMap_["getServiceVersion"] = &AlluxioServiceProcessor::process_getServiceVersion;
  }

  virtual ~AlluxioServiceProcessor() {}
};

class AlluxioServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  AlluxioServiceProcessorFactory(const ::boost::shared_ptr< AlluxioServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< AlluxioServiceIfFactory > handlerFactory_;
};

class AlluxioServiceMultiface : virtual public AlluxioServiceIf {
 public:
  AlluxioServiceMultiface(std::vector<boost::shared_ptr<AlluxioServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~AlluxioServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<AlluxioServiceIf> > ifaces_;
  AlluxioServiceMultiface() {}
  void add(boost::shared_ptr<AlluxioServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int64_t getServiceVersion() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getServiceVersion();
    }
    return ifaces_[i]->getServiceVersion();
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class AlluxioServiceConcurrentClient : virtual public AlluxioServiceIf {
 public:
  AlluxioServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  AlluxioServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  int64_t getServiceVersion();
  int32_t send_getServiceVersion();
  int64_t recv_getServiceVersion(const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif



#endif
