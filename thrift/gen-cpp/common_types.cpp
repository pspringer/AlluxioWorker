/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "common_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>



int _kCommandTypeValues[] = {
  CommandType::Unknown,
  CommandType::Nothing,
  CommandType::Register,
  CommandType::Free,
  CommandType::Delete,
  CommandType::Persist
};
const char* _kCommandTypeNames[] = {
  "Unknown",
  "Nothing",
  "Register",
  "Free",
  "Delete",
  "Persist"
};
const std::map<int, const char*> _CommandType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(6, _kCommandTypeValues, _kCommandTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kTTtlActionValues[] = {
  TTtlAction::Delete,
  TTtlAction::Free
};
const char* _kTTtlActionNames[] = {
  "Delete",
  "Free"
};
const std::map<int, const char*> _TTtlAction_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(2, _kTTtlActionValues, _kTTtlActionNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));


BlockInfo::~BlockInfo() throw() {
}


void BlockInfo::__set_blockId(const int64_t val) {
  this->blockId = val;
}

void BlockInfo::__set_length(const int64_t val) {
  this->length = val;
}

void BlockInfo::__set_locations(const std::vector<BlockLocation> & val) {
  this->locations = val;
}

uint32_t BlockInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->blockId);
          this->__isset.blockId = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->length);
          this->__isset.length = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->locations.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readListBegin(_etype3, _size0);
            this->locations.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              xfer += this->locations[_i4].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.locations = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t BlockInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("BlockInfo");

  xfer += oprot->writeFieldBegin("blockId", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->blockId);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("length", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->length);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("locations", ::apache::thrift::protocol::T_LIST, 3);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->locations.size()));
    std::vector<BlockLocation> ::const_iterator _iter5;
    for (_iter5 = this->locations.begin(); _iter5 != this->locations.end(); ++_iter5)
    {
      xfer += (*_iter5).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(BlockInfo &a, BlockInfo &b) {
  using ::std::swap;
  swap(a.blockId, b.blockId);
  swap(a.length, b.length);
  swap(a.locations, b.locations);
  swap(a.__isset, b.__isset);
}

BlockInfo::BlockInfo(const BlockInfo& other6) {
  blockId = other6.blockId;
  length = other6.length;
  locations = other6.locations;
  __isset = other6.__isset;
}
BlockInfo& BlockInfo::operator=(const BlockInfo& other7) {
  blockId = other7.blockId;
  length = other7.length;
  locations = other7.locations;
  __isset = other7.__isset;
  return *this;
}
void BlockInfo::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "BlockInfo(";
  out << "blockId=" << to_string(blockId);
  out << ", " << "length=" << to_string(length);
  out << ", " << "locations=" << to_string(locations);
  out << ")";
}


BlockLocation::~BlockLocation() throw() {
}


void BlockLocation::__set_workerId(const int64_t val) {
  this->workerId = val;
}

void BlockLocation::__set_workerAddress(const WorkerNetAddress& val) {
  this->workerAddress = val;
}

void BlockLocation::__set_tierAlias(const std::string& val) {
  this->tierAlias = val;
}

uint32_t BlockLocation::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->workerId);
          this->__isset.workerId = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->workerAddress.read(iprot);
          this->__isset.workerAddress = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->tierAlias);
          this->__isset.tierAlias = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t BlockLocation::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("BlockLocation");

  xfer += oprot->writeFieldBegin("workerId", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->workerId);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("workerAddress", ::apache::thrift::protocol::T_STRUCT, 2);
  xfer += this->workerAddress.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("tierAlias", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->tierAlias);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(BlockLocation &a, BlockLocation &b) {
  using ::std::swap;
  swap(a.workerId, b.workerId);
  swap(a.workerAddress, b.workerAddress);
  swap(a.tierAlias, b.tierAlias);
  swap(a.__isset, b.__isset);
}

BlockLocation::BlockLocation(const BlockLocation& other8) {
  workerId = other8.workerId;
  workerAddress = other8.workerAddress;
  tierAlias = other8.tierAlias;
  __isset = other8.__isset;
}
BlockLocation& BlockLocation::operator=(const BlockLocation& other9) {
  workerId = other9.workerId;
  workerAddress = other9.workerAddress;
  tierAlias = other9.tierAlias;
  __isset = other9.__isset;
  return *this;
}
void BlockLocation::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "BlockLocation(";
  out << "workerId=" << to_string(workerId);
  out << ", " << "workerAddress=" << to_string(workerAddress);
  out << ", " << "tierAlias=" << to_string(tierAlias);
  out << ")";
}


Command::~Command() throw() {
}


void Command::__set_commandType(const CommandType::type val) {
  this->commandType = val;
}

