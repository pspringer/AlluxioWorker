/*
 * Copyright 2018 by Paul Springer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
