#ifndef DATAREADER_H
#define DATAREADER_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <chrono>
#include <sstream>
#include <vector>
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

							

class DataReader : public RootData {


	public:
		//Default destructor:
		~DataReader() {
			//Clean up everything after done:
			//Delete File Handlers:
			//Part:
			if (objectType == 0) {
				switch(curSerializationType) {
					case 0: delete partHandCodedFileHandler;
							break;
					case 1: delete partProtoFileHandler;
							break;
					case 2: delete partInPlaceFileHandler;	
							break;
					case 3: delete partBoostFileHandler;	
							break;
				}
			}
			//LineItem:
			else if (objectType == 1) {
				switch(curSerializationType) {
					case 0: delete lineItemHandCodedFileHandler;
							break;
					case 1: delete lineItemProtoFileHandler;
							break;
					case 2: delete lineItemInPlaceFileHandler;	
							break;
					case 3: delete lineItemBoostFileHandler;	
							break;
				}
			}
			//Customer:
			else if(objectType == 2) {
				switch(curSerializationType) {
					case 0: delete customerHandCodedFileHandler;
							break;
					case 1: delete customerProtoFileHandler;
							break;
					case 2: delete customerInPlaceFileHandler;	
							break;
					case 3: delete customerBoostFileHandler;	
							break;
				}

			}
		}


		//General constructor used all along:
		DataReader(string scaleFactor, int objectType, int serializationType) {
			//We have datasets with 0.1, 1, ...
			this->scaleFactor = scaleFactor;

			this->curSerializationType = serializationType;

			this->objectType = objectType;

			//Prepare data in appropriate files:
			this->retrieve();
		}


		//Read objects sequentially: threaded
		void threadSafeReadObjectsSequential(int threadID, int numObjects) {
			//Part:
			if (objectType == 0) {
				switch(curSerializationType) {
					case 0: threadSafeReadObjectsSequential(partHandCodedFileHandler, threadID, numObjects, "Part");	break;
					case 1: threadSafeReadObjectsSequential(partProtoFileHandler, threadID, numObjects, "Part");	break;
					case 2: threadSafeReadObjectsSequential(partInPlaceFileHandler, threadID, numObjects, "Part");	break;
					case 3: threadSafeReadObjectsSequential(partBoostFileHandler, threadID, numObjects, "Part");	break;
				}
			}
			//LineItem:
			else if (objectType == 1) {
				switch(curSerializationType) {
					case 0: threadSafeReadObjectsSequential(lineItemHandCodedFileHandler, threadID, numObjects, "LineItem");	break;
					case 1: threadSafeReadObjectsSequential(lineItemProtoFileHandler, threadID, numObjects, "LineItem");	break;
					case 2: threadSafeReadObjectsSequential(lineItemInPlaceFileHandler, threadID, numObjects, "LineItem");	break;
					case 3: threadSafeReadObjectsSequential(lineItemBoostFileHandler, threadID, numObjects, "LineItem");	break;
				}

			}
			//Customer:
			else if(objectType == 2) {
				switch(curSerializationType) {
					case 0: threadSafeReadObjectsSequential(customerHandCodedFileHandler, threadID, numObjects, "Customer");	break;
					case 1: threadSafeReadObjectsSequential(customerProtoFileHandler, threadID, numObjects, "Customer");	break;
					case 2: threadSafeReadObjectsSequential(customerInPlaceFileHandler, threadID, numObjects, "Customer");	break;
					case 3: threadSafeReadObjectsSequential(customerBoostFileHandler, threadID, numObjects, "Customer");	break;
				}
			}
		}

