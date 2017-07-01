/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef file_system_worker_TYPES_H
#define file_system_worker_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>
#include "common_types.h"
#include "exception_types.h"




class CancelUfsFileTOptions;

class CloseUfsFileTOptions;

class CompleteUfsFileTOptions;

class CreateUfsFileTOptions;

class OpenUfsFileTOptions;


class CancelUfsFileTOptions : public virtual ::apache::thrift::TBase {
 public:

  CancelUfsFileTOptions(const CancelUfsFileTOptions&);
  CancelUfsFileTOptions& operator=(const CancelUfsFileTOptions&);
  CancelUfsFileTOptions() {
  }

  virtual ~CancelUfsFileTOptions() throw();

  bool operator == (const CancelUfsFileTOptions & /* rhs */) const
  {
    return true;
  }
  bool operator != (const CancelUfsFileTOptions &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CancelUfsFileTOptions & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CancelUfsFileTOptions &a, CancelUfsFileTOptions &b);

inline std::ostream& operator<<(std::ostream& out, const CancelUfsFileTOptions& obj)
{
  obj.printTo(out);
  return out;
}


class CloseUfsFileTOptions : public virtual ::apache::thrift::TBase {
 public:

  CloseUfsFileTOptions(const CloseUfsFileTOptions&);
  CloseUfsFileTOptions& operator=(const CloseUfsFileTOptions&);
  CloseUfsFileTOptions() {
  }

  virtual ~CloseUfsFileTOptions() throw();

  bool operator == (const CloseUfsFileTOptions & /* rhs */) const
  {
    return true;
  }
  bool operator != (const CloseUfsFileTOptions &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CloseUfsFileTOptions & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CloseUfsFileTOptions &a, CloseUfsFileTOptions &b);

inline std::ostream& operator<<(std::ostream& out, const CloseUfsFileTOptions& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _CompleteUfsFileTOptions__isset {
  _CompleteUfsFileTOptions__isset() : owner(false), group(false), mode(false) {}
  bool owner :1;
  bool group :1;
  bool mode :1;
} _CompleteUfsFileTOptions__isset;

class CompleteUfsFileTOptions : public virtual ::apache::thrift::TBase {
 public:

  CompleteUfsFileTOptions(const CompleteUfsFileTOptions&);
  CompleteUfsFileTOptions& operator=(const CompleteUfsFileTOptions&);
  CompleteUfsFileTOptions() : owner(), group(), mode(0) {
  }

  virtual ~CompleteUfsFileTOptions() throw();
  std::string owner;
  std::string group;
  int16_t mode;

  _CompleteUfsFileTOptions__isset __isset;

  void __set_owner(const std::string& val);

  void __set_group(const std::string& val);

  void __set_mode(const int16_t val);

  bool operator == (const CompleteUfsFileTOptions & rhs) const
  {
    if (__isset.owner != rhs.__isset.owner)
      return false;
    else if (__isset.owner && !(owner == rhs.owner))
      return false;
    if (__isset.group != rhs.__isset.group)
      return false;
    else if (__isset.group && !(group == rhs.group))
      return false;
    if (__isset.mode != rhs.__isset.mode)
      return false;
    else if (__isset.mode && !(mode == rhs.mode))
      return false;
    return true;
  }
  bool operator != (const CompleteUfsFileTOptions &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CompleteUfsFileTOptions & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CompleteUfsFileTOptions &a, CompleteUfsFileTOptions &b);

inline std::ostream& operator<<(std::ostream& out, const CompleteUfsFileTOptions& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _CreateUfsFileTOptions__isset {
  _CreateUfsFileTOptions__isset() : owner(false), group(false), mode(false) {}
  bool owner :1;
  bool group :1;
  bool mode :1;
} _CreateUfsFileTOptions__isset;

class CreateUfsFileTOptions : public virtual ::apache::thrift::TBase {
 public:

  CreateUfsFileTOptions(const CreateUfsFileTOptions&);
  CreateUfsFileTOptions& operator=(const CreateUfsFileTOptions&);
  CreateUfsFileTOptions() : owner(), group(), mode(0) {
  }

  virtual ~CreateUfsFileTOptions() throw();
  std::string owner;
  std::string group;
  int16_t mode;

  _CreateUfsFileTOptions__isset __isset;

  void __set_owner(const std::string& val);

  void __set_group(const std::string& val);

  void __set_mode(const int16_t val);

  bool operator == (const CreateUfsFileTOptions & rhs) const
  {
    if (__isset.owner != rhs.__isset.owner)
      return false;
    else if (__isset.owner && !(owner == rhs.owner))
      return false;
    if (__isset.group != rhs.__isset.group)
      return false;
    else if (__isset.group && !(group == rhs.group))
      return false;
    if (__isset.mode != rhs.__isset.mode)
      return false;
    else if (__isset.mode && !(mode == rhs.mode))
      return false;
    return true;
  }
  bool operator != (const CreateUfsFileTOptions &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CreateUfsFileTOptions & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CreateUfsFileTOptions &a, CreateUfsFileTOptions &b);

inline std::ostream& operator<<(std::ostream& out, const CreateUfsFileTOptions& obj)
{
  obj.printTo(out);
  return out;
}


class OpenUfsFileTOptions : public virtual ::apache::thrift::TBase {
 public:

  OpenUfsFileTOptions(const OpenUfsFileTOptions&);
  OpenUfsFileTOptions& operator=(const OpenUfsFileTOptions&);
  OpenUfsFileTOptions() {
  }

  virtual ~OpenUfsFileTOptions() throw();

  bool operator == (const OpenUfsFileTOptions & /* rhs */) const
  {
    return true;
  }
  bool operator != (const OpenUfsFileTOptions &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const OpenUfsFileTOptions & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(OpenUfsFileTOptions &a, OpenUfsFileTOptions &b);

inline std::ostream& operator<<(std::ostream& out, const OpenUfsFileTOptions& obj)
{
  obj.printTo(out);
  return out;
}



#endif