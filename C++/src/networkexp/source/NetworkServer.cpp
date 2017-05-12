#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <pthread.h>
#include "FileHandler.h"
#include "RootData.h"

#include "Part.h"
#include "Supplier.h"
#include "LineItem.h"
#include "Order.h"
#include "Customer.h"

#include "PartBoost.h"
#include "SupplierBoost.h"
#include "LineItemBoost.h"
#include "OrderBoost.h"
#include "CustomerBoost.h"

#include "PartProto.h"
#include "SupplierProto.h"
#include "LineItemProto.h"
#include "OrderProto.h"
#include "CustomerProto.h"

using namespace std;


//Condition variables and others:
 int clientCount = 0;

 //Main waits on this:
 pthread_mutex_t count_mutex;
 pthread_cond_t  count_threshold_cv;

 //Threads wait on this:
 pthread_mutex_t countdown_mutex;
 pthread_cond_t  countdown_threshold_cv;

//Number of clients:
int NUM_THREADS;

// Argument to a thread:
struct threadStruct {
    long threadID;
    int portNumber;
};


// Create the socket on appropriate port:
ServerSocket server (30000);


void *receiveContent(void *arguments) {
   struct threadStruct *args = (struct threadStruct *)arguments;
   long tid = args->threadID;
 
   cout << "Inside receiveContent():" << endl;  
   cout << "Current Thread ID, " << args->threadID << endl;
   cout << "Port Number: " << args->portNumber << endl;

	try {

		while (true) {
			bool flag = false;
			ServerSocket new_sock;
			server.accept (new_sock);
		
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


          //Write to file: Open the file in binary write mode:
          ofstream outfile("Client" + to_string(tid)  + ".out", ofstream::binary);
          long localPageCount = 0;
          while (true) {
            //Receive a "1":
            new_sock.read(longBuffer, sizeof(long));
            memcpy(&startCode, longBuffer, sizeof(long));  

            //Termination signal:
            if(startCode == 0) break;   

            //Read 20MB:
            new_sock.read(longBuffer, networkPageSize);

            //Write the page in file:
            outfile.write(longBuffer, networkPageSize);

            //Print info:
            cout << "Received page ID = " << localPageCount << " from thread ID = " << tid << endl;
            
            //Increment pagecount:
            localPageCount++;
          }
          outfile.close();


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


int main (int argc, char* argv[]) {

  if(argc != 3) {
    cout << "Arguments are: numClients objectType" << endl;
    cout << "Run as: ./NetworkServer 5 0" << endl;
    return 0;
  }


  NUM_THREADS = atoi(argv[1]);  // Number of clients.
  int objectType = atoi(argv[2]);



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


  float totalTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
  cout << "Total Time: " << totalTime << endl;


  pthread_exit(NULL);
  return 0;
}





