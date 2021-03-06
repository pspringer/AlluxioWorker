// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "BlockMasterClientService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class BlockMasterClientServiceHandler : virtual public BlockMasterClientServiceIf {
 public:
  BlockMasterClientServiceHandler() {
    // Your initialization goes here
  }

  /**
   * Returns the block information for the given block id.
   * 
   * @param blockId the id of the block
   */
  void getBlockInfo( ::BlockInfo& _return, const int64_t blockId) {
    // Your implementation goes here
    printf("getBlockInfo\n");
  }

  /**
   * Returns the capacity (in bytes).
   */
  int64_t getCapacityBytes() {
    // Your implementation goes here
    printf("getCapacityBytes\n");
  }

  /**
   * Returns the used storage (in bytes).
   */
  int64_t getUsedBytes() {
    // Your implementation goes here
    printf("getUsedBytes\n");
  }

  /**
   * Returns a list of workers information.
   */
  void getWorkerInfoList(std::vector<WorkerInfo> & _return) {
    // Your implementation goes here
    printf("getWorkerInfoList\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<BlockMasterClientServiceHandler> handler(new BlockMasterClientServiceHandler());
  shared_ptr<TProcessor> processor(new BlockMasterClientServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