		//Read objects sequentially:
		void readObjectsSequential(int numObjects) {
			//Part:
			if (objectType == 0) {
				switch(curSerializationType) {
					case 0: readObjectsSequential(partHandCodedFileHandler, numObjects, "Part");	break;
					case 1: readObjectsSequential(partProtoFileHandler, numObjects, "Part");	break;
					case 2: readObjectsSequential(partInPlaceFileHandler, numObjects, "Part");	break;
					case 3: readObjectsSequential(partBoostFileHandler, numObjects, "Part");	break;
				}
			}
			//LineItem:
			else if (objectType == 1) {
				switch(curSerializationType) {
					case 0: readObjectsSequential(lineItemHandCodedFileHandler, numObjects, "LineItem");	break;
					case 1: readObjectsSequential(lineItemProtoFileHandler, numObjects, "LineItem");	break;
					case 2: readObjectsSequential(lineItemInPlaceFileHandler, numObjects, "LineItem");	break;
					case 3: readObjectsSequential(lineItemBoostFileHandler, numObjects, "LineItem");	break;
				}

			}
			//Customer:
			else if(objectType == 2) {
				switch(curSerializationType) {
					case 0: readObjectsSequential(customerHandCodedFileHandler, numObjects, "Customer");	break;
					case 1: readObjectsSequential(customerProtoFileHandler, numObjects, "Customer");	break;
					case 2: readObjectsSequential(customerInPlaceFileHandler, numObjects, "Customer");	break;
					case 3: readObjectsSequential(customerBoostFileHandler, numObjects, "Customer");	break;
				}
			}
		}


		//Read objects in random order:
		void readObjectsRandom(int numObjects) {
			//Part:
			if (objectType == 0) {
				switch(curSerializationType) {
					case 0: readObjectsRandom(partHandCodedFileHandler, numObjects, "Part");	break;
					case 1: readObjectsRandom(partProtoFileHandler, numObjects, "Part");	break;
					case 2: readObjectsRandom(partInPlaceFileHandler, numObjects, "Part");	break;
					case 3: readObjectsRandom(partBoostFileHandler, numObjects, "Part");	break;
				}
			}
			//LineItem:
			else if (objectType == 1) {
				switch(curSerializationType) {
					case 0: readObjectsRandom(lineItemHandCodedFileHandler, numObjects, "LineItem");	break;
					case 1: readObjectsRandom(lineItemProtoFileHandler, numObjects, "LineItem");	break;
					case 2: readObjectsRandom(lineItemInPlaceFileHandler, numObjects, "LineItem");	break;
					case 3: readObjectsRandom(lineItemBoostFileHandler, numObjects, "LineItem");	break;
				}

			}
			//Customer:
			else if(objectType == 2) {
				switch(curSerializationType) {
					case 0: readObjectsRandom(customerHandCodedFileHandler, numObjects, "Customer");	break;
					case 1: readObjectsRandom(customerProtoFileHandler, numObjects, "Customer");	break;
					case 2: readObjectsRandom(customerInPlaceFileHandler, numObjects, "Customer");	break;
					case 3: readObjectsRandom(customerBoostFileHandler, numObjects, "Customer");	break;
				}
			}
		}


