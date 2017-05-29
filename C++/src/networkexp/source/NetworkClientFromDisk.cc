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

//Newer APIs:


//Number of pages to read from disk:
long getNetworkPageCount(int objectType, int curSerializationType, int numObjects) {
    //Part:
    if (objectType == 0) {
      switch(curSerializationType) {
        case 0: return partHandCodedFileHandler->getNetworkPageCount(numObjects);
            break;
        case 1: return partProtoFileHandler->getNetworkPageCount(numObjects);
            break;
        case 2: return partInPlaceFileHandler->getNetworkPageCount(numObjects);  
            break;
        case 3: return partBoostFileHandler->getNetworkPageCount(numObjects);  
            break;
      }
    }
    //LineItem:
    else if (objectType == 1) {
      switch(curSerializationType) {
        case 0: return lineItemHandCodedFileHandler->getNetworkPageCount(numObjects);
            break;
        case 1: return lineItemProtoFileHandler->getNetworkPageCount(numObjects);
            break;
        case 2: return lineItemInPlaceFileHandler->getNetworkPageCount(numObjects);  
            break;
        case 3: return lineItemBoostFileHandler->getNetworkPageCount(numObjects);  
            break;
      }
    }
    //Customer:
    else if(objectType == 2) {
      switch(curSerializationType) {
        case 0: return customerHandCodedFileHandler->getNetworkPageCount(numObjects);
            break;
        case 1: return customerProtoFileHandler->getNetworkPageCount(numObjects);
            break;
        case 2: return customerInPlaceFileHandler->getNetworkPageCount(numObjects);  
            break;
        case 3: return customerBoostFileHandler->getNetworkPageCount(numObjects);  
            break;
      }
    }
}

void getNextNetworkPage(int objectType, int curSerializationType, char * networkBuffer) {
    //Part:
    if (objectType == 0) {
      switch(curSerializationType) {
        case 0: partHandCodedFileHandler->getNextNetworkPage(networkBuffer);
            break;
        case 1: partProtoFileHandler->getNextNetworkPage(networkBuffer);
            break;
        case 2: partInPlaceFileHandler->getNextNetworkPage(networkBuffer);  
            break;
        case 3: partBoostFileHandler->getNextNetworkPage(networkBuffer);  
            break;
      }
    }
    //LineItem:
    else if (objectType == 1) {
      switch(curSerializationType) {
        case 0: lineItemHandCodedFileHandler->getNextNetworkPage(networkBuffer);
            break;
        case 1: lineItemProtoFileHandler->getNextNetworkPage(networkBuffer);
            break;
        case 2: lineItemInPlaceFileHandler->getNextNetworkPage(networkBuffer);  
            break;
        case 3: lineItemBoostFileHandler->getNextNetworkPage(networkBuffer);  
            break;
      }
    }
    //Customer:
    else if(objectType == 2) {
      switch(curSerializationType) {
        case 0: customerHandCodedFileHandler->getNextNetworkPage(networkBuffer);
            break;
        case 1: customerProtoFileHandler->getNextNetworkPage(networkBuffer);
            break;
        case 2: customerInPlaceFileHandler->getNextNetworkPage(networkBuffer);  
            break;
        case 3: customerBoostFileHandler->getNextNetworkPage(networkBuffer);  
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

  bool flag = false;
  while(true) {
    try {

        ClientSocket client_socket (hostName, portNumber);  //"localhost"

        try {
          //Create a 20MB page:
          char * longBuffer;
          longBuffer = new char[networkPageSize];  

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

          long totalPageCount = 0;
          totalPageCount = getNetworkPageCount(objectType, serializationType, numObjects);
          cout << "Pages to be sent: " << totalPageCount << endl;

          long localPageCount = 0;
          while (localPageCount < totalPageCount) {
            //Send a "1":
            startCode = 1;
            memcpy(longBuffer, &startCode, sizeof(startCode));
            client_socket.write(longBuffer, sizeof(startCode));


            //Send 20MB page:
            getNextNetworkPage(objectType, serializationType, longBuffer);
            client_socket.write(longBuffer, networkPageSize);

            //Print info:
            cout << "Sending page ID = " << localPageCount << endl;

            //Increment pagecount:
            localPageCount++;
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
