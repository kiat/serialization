#include "ServerSocket.h"
#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "SparseVectors.h"

using namespace std;


#define networkPageSize  20971520L

// Number of clients.
int NUM_THREADS = 2;  
int serializationType;

// Argument to a thread:
struct threadStruct {
    long threadID;
    int portNumber;
};

// 0 = Root: only receive, no sending; 
// 1 = Forwarding Server: receive and send; 
// 2 = Leaf: only send;
int nodeType;

//Condition variables and others:
 int clientCount = 0;

 //Main waits on this:
 pthread_mutex_t count_mutex;
 pthread_cond_t  count_threshold_cv;

 //Threads wait on this:
 pthread_mutex_t countdown_mutex;
 pthread_cond_t  countdown_threshold_cv;


 vector<SparseVectors *> allVectors;

  // Create the socket on appropriate port:
  ServerSocket * server;



void *receiveContent(void *arguments) {
   struct threadStruct *args = (struct threadStruct *)arguments;
   long tid = args->threadID;

   //Set up serialization type:
   allVectors.at(tid)->setUp(serializationType);
 
   cout << "Inside receiveContent():" << endl;  
   cout << "Current Thread ID, " << args->threadID << endl;
   cout << "Port Number: " << args->portNumber << endl;

  try {

    while (true) {
      bool flag = false;
      ServerSocket new_sock;
      server->accept (new_sock);
    
      try {
        while (true) {
          //Create a 20MB page:
          char * longBuffer;
          longBuffer = new char[networkPageSize];  

          //Receive Start Code:
          long startCode = 0;
          new_sock.read(longBuffer, sizeof(long));
          memcpy(&startCode, longBuffer, sizeof(long));     

          if(startCode != 1) {
            throw invalid_argument("ERROR: Start code mismatch!");
          }
            
          //Increment counter as each client connects:
          pthread_mutex_lock(&count_mutex);
          clientCount++;
          if(clientCount == NUM_THREADS) {
             pthread_cond_broadcast(&count_threshold_cv);
          }
          pthread_mutex_unlock(&count_mutex);


          //Wait till main broadcasts that all clients are up::
          pthread_mutex_lock(&countdown_mutex);
          pthread_cond_wait(&countdown_threshold_cv, &countdown_mutex);
          pthread_mutex_unlock(&countdown_mutex);


          //Send "GO":
          startCode = 0;
          memcpy(longBuffer, &startCode, sizeof(startCode));
          new_sock.write(longBuffer, sizeof(startCode));


          long localPageCount = 0;
          while (true) {
            //Receive a "1":
            new_sock.read(longBuffer, sizeof(long));
            memcpy(&startCode, longBuffer, sizeof(long));  

            //Termination signal:
            if(startCode == 0) break;   

            //Read 20MB:
            new_sock.read(longBuffer, networkPageSize);
          
            //Parse the page:
            allVectors.at(tid)->parseNetworkPage(longBuffer);


            //Print info:
            cout << "Received page ID = " << localPageCount << " from thread ID = " << tid << endl;
            
            //Increment pagecount:
            localPageCount++;
          }


          //Decrement counter as each client connects: Notify when done:
          pthread_mutex_lock(&count_mutex);
          clientCount--;
          if(clientCount == 0) {
             pthread_cond_broadcast(&count_threshold_cv);
          }
          pthread_mutex_unlock(&count_mutex);

          //Free the buffer:
          delete[] longBuffer;

          //Make flag true when all done:
          flag = true;
          break;
        }
      }
      catch (SocketException&) {}
      

      if(flag == true) break;
    }
  }
  catch ( SocketException& e ) {
    cout << "Exception was caught during receiveContent():" << e.description() << "\nExiting.\n";
  }

  pthread_exit(NULL);
}



