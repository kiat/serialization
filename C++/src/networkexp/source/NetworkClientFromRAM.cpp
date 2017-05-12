#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <cstdlib>
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


    //Make the filehandlers 
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


    // Part object pointers:
    vector<Part*> partHandCodedObjects;
    vector<PartIP*> partInPlaceObjects;
    vector<PartProto*> partProtoObjects;
    vector<PartBoost*> partBoostObjects;

    // LineItem object pointers:
    vector<LineItem*> lineItemHandCodedObjects;
    vector<LineItemIP*> lineItemInPlaceObjects;
    vector<LineItemProto*> lineItemProtoObjects;
    vector<LineItemBoost*> lineItemBoostObjects;

    // Customer object pointers:
    vector<Customer*> customerHandCodedObjects;
    vector<CustomerIP*> customerInPlaceObjects;
    vector<CustomerProto*> customerProtoObjects;
    vector<CustomerBoost*> customerBoostObjects;




    //DELETE:
    // Part object pointers parsed:
    vector<Part*> partHandCodedObjectsParsed;
    vector<PartIP*> partInPlaceObjectsParsed;
    vector<PartProto*> partProtoObjectsParsed;
    vector<PartBoost*> partBoostObjectsParsed;

    // LineItem object pointers parsed:
    vector<LineItem*> lineItemHandCodedObjectsParsed;
    vector<LineItemIP*> lineItemInPlaceObjectsParsed;
    vector<LineItemProto*> lineItemProtoObjectsParsed;
    vector<LineItemBoost*> lineItemBoostObjectsParsed;

    // Customer object pointers parsed:
    vector<Customer*> customerHandCodedObjectsParsed;
    vector<CustomerIP*> customerInPlaceObjectsParsed;
    vector<CustomerProto*> customerProtoObjectsParsed;
    vector<CustomerBoost*> customerBoostObjectsParsed;    




/*
 * Prepare index for appropriate (objectType, serializationType):
 */
void prepareIndex(int objectType, int curSerializationType) {
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
      else {
        throw invalid_argument("PrepareIndex: Invalid Object Type");
      }
}

/*
 * Clean up  file handlers:
 */
void cleanUp(int objectType, int curSerializationType) {
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
    else {
        throw invalid_argument("PrepareIndex: Invalid Object Type");
    }
}


//Read Objects onto RAM:
void getObjectsFromFile(int objectType, int curSerializationType, int numObjects) {
    //Part:
    if (objectType == 0) {
      switch(curSerializationType) {
        case 0: partHandCodedFileHandler->getObjectsFromFile(0, numObjects, partHandCodedObjects);
            break;
        case 1: partProtoFileHandler->getObjectsFromFile(0, numObjects, partProtoObjects);
            break;
        case 2: partInPlaceFileHandler->getObjectsFromFile(0, numObjects, partInPlaceObjects);  
            break;
        case 3: partBoostFileHandler->getObjectsFromFile(0, numObjects, partBoostObjects);  
            break;
      }
    }
    //LineItem:
    else if (objectType == 1) {
      switch(curSerializationType) {
        case 0: lineItemHandCodedFileHandler->getObjectsFromFile(0, numObjects, lineItemHandCodedObjects);
            break;
        case 1: lineItemProtoFileHandler->getObjectsFromFile(0, numObjects, lineItemProtoObjects);
            break;
        case 2: lineItemInPlaceFileHandler->getObjectsFromFile(0, numObjects, lineItemInPlaceObjects);  
            break;
        case 3: lineItemBoostFileHandler->getObjectsFromFile(0, numObjects, lineItemBoostObjects);  
            break;
      }
    }
    //Customer:
    else if(objectType == 2) {
      switch(curSerializationType) {
        case 0: customerHandCodedFileHandler->getObjectsFromFile(0, numObjects, customerHandCodedObjects);
            break;
        case 1: customerProtoFileHandler->getObjectsFromFile(0, numObjects, customerProtoObjects);
            break;
        case 2: customerInPlaceFileHandler->getObjectsFromFile(0, numObjects, customerInPlaceObjects);  
            break;
        case 3: customerBoostFileHandler->getObjectsFromFile(0, numObjects, customerBoostObjects);  
            break;
      }

    }
    else {
        throw invalid_argument("PrepareIndex: Invalid Object Type");
    }
}