		//Retrieve saved data:
		void retrieve() {
			//Part:
			if (objectType == 0) {
				switch(curSerializationType) {
					case 0: partHandCodedFileHandler = new FileHandler<Part>();	
							partHandCodedFileHandler->retrieve(string("store_partObjects" + to_string(curSerializationType) + ".txt"));	
							break;
					case 1: partProtoFileHandler  = new FileHandler<PartProto>();	
							partProtoFileHandler->retrieve(string("store_partObjects" + to_string(curSerializationType) + ".txt"));
							break;
					case 2: partInPlaceFileHandler  = new FileHandler<PartIP>();	
							partInPlaceFileHandler->retrieve(string("store_partObjects" + to_string(curSerializationType) + ".txt"));
							break;
					case 3: partBoostFileHandler = new FileHandler<PartBoost>();	
							partBoostFileHandler->retrieve(string("store_partObjects" + to_string(curSerializationType) + ".txt"));
							break;
				}
			}
			//LineItem:
			else if (objectType == 1) {
				switch(curSerializationType) {
					case 0: lineItemHandCodedFileHandler = new FileHandler<LineItem>();	
							lineItemHandCodedFileHandler->retrieve(string("store_lineItemObjects" + to_string(curSerializationType) + ".txt"));	
							break;
					case 1: lineItemProtoFileHandler  = new FileHandler<LineItemProto>();	
							lineItemProtoFileHandler->retrieve(string("store_lineItemObjects" + to_string(curSerializationType) + ".txt"));
							break;
					case 2: lineItemInPlaceFileHandler  = new FileHandler<LineItemIP>();	
							lineItemInPlaceFileHandler->retrieve(string("store_lineItemObjects" + to_string(curSerializationType) + ".txt"));
							break;
					case 3: lineItemBoostFileHandler = new FileHandler<LineItemBoost>();	
							lineItemBoostFileHandler->retrieve(string("store_lineItemObjects" + to_string(curSerializationType) + ".txt"));
							break;
				}
			}
			//Customer:
			else if(objectType == 2) {
				switch(curSerializationType) {
					case 0: customerHandCodedFileHandler = new FileHandler<Customer>();	
							customerHandCodedFileHandler->retrieve(string("store_customerObjects" + to_string(curSerializationType) + ".txt"));	
							break;
					case 1: customerProtoFileHandler  = new FileHandler<CustomerProto>();	
							customerProtoFileHandler->retrieve(string("store_customerObjects" + to_string(curSerializationType) + ".txt"));
							break;
					case 2: customerInPlaceFileHandler  = new FileHandler<CustomerIP>();	
							customerInPlaceFileHandler->retrieve(string("store_customerObjects" + to_string(curSerializationType) + ".txt"));
							break;
					case 3: customerBoostFileHandler = new FileHandler<CustomerBoost>();	
							customerBoostFileHandler->retrieve(string("store_customerObjects" + to_string(curSerializationType) + ".txt"));
							break;
				}

			}
		}


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


	private:

		//FileHandlers for Part:
		FileHandler<Part> * partHandCodedFileHandler;
		FileHandler<PartIP> * partInPlaceFileHandler;
		FileHandler<PartProto> * partProtoFileHandler;
		FileHandler<PartBoost> * partBoostFileHandler;

		//FileHandlers for Customer:
		FileHandler<Customer> * customerHandCodedFileHandler;
		FileHandler<CustomerIP> * customerInPlaceFileHandler;
		FileHandler<CustomerProto> * customerProtoFileHandler;
		FileHandler<CustomerBoost> * customerBoostFileHandler;

		//FileHandlers for LineItem:
		FileHandler<LineItem> * lineItemHandCodedFileHandler;
		FileHandler<LineItemIP> * lineItemInPlaceFileHandler;
		FileHandler<LineItemProto> * lineItemProtoFileHandler;
		FileHandler<LineItemBoost> * lineItemBoostFileHandler;

		//Specifications:
		string scaleFactor;


		//Print Logs:
		bool printLogs = true;

		int curSerializationType;
		int objectType;



		template<class T> 
		void readObjectsSequential(FileHandler<T> * fileHandler, int numObjects, string className) {
			cout << fixed;

			// Make sure different runs of the same experiment reads different objects.
			int time_ui = int( time(NULL) );
			srand( time_ui );


			// Auxiliary variables:
			float totalTime, totalIOTime;
			int startIndex;



			int i = curSerializationType;

			//Serialization Name:
			string serializationName = getSerializationName(i);		
			
			/*
			 * Read  objects:
			 */
			totalTime = totalIOTime = 0.0;
			vector<T *> readObjects;
			
			// Experiments:
			auto beginTime = chrono::steady_clock::now();
			startIndex = rand()% (fileHandler->getObjectCount() - numObjects); 
			fileHandler->getObjectsFromFile(startIndex, numObjects, readObjects);
		
			//Time calculations:
			totalTime += chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
			totalIOTime += fileHandler->getPreviousIOTime();


			//Sanity Check:
			if (readObjects.size() != numObjects) {
				throw invalid_argument("Sequential: Mismatch in number of objects.");
			}


			//Memory Leak: 
			//HandCoded/Proto/Boost:
			if (curSerializationType != 2) {
				if (curSerializationType != 3) {
					for (int n = 0; n < readObjects.size(); n++) {
						delete readObjects.at(n);
					}					
				}
				else {
					fileHandler->clearArchives();
				}
			}
			//InPlace:
			else {
				//cout << readObjects.at(0)->toString() << endl;
				fileHandler->clearPages();
				// vector<char *> readPages = fileHandler->getReadPages();
				// for (int n = 0; n < readPages.size(); n++) {
				// 	delete [] readPages.at(n);
				// }
			}

			//Print Console:
			if(totalIOTime > totalTime) {
				throw invalid_argument("Sequential: Timer has bugs.");
			}
			cout << "[ReadTimeCPP]" << "#" << serializationName << "#" << "true" << "#" << className << "#" << totalIOTime << "#" << totalTime << endl;
		
		}


