#include "DataReader.h"
using namespace std;


//Number of clients:
int NUM_THREADS;

// Argument to a thread:
struct threadStruct {
    long threadID;
    DataReader * d;
    int numObjects;
};


//Condition variables and others:
 int clientCount = 0;

 //Main waits on this:
 pthread_mutex_t count_mutex;
 pthread_cond_t  count_threshold_cv;

 //Threads wait on this:
 pthread_mutex_t countdown_mutex;
 pthread_cond_t  countdown_threshold_cv;



//Convert serialization type to serialization Name:
string getSerializationName(int i) {

	string serializationName;		
	switch(i) {
		case 0: serializationName = "HANDCODED"; break;
		case 1: serializationName = "PROTOBUF"; break;
		case 2: serializationName = "INPLACE"; break;
		case 3: serializationName = "BOOST"; break;
		default: serializationName = "UNKNOWN"; break;
	}

	return serializationName;
}


//Convert objectType to objectName:
string getClassName(int objectType) {

	string className;		
	switch(objectType) {
		case 0: className = "Part"; break;
		case 1: className = "LineItem"; break;
		case 2: className = "Customer"; break;
		default: className = "UNKNOWN"; break;
	}
	return className;
}



void *readContent(void *arguments) {
   struct threadStruct *args = (struct threadStruct *)arguments;
   long tid = args->threadID;
   int numObjects = args->numObjects;
 
   // cout << "Inside readContent():" << endl;  
   // cout << "Current Thread ID, " << args->threadID << endl;

   
	//Increment counter as each thread starts:
	pthread_mutex_lock(&count_mutex);
	clientCount++;
	if(clientCount == NUM_THREADS) {
	 pthread_cond_broadcast(&count_threshold_cv);
	}
	pthread_mutex_unlock(&count_mutex);


	//Wait till main broadcasts that all threads are ready:
	pthread_mutex_lock(&countdown_mutex);
	pthread_cond_wait(&countdown_threshold_cv, &countdown_mutex);
	pthread_mutex_unlock(&countdown_mutex);


	//Read objects:
	args->d->threadSafeReadObjectsSequential(tid, numObjects);



	//Decrement counter as each client connects: Notify when done:
	pthread_mutex_lock(&count_mutex);
	clientCount--;
	if(clientCount == 0) {
	 pthread_cond_broadcast(&count_threshold_cv);
	}
	pthread_mutex_unlock(&count_mutex);

}



int main(int argc, char* argv[]) {

	if (argc != 4) {
		cout << "Requires two arguments: numObjects serializationType objectType" << endl; 
		cout << "Run as: ./a.out 10 0 0" << endl;
		return 0;
	}

	string scaleFactor = "0.1";
	int numObjects = atoi(argv[1]);
	int serializationType = atoi(argv[2]);
	int objectType = atoi(argv[3]);
	NUM_THREADS = 4;

	//Basic Checks:
	if (objectType < 0 || objectType > 2) {
		throw invalid_argument("ObjectType: Invalid input.");
	}

	if (serializationType < 0 || serializationType > 3) {
		throw invalid_argument("SerializationType: Invalid input.");
	}

	


   //Create threads:
   pthread_t threads[NUM_THREADS];
   struct threadStruct currentArgs[NUM_THREADS];
   int rc;

   for(int i = 0; i < NUM_THREADS; i++) {     
      currentArgs[i].threadID = i;
      currentArgs[i].d = new DataReader(scaleFactor, objectType, serializationType);
      currentArgs[i].numObjects = numObjects;

      rc = pthread_create(&threads[i], NULL, readContent, (void *)&currentArgs[i]);
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

  cout << "[ReadTimeCPPMultiThreadToTal]" << "#" << getSerializationName(serializationType) << "#" << (4*numObjects) << "#" << getClassName(objectType) << "#" << totalTime << endl;

  pthread_exit(NULL);
  return 0;

}