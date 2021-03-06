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

#include "BlockWorkerClientService.h"
#include "BlockWorkerClientServiceHandler.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

BlockWorkerClientServiceHandler::BlockWorkerClientServiceHandler()
  {
    // Your initialization goes here
  }

int64_t BlockWorkerClientServiceHandler::getServiceVersion()
	  {
	  printf( "Returning version 1\n" );
	  return 1;
	  }

  /**
   * Accesses a block given the block id.
   * 
   * @param blockId the id of the block being accessed
   */
  void BlockWorkerClientServiceHandler::accessBlock(const int64_t blockId) {
    // Your implementation goes here
    printf("accessBlock\n");
  }

  /**
   * Used to cache a block into Alluxio space, worker will move the temporary block file from session
   * folder to data folder, and update the space usage information related. then update the block
   * information to master.
   * 
   * @param sessionId the id of the current session
   * 
   * @param blockId the id of the block being accessed
   */
  void BlockWorkerClientServiceHandler::cacheBlock(const int64_t sessionId, const int64_t blockId) {
    // Your implementation goes here
    printf("cacheBlock\n");
  }

  /**
   * Used to cancel a block which is being written. worker will delete the temporary block file and
   * the location and space information related, then reclaim space allocated to the block.
   * 
   * @param sessionId the id of the current session
   * 
   * @param blockId the id of the block being accessed
   */
  void BlockWorkerClientServiceHandler::cancelBlock(const int64_t sessionId, const int64_t blockId) {
    // Your implementation goes here
    printf("cancelBlock\n");
  }

  /**
   * Locks the file in Alluxio's space while the session is reading it. If lock succeeds, the path of
   * the block's file along with the internal lock id of locked block will be returned. If the block's file
   * is not found, FileDoesNotExistException will be thrown.
   * 
   * @param blockId the id of the block being accessed
   * 
   * @param sessionId the id of the current session
   */
  void BlockWorkerClientServiceHandler::lockBlock(LockBlockResult& _return, const int64_t blockId, const int64_t sessionId) {
    // Your implementation goes here
    printf("lockBlock\n");
  }

  /**
   * Used to promote block on under storage layer to top storage layer when there are more than one
   * storage layers in Alluxio's space. return true if the block is successfully promoted, false
   * otherwise.
   * 
   * @param blockId the id of the block being accessed
   */
  bool BlockWorkerClientServiceHandler::promoteBlock(const int64_t blockId) {
    // Your implementation goes here
    printf("promoteBlock\n");
    return false;
  }

  /**
   * Used to remove a block from an Alluxio worker.
   * 
   * 
   * @param blockId the id of the block being removed
   */
  void BlockWorkerClientServiceHandler::removeBlock(const int64_t blockId) {
    // Your implementation goes here
    printf("removeBlock\n");
  }

  /**
   * Used to allocate location and space for a new coming block, worker will choose the appropriate
   * storage directory which fits the initial block size by some allocation strategy, and the
   * temporary file path of the block file will be returned. if there is no enough space on Alluxio
   * storage OutOfSpaceException will be thrown, if the file is already being written by the session,
   * FileAlreadyExistsException will be thrown.
   * 
   * @param sessionId the id of the current session
   * 
   * @param blockId the id of the block being accessed
   * 
   * @param initialBytes initial number of bytes requested
   * 
   * @param writeTier the target tier to write to
   */
  void BlockWorkerClientServiceHandler::requestBlockLocation(std::string& _return, const int64_t sessionId, const int64_t blockId, const int64_t initialBytes, const int32_t writeTier) {
    // Your implementation goes here
    printf("requestBlockLocation\n");
  }

  /**
   * Used to request space for some block file. return true if the worker successfully allocates
   * space for the block on block's location, false if there is no enough space, if there is no
   * information of the block on worker, FileDoesNotExistException will be thrown.
   * 
   * @param sessionId the id of the current session
   * 
   * @param blockId the id of the block being accessed
   * 
   * @param requestBytes the number of bytes requested
   */
  bool BlockWorkerClientServiceHandler::requestSpace(const int64_t sessionId, const int64_t blockId, const int64_t requestBytes) {
    // Your implementation goes here
    printf("requestSpace\n");
    return false;
  }

  /**
   * Local session send heartbeat to local worker to keep its temporary folder.
   * 
   * @param sessionId the id of the current session
   * 
   * @param metrics deprecated since 1.3.0 and will be removed in 2.0
   */
  void BlockWorkerClientServiceHandler::sessionHeartbeat(const int64_t sessionId, const std::vector<int64_t> & metrics) {
    // Your implementation goes here
    printf("sessionHeartbeat\n");
  }

  /**
   * Used to unlock a block after the block is accessed, if the block is to be removed, delete the
   * block file. return true if successfully unlock the block, return false if the block is not
   * found or failed to delete the block.
   * 
   * @param blockId the id of the block being accessed
   * 
   * @param sessionId the id of the current session
   */
  bool BlockWorkerClientServiceHandler::unlockBlock(const int64_t blockId, const int64_t sessionId) {
    // Your implementation goes here
    printf("unlockBlock\n");
    return false;
  }


//int main(int argc, char **argv) {
//  int port = 9090;
//  shared_ptr<BlockWorkerClientServiceHandler> handler(new BlockWorkerClientServiceHandler());
//  shared_ptr<TProcessor> processor(new BlockWorkerClientServiceProcessor(handler));
//  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
//  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
//  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
//
//  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
//  server.serve();
//  return 0;
//}