void receiveData(SparseVectors * sv) {
   server = new ServerSocket(30000);

   //Create threads:
   pthread_t threads[NUM_THREADS];
   struct threadStruct currentArgs[NUM_THREADS];
   int rc;

   for(int i = 0; i < NUM_THREADS; i++) {     
      currentArgs[i].threadID = i;
      currentArgs[i].portNumber = 30000;

      rc = pthread_create(&threads[i], NULL, receiveContent, (void *)&currentArgs[i]);
      if (rc){
         cout << "Error: Unable to create thread," << rc << endl;
         exit(-1);
      }
   }

  //Wait on main thread till all the clients are up:
  pthread_mutex_lock(&count_mutex);
  pthread_cond_wait(&count_threshold_cv, &count_mutex);
  pthread_mutex_unlock(&count_mutex);


  //Start all the waiting threads:
  pthread_mutex_lock(&countdown_mutex);
  pthread_cond_broadcast(&countdown_threshold_cv);
  pthread_mutex_unlock(&countdown_mutex);



  cout << "Timer Started: " << endl;
  auto beginTime = chrono::steady_clock::now();


  //Wait on main thread till computation finishes:
  pthread_mutex_lock(&count_mutex);
  pthread_cond_wait(&count_threshold_cv, &count_mutex);
  pthread_mutex_unlock(&count_mutex);


  //Aggregate the vectors:
  SparseVectors * finalResult = new SparseVectors();
  finalResult->aggergateSparseVector(allVectors.at(0), allVectors.at(1), sv);



  ofstream outfile("RootAggregatedData.out", ofstream::binary);
  int numObjects = finalResult->getSize();
  int startIndex = 0;
  char * longBuffer = new char[networkPageSize];

  while (true) {
    //Send 20MB page:
    startIndex = finalResult->getNextNetworkPage(startIndex, longBuffer);            

    outfile.write(longBuffer, networkPageSize);

    // break Out: Done:
    if (startIndex >= numObjects) {
      break;
    }
  }
  outfile.close();


  float totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
  cout << "Total Time: " << totalTime << endl;

}


void receiveAndSendData(string hostName, int portNumber, SparseVectors * sv) {

   server = new ServerSocket(30000);

   //Create threads:
   pthread_t threads[NUM_THREADS];
   struct threadStruct currentArgs[NUM_THREADS];
   int rc;

   for(int i = 0; i < NUM_THREADS; i++) {     
      currentArgs[i].threadID = i;
      currentArgs[i].portNumber = 30000;

      rc = pthread_create(&threads[i], NULL, receiveContent, (void *)&currentArgs[i]);
      if (rc){
         cout << "Error: Unable to create thread," << rc << endl;
         exit(-1);
      }
   }

  //Wait on main thread till all the clients are up:
  pthread_mutex_lock(&count_mutex);
  pthread_cond_wait(&count_threshold_cv, &count_mutex);
  pthread_mutex_unlock(&count_mutex);


  //At this point, the threads supposed to receive are ready:
  //But we have to get the verification from the root node:

  cout << "Forwarding Server: All leaf nodes are connected!" << endl;


  bool flag = false;
  while(true) {
    try {

        ClientSocket client_socket (hostName, portNumber);  //"localhost"

        try {
          //Create a 20MB page:
          char * longBuffer;
          longBuffer = new char[2 * networkPageSize]; 

          //Send start code first: Goal is to let them know "Client" is up:
          long startCode = 1;
          memcpy(longBuffer, &startCode, sizeof(startCode));
          client_socket.write(longBuffer, sizeof(startCode));

          //Receive "GO":
          client_socket.read(longBuffer, sizeof(long));
          memcpy(&startCode, longBuffer, sizeof(long));         
          if (startCode != 0) {
            throw invalid_argument("ERROR: \"GO\" signal mismatch!");
          }

          cout << "Forwarding Server: Root node up!" << endl;

          
          //At this point we know, root is up:
          //Start all the waiting threads:
          pthread_mutex_lock(&countdown_mutex);
          pthread_cond_broadcast(&countdown_threshold_cv);
          pthread_mutex_unlock(&countdown_mutex);


          //Wait on main thread till computation finishes:
          pthread_mutex_lock(&count_mutex);
          pthread_cond_wait(&count_threshold_cv, &count_mutex);
          pthread_mutex_unlock(&count_mutex);


          //Aggregate the vectors:
          SparseVectors * finalResult = new SparseVectors();
          finalResult->aggergateSparseVector(allVectors.at(0), allVectors.at(1), sv);

          //Number of objects in final result:
          int numObjects = finalResult->getSize();





          cout << "Pages to be sent: Unknown"  << endl;

          int startIndex = 0;
          int localPageCount = 0;
          while (true) {
            //Send a "1":
            startCode = 1;
            memcpy(longBuffer, &startCode, sizeof(startCode));
            client_socket.write(longBuffer, sizeof(startCode));


            //Send 20MB page:
            startIndex = finalResult->getNextNetworkPage(startIndex, longBuffer);            

            client_socket.write(longBuffer, networkPageSize);

            //Print info:
            cout << "Sending page ID = " << localPageCount << endl;
            localPageCount++;

            // break Out: Done:
            if (startIndex >= numObjects) {
              break;
            }
          }

          //Termination Signal: Send a "0":
          startCode = 0;
          memcpy(longBuffer, &startCode, sizeof(startCode));
          client_socket.write(longBuffer, sizeof(startCode));          


          //Free the buffer:
          delete[] longBuffer;

          //Everything done:
          flag = true;

          if(!flag) {
            cout << "Error: Should abort and investigate." << endl;
          }

        }
        catch ( SocketException& ) {}

    }
    catch ( SocketException& e ) {
      cout << "Exception was caught:" << e.description() << "\n";
      sleep(10);
    }

    if(flag) {
      break;
    }

  }

}


