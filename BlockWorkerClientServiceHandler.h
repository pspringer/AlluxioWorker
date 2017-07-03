/*
 * BlockWorkerClientServiceHandler.h
 *
 *  Created on: Jun 16, 2017
 *      Author: pls
 */

#ifndef BLOCKWORKERCLIENTSERVICEHANDLER_H_
#define BLOCKWORKERCLIENTSERVICEHANDLER_H_

#include "BlockWorkerClientService.h"

class BlockWorkerClientServiceHandler :
		virtual public BlockWorkerClientServiceIf
{
 public:
  BlockWorkerClientServiceHandler();
  int64_t getServiceVersion();

  void accessBlock(const int64_t blockId);
  void cacheBlock(const int64_t sessionId, const int64_t blockId);
  void cancelBlock(const int64_t sessionId, const int64_t blockId);
  void lockBlock(LockBlockResult& _return, const int64_t blockId, const int64_t sessionId);
  bool promoteBlock(const int64_t blockId);
  void removeBlock(const int64_t blockId);
  void requestBlockLocation(std::string& _return, const int64_t sessionId, const int64_t blockId, const int64_t initialBytes, const int32_t writeTier);
  bool requestSpace(const int64_t sessionId, const int64_t blockId, const int64_t requestBytes);
  void sessionHeartbeat(const int64_t sessionId, const std::vector<int64_t> & metrics);
  bool unlockBlock(const int64_t blockId, const int64_t sessionId);
};

#endif /* BLOCKWORKERCLIENTSERVICEHANDLER_H_ */