int getNextNetworkPage(int objectType, int curSerializationType, int startIndex, int numObjects, char * networkBuffer) {
    //Part:
    if (objectType == 0) {
      switch(curSerializationType) {
        case 0: startIndex =partHandCodedFileHandler->getNextNetworkPage(partHandCodedObjects, startIndex, numObjects, networkBuffer);
            break;
        case 1: startIndex =partProtoFileHandler->getNextNetworkPage(partProtoObjects, startIndex, numObjects, networkBuffer);
            break;
        case 2: startIndex =partInPlaceFileHandler->getNextNetworkPage(partInPlaceObjects, startIndex, numObjects, networkBuffer);  
            break;
        case 3: startIndex =partBoostFileHandler->getNextNetworkPage(partBoostObjects, startIndex, numObjects, networkBuffer);  
            break;
      }
    }
    //LineItem:
    else if (objectType == 1) {
      switch(curSerializationType) {
        case 0: startIndex =lineItemHandCodedFileHandler->getNextNetworkPage(lineItemHandCodedObjects, startIndex, numObjects, networkBuffer);
            break;
        case 1: startIndex =lineItemProtoFileHandler->getNextNetworkPage(lineItemProtoObjects, startIndex, numObjects, networkBuffer);
            break;
        case 2: startIndex =lineItemInPlaceFileHandler->getNextNetworkPage(lineItemInPlaceObjects, startIndex, numObjects, networkBuffer);  
            break;
        case 3: startIndex =lineItemBoostFileHandler->getNextNetworkPage(lineItemBoostObjects, startIndex, numObjects, networkBuffer);  
            break;
      }
    }
    //Customer:
    else if(objectType == 2) {
      switch(curSerializationType) {
        case 0: startIndex =customerHandCodedFileHandler->getNextNetworkPage(customerHandCodedObjects, startIndex, numObjects, networkBuffer);
            break;
        case 1: startIndex =customerProtoFileHandler->getNextNetworkPage(customerProtoObjects, startIndex, numObjects, networkBuffer);
            break;
        case 2: startIndex =customerInPlaceFileHandler->getNextNetworkPage(customerInPlaceObjects, startIndex, numObjects, networkBuffer);  
            break;
        case 3: startIndex =customerBoostFileHandler->getNextNetworkPage(customerBoostObjects, startIndex, numObjects, networkBuffer);  
            break;
      }
    }

    return startIndex;
}



void parseNetworkPage(int objectType, int curSerializationType, char * networkBuffer) {
      //Part:
    if (objectType == 0) {
      switch(curSerializationType) {
        case 0: partHandCodedFileHandler->parseNetworkPage(partHandCodedObjectsParsed, networkBuffer);
            break;
        case 1: partProtoFileHandler->parseNetworkPage(partProtoObjectsParsed, networkBuffer);
            break;
        case 2: partInPlaceFileHandler->parseNetworkPage(partInPlaceObjectsParsed, networkBuffer);  
            break;
        case 3: partBoostFileHandler->parseNetworkPage(partBoostObjectsParsed, networkBuffer);  
            break;
      }
    }
    //LineItem:
    else if (objectType == 1) {
      switch(curSerializationType) {
        case 0: lineItemHandCodedFileHandler->parseNetworkPage(lineItemHandCodedObjectsParsed, networkBuffer);
            break;
        case 1: lineItemProtoFileHandler->parseNetworkPage(lineItemProtoObjectsParsed, networkBuffer);
            break;
        case 2: lineItemInPlaceFileHandler->parseNetworkPage(lineItemInPlaceObjectsParsed, networkBuffer);  
            break;
        case 3: lineItemBoostFileHandler->parseNetworkPage(lineItemBoostObjectsParsed, networkBuffer);  
            break;
      }
    }
    //Customer:
    else if(objectType == 2) {
      switch(curSerializationType) {
        case 0: customerHandCodedFileHandler->parseNetworkPage(customerHandCodedObjectsParsed, networkBuffer);
            break;
        case 1: customerProtoFileHandler->parseNetworkPage(customerProtoObjectsParsed, networkBuffer);
            break;
        case 2: customerInPlaceFileHandler->parseNetworkPage(customerInPlaceObjectsParsed, networkBuffer);  
            break;
        case 3: customerBoostFileHandler->parseNetworkPage(customerBoostObjectsParsed, networkBuffer);  
            break;
      }
    }
}


int main (int argc, char* argv[]) {

  // objectType: [0-2]  serializationType: [0-3]
  if(argc != 5) {
    cout << argc << endl;
    cout << "Arguments are: hostName serializationType objectType numObjects " << endl;
    cout << "Run as: ./NetworkClient localhost 0 0 10000 " << endl;
    return 0;
  }

  string hostName = string(argv[1]);
  int serializationType = atoi(argv[2]);
  int objectType = atoi(argv[3]);
  int numObjects = atoi(argv[4]);
  int portNumber = 30000;

  cout << "ObjectType = " << objectType << endl;
  cout << "SerializationType = " << serializationType << endl;

  //Prepare Index first:
  prepareIndex(objectType, serializationType);
  getObjectsFromFile(objectType, serializationType, numObjects);
  cout << "Client: Objects are ready in RAM." << endl;



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
            startIndex = getNextNetworkPage(objectType, serializationType, startIndex, numObjects, longBuffer);
            
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

  //Clean up:
  cleanUp(objectType, serializationType);
  return 0;
}
