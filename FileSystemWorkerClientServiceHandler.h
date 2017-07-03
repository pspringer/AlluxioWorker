/*
 * FileSystemWorkerClientServiceHandler.h
 *
 *  Created on: Jun 16, 2017
 *      Author: pls
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
