/*
 * NettyProcessor.h
 *
 *  Created on: Jul 17, 2017
 *      Author: pls
 */

#ifndef NETTYPROCESSOR_H_
#define NETTYPROCESSOR_H_

#include "NettyProtocol.h"

namespace apache {
namespace thrift {

class NettyProcessor {
public:
	NettyProcessor();
	bool process(boost::shared_ptr<protocol::NettyProtocol> in,
			boost::shared_ptr<protocol::NettyProtocol> out);
};

}
}	// namespace apache::thrift
#endif /* NETTYPROCESSOR_H_ */
