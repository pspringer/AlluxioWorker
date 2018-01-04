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

#ifndef FILESYSTEMWORKERCLIENTSERVICEHANDLER_H_
#define FILESYSTEMWORKERCLIENTSERVICEHANDLER_H_

#include "FileSystemWorkerClientService.h"
#include "FileSystemWorker.h"

class FileSystemWorkerClientServiceHandler :
		virtual public FileSystemWorkerClientServiceIf
{
 public:
  FileSystemWorkerClientServiceHandler(boost::shared_ptr<FileSystemWorker> fsWorker);
  int64_t getServiceVersion();

  void cancelUfsFile(const int64_t sessionId, const int64_t tempUfsFileId, const CancelUfsFileTOptions& options);
  void closeUfsFile(const int64_t sessionId, const int64_t tempUfsFileId, const CloseUfsFileTOptions& options);
  int64_t completeUfsFile(const int64_t sessionId, const int64_t tempUfsFileId, const CompleteUfsFileTOptions& options);
  int64_t createUfsFile(const int64_t sessionId, const std::string& ufsPath, const CreateUfsFileTOptions& options);
  int64_t openUfsFile(const int64_t sessionId, const std::string& ufsPath, const OpenUfsFileTOptions& options);
  void sessionHeartbeat(const int64_t sessionId, const std::vector<int64_t> & metrics);
};

#endif /* FILESYSTEMWORKERCLIENTSERVICEHANDLER_H_ */
