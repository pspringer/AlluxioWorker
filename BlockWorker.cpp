/*
 * BlockWorker.cpp
 *
 *  Created on: Jul 1, 2017
 *      Author: pls
 */

#include <iostream>

#include "thrift/TOutput.h"

#include "BlockWorker.h"

using namespace apache::thrift;

BlockWorker::BlockWorker()
	{
	GlobalOutput.printf( "BlockWorker started\n" );

	}

