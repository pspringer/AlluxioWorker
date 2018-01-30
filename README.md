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
1.  Does not offer web support
1.  Does not respond to heartbeat messages
#### Usage Example
1.  Download alluxio-1.4.0 (hadoop2.7 build) from alluxio.org
1.  Install per documentation
1.  Move to the Alluxio bin directory.  Start up master and client using the command "./alluxio-start local".  Refer to http://www.alluxio.org/docs/master/en/Getting-Started.html
1.  Create test file *when* using the following command:  echo "Now" > ../underFSStorage/when
1.  Type "./alluxio fs cat /when" to make sure the file is visible to Alluxio.  It should contain exactly 4 bytes, including the newline character at the end.
1.  Replace the IP address at about line 111 in AlluxioWorker.cpp with the address of where your Alluxio master will be running
1.  At about line 131 in AlluxioWorker.cpp, replace the three "pls" characters with the user name you intend to use on your system.  If your user name is longer than 3 characters, adjust the length accordingly.  The length number (currently 20) appears on that line and the line following.  So for example, if your user name is 4 characters, change the 20 in each of those two lines to 21.
1.  Compile
1.  Stop all Alluxio processes by entering "./alluxio-stop local"
1.  Start just the master process by entering "./alluxio-start master"
1.  Wait until the .../logs/master.log file shows a message that includes the words "startServing"
1.  Start up the C++ version of the Alluxio worker
1.  Wait until master.log shows an entry with the words "workerRegister"
1.  Invoke the Alluxio client, within the bin directory, with the command "./alluxio fs cat /when"
1.  Verify that the client types out the contents of the *when* file that you created previously
#### How It Works
The Alluxio master component knows about the file system, such as file names and directories and so on.  The worker in this repository knows what IP and port number (19998) to use to communicate with the master.  It registers itself, and the client, once it starts up, queries the master for this information.  The client uses that information to initiate a connection to the worker if it needs the contents of a file.  The worker acts like a file or data server, sending the file contents directly to the client.

The original worker code used the "Netty" communication package to handle the data server work, and the "Thrift" package to handle all other communication.  Thrift has been ported to C++ already, but Netty has not, so my decision was to use Thrift for the data server code in my version of the worker, even though Thrift is not as efficient as Netty for this.  If Netty is ever ported to C++, it would be desirable to plug that in to this software.
