# AlluxioWorker
### Framework for porting the Alluxio Worker component to C++
This is not a complete port of the worker component of Alluxio to C++.  But it is a start, and shows how work could proceed.  It uses Thrift to communicate to the other parts of Alluxio, the master and the client, just like the original Alluxio worker code (written in Java) does.  There is enough code in the worker to respond to a "fs cat" command generated from the client.  No UFS directory is supported by this worker, so only a canned file content is returned.
#### Requirements
* Interfaces with version 1.4 of Alluxio master and client (other versions not tested)
* Needs Thrift 0.10 libraries and includes (other versions not tested)
* Needs Boost 1.59 includes (other versions not tested)
#### Motivation
I developed this with an eye toward allowing Alluxio workers to run on an advanced parallel system that did not have standard Linux file support, and instead used RAM to store file contents.  The system had no implementation of Java, only C++, and so there was a need to port the Alluxio worker component to C++, and to leave open the implementation of the underlying file system (UFS).
#### Caveats
1.  Supports only the SIMPLE mode of SASL security, which is the default
#### Usage Example
1.  Download alluxio-1.4.0 (hadoop2.7 build) from alluxio.org
1.  Install per documentation
1.  Move to the Alluxio bin directory.  Start up master and client using the command "./alluxio-start local".  Refer to http://www.alluxio.org/docs/master/en/Getting-Started.html
1.  Create test file *when* using the following command:  echo "Now" > ..underFSStorage/when
1.  Type "./alluxio fs cat /when" to make sure the file is visible to Alluxio
1.  Replace the IP address at about line 111 in AlluxioWorker.cpp with the address of where your Alluxio master will be running
1.  Compile
#### How It Works