void Command::__set_data(const std::vector<int64_t> & val) {
  this->data = val;
}

uint32_t Command::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast10;
          xfer += iprot->readI32(ecast10);
          this->commandType = (CommandType::type)ecast10;
          this->__isset.commandType = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->data.clear();
            uint32_t _size11;
            ::apache::thrift::protocol::TType _etype14;
            xfer += iprot->readListBegin(_etype14, _size11);
            this->data.resize(_size11);
            uint32_t _i15;
            for (_i15 = 0; _i15 < _size11; ++_i15)
            {
              xfer += iprot->readI64(this->data[_i15]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.data = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Command::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Command");

  xfer += oprot->writeFieldBegin("commandType", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->commandType);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("data", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I64, static_cast<uint32_t>(this->data.size()));
    std::vector<int64_t> ::const_iterator _iter16;
    for (_iter16 = this->data.begin(); _iter16 != this->data.end(); ++_iter16)
    {
      xfer += oprot->writeI64((*_iter16));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Command &a, Command &b) {
  using ::std::swap;
  swap(a.commandType, b.commandType);
  swap(a.data, b.data);
  swap(a.__isset, b.__isset);
}

Command::Command(const Command& other17) {
  commandType = other17.commandType;
  data = other17.data;
  __isset = other17.__isset;
}
Command& Command::operator=(const Command& other18) {
  commandType = other18.commandType;
  data = other18.data;
  __isset = other18.__isset;
  return *this;
}
void Command::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Command(";
  out << "commandType=" << to_string(commandType);
  out << ", " << "data=" << to_string(data);
  out << ")";
}


WorkerNetAddress::~WorkerNetAddress() throw() {
}


void WorkerNetAddress::__set_host(const std::string& val) {
  this->host = val;
}

void WorkerNetAddress::__set_rpcPort(const int32_t val) {
  this->rpcPort = val;
}

void WorkerNetAddress::__set_dataPort(const int32_t val) {
  this->dataPort = val;
}

void WorkerNetAddress::__set_webPort(const int32_t val) {
  this->webPort = val;
}

uint32_t WorkerNetAddress::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->host);
          this->__isset.host = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->rpcPort);
          this->__isset.rpcPort = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->dataPort);
          this->__isset.dataPort = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->webPort);
          this->__isset.webPort = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t WorkerNetAddress::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("WorkerNetAddress");

  xfer += oprot->writeFieldBegin("host", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->host);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("rpcPort", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->rpcPort);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("dataPort", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->dataPort);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("webPort", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->webPort);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(WorkerNetAddress &a, WorkerNetAddress &b) {
  using ::std::swap;
  swap(a.host, b.host);
  swap(a.rpcPort, b.rpcPort);
  swap(a.dataPort, b.dataPort);
  swap(a.webPort, b.webPort);
  swap(a.__isset, b.__isset);
}

WorkerNetAddress::WorkerNetAddress(const WorkerNetAddress& other19) {
  host = other19.host;
  rpcPort = other19.rpcPort;
  dataPort = other19.dataPort;
  webPort = other19.webPort;
  __isset = other19.__isset;
}
WorkerNetAddress& WorkerNetAddress::operator=(const WorkerNetAddress& other20) {
  host = other20.host;
  rpcPort = other20.rpcPort;
  dataPort = other20.dataPort;
  webPort = other20.webPort;
  __isset = other20.__isset;
  return *this;
}
void WorkerNetAddress::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "WorkerNetAddress(";
  out << "host=" << to_string(host);
  out << ", " << "rpcPort=" << to_string(rpcPort);
  out << ", " << "dataPort=" << to_string(dataPort);
  out << ", " << "webPort=" << to_string(webPort);
  out << ")";
}


RpcOptions::~RpcOptions() throw() {
}


void RpcOptions::__set_key(const std::string& val) {
  this->key = val;
__isset.key = true;
}

uint32_t RpcOptions::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->key);
          this->__isset.key = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t RpcOptions::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("RpcOptions");

  if (this->__isset.key) {
    xfer += oprot->writeFieldBegin("key", ::apache::thrift::protocol::T_STRING, 1);
    xfer += oprot->writeString(this->key);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(RpcOptions &a, RpcOptions &b) {
  using ::std::swap;
  swap(a.key, b.key);
  swap(a.__isset, b.__isset);
}

RpcOptions::RpcOptions(const RpcOptions& other21) {
  key = other21.key;
  __isset = other21.__isset;
}
RpcOptions& RpcOptions::operator=(const RpcOptions& other22) {
  key = other22.key;
  __isset = other22.__isset;
  return *this;
}
void RpcOptions::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "RpcOptions(";
  out << "key="; (__isset.key ? (out << to_string(key)) : (out << "<null>"));
  out << ")";
}