		template<class T> 
		void threadSafeReadObjectsSequential(FileHandler<T> * fileHandler, int threadID, int numObjects, string className) {

			int startIndex = threadID * (fileHandler->getObjectCount()/4);
			cout << "StartIndex:: " << startIndex << endl;

			int i = curSerializationType;

			//Serialization Name:
			string serializationName = getSerializationName(i);		
			
			/*
			 * Read  objects:
			 */
			vector<T *> readObjects;
			
			// Experiments:
			fileHandler->getObjectsFromFile(startIndex, numObjects, readObjects);

			//Sanity Check:
			if (readObjects.size() != numObjects) {
				throw invalid_argument("Sequential: Mismatch in number of objects.");
			}
		}


		template<class T> 
		void readObjectsRandom(FileHandler<T> * fileHandler, int numObjects, string className) {
			cout << fixed;

			// Make sure different runs of the same experiment reads different objects.
			int time_ui = int( time(NULL) );
			srand( time_ui );

			// Auxiliary variables:
			float totalTime, totalIOTime;
			int startIndex;


			int k = curSerializationType;

			//Serialization Name:
			string serializationName = getSerializationName(k);	

			/*
			 * Read objects:
			 */
			totalTime = totalIOTime = 0.0;
			vector<T *> readObjectsList;

			//InPlace:
			vector<char *> readPages;

			//Timer start:
			auto beginTime = chrono::steady_clock::now();

			// Read 1 random object at a time:
			for (int i = 0; i < numObjects; i++) {

				startIndex = rand()% (fileHandler->getObjectCount()); 
				vector<T *> readObjects;
				fileHandler->getObjectsFromFile(startIndex, 1, readObjects);
			
				//Add time reading disks:
				totalIOTime += fileHandler->getPreviousIOTime();

				//Populate List:
				readObjectsList.push_back(readObjects.at(0));

				//DELETE:
				//Timer stop:
				//totalTime += chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();

				//Populate pages: InPlace Only:
				//if (curSerializationType == 2) {
					//readPages.push_back(fileHandler->getReadPages().at(0));
					//delete [] fileHandler->getReadPages().at(0);
				//}
			}


			//Timer stop:
			totalTime += chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();


			//Sanity Check:
			if (readObjectsList.size() != numObjects) {
				throw invalid_argument("Random: Mismatch in number of objects.");
			}

			
			//Memory Leak: 
			//HandCoded/Proto/Boost:
			if (curSerializationType != 2) {
				if (curSerializationType != 3) {
					for (int n = 0; n < readObjectsList.size(); n++) {
							delete readObjectsList.at(n);
					}	
				}
				//BOOST:
				else {
					fileHandler->clearArchives();
				}						
			}
			//InPlace:
			else {
				//cout << readObjectsList.at(0)->toString() << endl;
				fileHandler->clearPages();
				// for (int n = 0; n < readPages.size(); n++) {
				// 	delete [] readPages.at(n);
				// }
			}


			//Print Console:
			if(totalIOTime > totalTime) {
				throw invalid_argument("Random: Timer has bugs.");
			}
			cout << "[ReadTimeCPP]" << "#" << serializationName << "#" << "false" << "#" << className << "#" << totalIOTime << "#" << totalTime << endl;

		}




};





















#endif