void sendData(string hostName, int portNumber, SparseVectors * sv) {
  //Total number of objects:
  int numObjects = sv->getSize();
  cout << "Total objects at Leaf: " << numObjects << endl;

  bool flag = false;
  while(true) {
    try {

        ClientSocket client_socket (hostName, portNumber);  //"localhost"

        try {
          //Create a 20MB page:
          char * longBuffer;
          longBuffer = new char[2 * networkPageSize]; 

          //Send start code first: Goal is to let them know "Client" is up:
          long startCode = 1;
          memcpy(longBuffer, &startCode, sizeof(startCode));
          client_socket.write(longBuffer, sizeof(startCode));


          //Receive "GO":
          client_socket.read(longBuffer, sizeof(long));
          memcpy(&startCode, longBuffer, sizeof(long));         
          if (startCode != 0) {
            throw invalid_argument("ERROR: \"GO\" signal mismatch!");
          }

          cout << "Pages to be sent: Unknown"  << endl;

          int startIndex = 0;
          int localPageCount = 0;
          while (true) {
            //Send a "1":
            startCode = 1;
            memcpy(longBuffer, &startCode, sizeof(startCode));
            client_socket.write(longBuffer, sizeof(startCode));


            //Send 20MB page:
            startIndex = sv->getNextNetworkPage(startIndex, longBuffer);
            
            //DELETE:
            //parseNetworkPage(objectType, serializationType, longBuffer);

            client_socket.write(longBuffer, networkPageSize);

            //Print info:
            cout << "Sending page ID = " << localPageCount << endl;
            localPageCount++;

            // break Out: Done:
            if (startIndex >= numObjects) {
              break;
            }
          }


          //Termination Signal: Send a "0":
          startCode = 0;
          memcpy(longBuffer, &startCode, sizeof(startCode));
          client_socket.write(longBuffer, sizeof(startCode));          


          //Free the buffer:
          delete[] longBuffer;

          //Everything done:
          flag = true;

          if(!flag) {
            cout << "Error: Should abort and investigate." << endl;
          }

        }
        catch ( SocketException& ) {}

    }
    catch ( SocketException& e ) {
      cout << "Exception was caught:" << e.description() << "\n";
      sleep(10);
    }

    if(flag) {
      break;
    }

  }
}






int main(int argc, char* argv[]) {
  // nodeType::
  // 0 = Root; 1 = Forwarding Server; 2 = Leaf Nodes:
  if(argc < 2) {
    cout << "nodeType not provided!" << endl;
    return 0;
  }

  //NodeType is a global variable:
  nodeType = atoi(argv[1]);
  int numElements;
  int maxElements;
  string hostName;
  int portNumber;


  switch(nodeType) {
  	case 0: 
	  	if (argc != 5) {
		    cout << "Arguments are: nodeType numElements maxElements serializationType" << endl;
		    cout << "Run as: ./NetworkAggregator 0 10 1000 0" << endl;
		    return 0;
	  	}
	  	break;
  	case 1:
  	case 2:
	  	if (argc != 6) {
		    cout << "Arguments are: nodeType numElements maxElements serializationType hostName" << endl;
		    cout << "Run as: ./NetworkAggregator 10 1000 0 localhost" << endl;
		    return 0;
	  	}
	  	break;
  	default: break;
  }


  //Variables that control data generation:
  numElements = atoi(argv[2]);
  maxElements = atoi(argv[3]);
  serializationType = atoi(argv[4]);

  if(nodeType > 0) {
	  hostName = string(argv[5]);
	  portNumber = 30000;   
  }


  //Generate data on this machine:
  SparseVectors * sv = new SparseVectors();
  sv->generateSparseVector(numElements, maxElements, serializationType);
  cout << "Element Vector ready in RAM!" << endl;
  // sv->printInfo();

  switch(nodeType) {
  	case 0: 
      cout << "ROOT Server!" << endl;
      allVectors.push_back(new SparseVectors());
      allVectors.push_back(new SparseVectors());
  		receiveData(sv);
  		break;
  	case 1: 
      cout << "Forwarding Server!" << endl;
      allVectors.push_back(new SparseVectors());
      allVectors.push_back(new SparseVectors());
      receiveAndSendData(hostName, portNumber, sv);
  		break;
  	case 2:
  		sendData(hostName, portNumber, sv);
  		break;
  }


 


  

	return 0;

}