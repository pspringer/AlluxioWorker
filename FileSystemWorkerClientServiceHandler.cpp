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

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "FileSystemWorkerClientService.h"
#include "FileSystemWorkerClientServiceHandler.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

FileSystemWorkerClientServiceHandler::FileSystemWorkerClientServiceHandler(shared_ptr<FileSystemWorker> fsWorker)

  {
    // Your initialization goes here
	  printf("FileSystemWorkerClientServiceHandler initialization\n");
  }

  int64_t FileSystemWorkerClientServiceHandler::getServiceVersion()
  	  {
	  printf( "Returning version 1\n" );
	  return 1;
  	  }
  /**
   * Cancels a file which has not been completed in the under file system.
   * 
   * @param sessionId the id of the current session
   * 
   * @param tempUfsFileId the worker specific file id of the ufs file
   * 
   * @param options the options for canceling the file
   */
  void FileSystemWorkerClientServiceHandler::cancelUfsFile(const int64_t sessionId, const int64_t tempUfsFileId, const CancelUfsFileTOptions& options) {
    // Your implementation goes here
    printf("cancelUfsFile\n");
  }

  /**
   * Closes a file in the under file system which was previously opened for reading.
   * 
   * 
   * @param sessionId the id of the current session
   * 
   * @param tempUfsFileId the worker specific file id of the ufs file
   * 
   * @param options the options for closing the file
   */
  void FileSystemWorkerClientServiceHandler::closeUfsFile(const int64_t sessionId, const int64_t tempUfsFileId, const CloseUfsFileTOptions& options) {
    // Your implementation goes here
    printf("closeUfsFile for session %lld, file %lld\n", sessionId, tempUfsFileId);
  }

  /**
   * Completes a file in the under file system.
   * 
   * @param sessionId the id of the current session
   * 
   * @param tempUfsFileId the worker specific file id of the ufs file
   * 
   * @param options the options for completing the file
   */
  int64_t FileSystemWorkerClientServiceHandler::completeUfsFile(const int64_t sessionId, const int64_t tempUfsFileId, const CompleteUfsFileTOptions& options) {
    // Your implementation goes here
    printf("completeUfsFile\n");
    return(-1);
  }

  /**
   * Creates a file in the under file system.
   * 
   * @param sessionId the id of the current session
   * 
   * @param ufsPath the path of the file in the ufs
   * 
   * @param options the options for creating the file
   */
  int64_t FileSystemWorkerClientServiceHandler::createUfsFile(const int64_t sessionId, const std::string& ufsPath, const CreateUfsFileTOptions& options) {
    // Your implementation goes here
    printf("createUfsFile\n");
    return(-1);
  }

  /**
   * Opens an existing file in the under file system for reading.
   * 
   * @param sessionId the id of the current session
   * 
   * @param ufsPath the path of the file in the ufs
   * 
   * @param options the options for opening the file
   */
  int64_t FileSystemWorkerClientServiceHandler::openUfsFile(const int64_t sessionId, const std::string& ufsPath, const OpenUfsFileTOptions& options) {
    // Your implementation goes here
    printf("openUfsFile for session %lld, file %s\n", sessionId, ufsPath.c_str());
    return(1);
  }

  /**
   * Local session send heartbeat to local worker to keep its state.
   * 
   * @param sessionId the id of the current session
   * 
   * @param metrics the client metrics. deprecated since 1.3.0 and will be removed in 2.0
   */
  void FileSystemWorkerClientServiceHandler::sessionHeartbeat(const int64_t sessionId, const std::vector<int64_t> & metrics) {
    // Your implementation goes here
    printf("sessionHeartbeat\n");
  }


//int main(int argc, char **argv) {
//  int port = 9090;
//  shared_ptr<FileSystemWorkerClientServiceHandler> handler(new FileSystemWorkerClientServiceHandler());
//  shared_ptr<TProcessor> processor(new FileSystemWorkerClientServiceProcessor(handler));
//  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
//  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
//  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
//
//  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
//  server.serve();
//  return 0;
//}

