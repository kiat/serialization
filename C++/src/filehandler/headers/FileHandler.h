#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <cassert>
#include <queue>
#include <map>
#include <tr1/unordered_map>
#include <new>
#include <stdexcept>
#include <chrono>
#include "RootData.h"
#include "Object.h"


//BOOST:
#include <boost/archive/text_iarchive.hpp>
using namespace std;


//Network Experiments:
long networkPageSize = 20971520;

/*
 *	Error: http://stackoverflow.com/questions/10632251/undefined-reference-to-template-function
 *	The implementation of a non-specialized template must be visible to a translation unit that uses it.
 *	The compiler must be able to see the implementation in order to generate code for all specializations in your code.
 *	Hence, the implementation is inside the header.
 */


/*
 *  One filehandler object is associated with one particular object type "T",
 *  and one particular fileName. 
 *  primaryIndex = page number; secondaryIndex = offset within page.
 */

	template<class T>
	class FileHandler : public RootData {
		private: 
			string fileName;
			vector<int> primaryIndex;
			vector<int> secondaryIndex;
			int countObject;
			int currentPageNumber;
			int currentOffset;
			int pageSize;
			bool done;
			int serializationType;


			// Sets this to the last IO cost
			double previousIOTime;


			// Set default values to most member variables.
			void setDefaults();


			//Output Stream: For Writing:
			ofstream outStreamRegularFile;


			//Input Stream: For Reading:
			ifstream inStreamRegularFile;



			
			

			//While writing re-use this pagebuffer:
			char * writePageBuffer;


			//Bulk Page Buffer:
			//char * bulkPageBuffer;
			//bool hasBulkPage = false;


			//BOOST Memory Leaks:
			vector<boost::archive::text_iarchive *> archiveVector;
			vector<stringstream *> archiveStream;

			//InPlace memory leaks:
			vector<char *> archivePages;


			//Read bulk pages where each page is allocated in RAM:
			//vector<char *> readPages;
			void readPagesFromFile(int i, int n);


			//Read 1 page by ID into dedicated buffer:
			char * readPageFromFile(int id);
			char * readPageBuffer;
			long nextPageIDToRead = 0;
			 

		public:

			//Default destructor:
			~FileHandler(){
				if(done) {
					delete [] readPageBuffer;
				}
			}

			//Prevents memory leak for BOOST:
			void clearArchives() {
				if (archiveVector.size() != archiveStream.size()) {
					throw invalid_argument("BOOST clear archives: Mismatch in content.");
				}

				for(int i = 0; i < archiveVector.size(); i++) {
					archiveVector.at(i)->delete_created_pointers();
					delete archiveVector.at(i);
					delete archiveStream.at(i);
				}
				archiveVector.clear();
				archiveStream.clear();
			}


			//InPlace memory leaks:
			void clearPages() {
				for (int n = 0; n < archivePages.size(); n++) {
					delete [] archivePages.at(n);
				}
			}




			//Fresh Code: Constructors:
			FileHandler();	//Empty Constructor: Deserialization

			FileHandler(string fileName, int serializationType);	//Serialization




			void printInfo() {
				cout << "SerializationType: " << serializationType << endl;
				cout << "FileName: " << fileName << endl;
				cout << "ObjectCount: " << getObjectCount() << endl;
			}



			// Return the current file size.
			long getFileSize();
			
			//Write objects to file. Can be called multiple times.
			void appendObjectsToFile(vector<T *> objectList);
				
			//When you are done appending objects to the file, call this once to indicate completion.
			void flush();
			

			// Call this only after you have read some objects from the file.
			double getPreviousIOTime() {
				return previousIOTime;
			}


			//Only serialize the required content:
			//Size of serialized data on disk/memory: 
			long getSizeHC (void);
			//Hand Coded C++ serialization:
			char * serializeHC(void);
			//Hand Coded C++ de-serialization:		
			long deserializeHC(char * buffer) ;			


			
			//Basic getters and setters:
			string getFileName() {
				return fileName;
			}
			
			vector<int>& getPrimaryIndex() {
				return primaryIndex;
			}
			
			vector<int>& getSecondaryIndex() {
				return secondaryIndex;
			}
			
			//Return the number of objects in file.
			int getObjectCount() {
				return countObject;
			}


			//Return the size of a page.
			int getPageSize() {
				return pageSize;
			}

			int getSerializationType() {
				return serializationType;
			}

			bool isFlushed() {
				return (done == true);
			}


			//Do NOT return by reference:
			// vector<char *> getReadPages() {
			// 	return readPages;
			// }



			void getObjectsFromFile(int i, int n, vector<T*>& curObjectList);




			//Networking Experiments: Newer APIs:
			long getNetworkPageCount(int numObjects);
			void getNextNetworkPage(char * networkBuffer);
			int getObjectSize(int j);
			int getNextNetworkPage(vector <T *>& objectList, int startIndex, int numObjects, char * networkBuffer);
			int parseNetworkPage(vector <T *>& objectList, char * networkBuffer);






		void retrieve(string storageFileName) {

			// File Size:
			ifstream infile (storageFileName.c_str(), ifstream::binary);	
			infile.seekg (0,infile.end);
			long fileSize = infile.tellg();
			infile.close();

			//Read serialized content:
			char * buffer = new char[fileSize];
			infile.open(storageFileName.c_str(), ifstream::binary);
			infile.read(buffer, fileSize);
			infile.close();

			//Deserialize the buffer:
			deserializeHC(buffer);  

			done = true;


			//DELETE:
			//Open the file for read:
			// if(serializationType == 2) {
			// 	//cout << "In Place File opened retrieve(): " << inPlaceFileName << endl;
			// 	cout << storageFileName << endl;
			// 	cout << pageSize << endl;
			// 	inStreamInPlaceFile.open(inPlaceFileName.c_str(), ifstream::binary);
			// 	readPageBuffer = new char[pageSize + (pageSize/2)]; //Allocating extra space.
			// }
			// else {
				//cout << "Not In Place" << endl;
				if(nextPageIDToRead == 0) {
					//cout << "File Opened 1: " <<  fileName << endl;
					//cout << "Page Size: " << pageSize << endl;
					inStreamRegularFile.open(fileName.c_str(), ifstream::binary);
					readPageBuffer = new char[pageSize];
				}
			// }



			//Free memory: Memory Leak fixed.
			delete[] buffer;
		}


		string store() {
			string storageFileName = "store_" + fileName;
			char * buffer = serializeHC();
			long bufferSize = getSizeHC();

			ofstream outfile (storageFileName.c_str(), ofstream::binary);			
			outfile.write(buffer, bufferSize);
			outfile.close();

			//Free memory: Memory Leak fixed.
			delete[] buffer;

			return storageFileName;
		}

	};


	/*
	 *	 Set defaults. Page Size = 256 KB (Hard Coded)
	 */
	template<class T> void FileHandler<T> :: setDefaults() {
		this->countObject = 0;
		this->currentPageNumber = 0;
		this->currentOffset = 0;
		this->pageSize = 262144; // 256KB = (256 * 1024) bytes.


		//Active Page Allocation: :
		long activePageSize = 2 * pageSize;
		writePageBuffer = new char[activePageSize];
		memset (writePageBuffer, '\0', activePageSize);

		this->previousIOTime = 0.0;
		
		// Default is hand coded serialization.
		serializationType = 0;

		//Once "done" you can't add new objects to the file anymore.
		done = false;		
	}


	//Default Constructor: Use for deserialization:
	template<class T> FileHandler<T> :: FileHandler() {
	}

	/*
	 * Constructor: Specifies serialization type also.
	 */
	template<class T> FileHandler<T> :: FileHandler(string fileName, int serializationType) {

	 	//Set Default Values: 
	 	//Allocates write page buffer:
	 	this->setDefaults();

	 	//Handle InPlace:
	 	if (serializationType == 2) {
			Object::allocator.setUp(writePageBuffer, 2 * pageSize);
	 	}


		this->fileName = fileName;
		this->serializationType = serializationType;

		//Clear file.
		ofstream outfile; outfile.open(fileName.c_str(), ofstream::out | ofstream::trunc); outfile.close();

		//Opening the file and keeping it open:
		outStreamRegularFile.open(fileName.c_str(), ofstream::binary | ofstream::app);
	}


	/*
	 * Write objects to the file. 
	 * Can be called multiple times.
	 */
	template<class T> void FileHandler<T> :: appendObjectsToFile(vector<T *> objectList) {
		if (done) {
			throw invalid_argument("Writing not allowed after committing.");
		}


		previousIOTime = 0.0;

		//Other serialization techniques:	
		int curObjectSize;
		for (int i = 0; i < objectList.size(); i++) {
			
			if (serializationType == 2) {
				currentOffset = objectList.at(i)->serializeInPlace(writePageBuffer);
				curObjectSize = Object::allocator.getBytesUsed() - currentOffset;
			}
			else {
				curObjectSize = 0;
				objectList.at(i)->serialize(writePageBuffer + currentOffset, curObjectSize);
			}


			//Violated page boundary?
			if ((currentOffset + curObjectSize) > pageSize) {
				//Timer start:
				auto beginTime = chrono::steady_clock::now();

				//Write in file:
				outStreamRegularFile.write(writePageBuffer, pageSize);

				//Timer stop:
				previousIOTime += chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();					
						
				//At this point, previous page is written in file.
				//All you need is to write this object in the new current page.

				//Re-write the last object again at correct place:
				memmove(writePageBuffer, writePageBuffer + currentOffset, curObjectSize);

				//Reset InPlace counter appropriately:
				if (serializationType == 2) {
					Object::allocator.setBytesUsed(curObjectSize);
				}

				//Buffer has been written to file at this point.
				//Update indicators. 
				currentPageNumber++;
				currentOffset = 0;

			}
			//Object fits in current page:
			else {
			}


			//Common code to save object in current page:
			primaryIndex.push_back(currentPageNumber);
			secondaryIndex.push_back(currentOffset);			
			currentOffset += curObjectSize;			
			countObject++;	
		}
	}
	

	/*
	 * This is similar to flushing content in file. 
	 * At any point, all pages but last is on file/disk. The last page is maintained in memory.
	 * When you are done appending objects to the file, call this once to indicate completion.
	 * It will write that last page in file and (PREVENT SUBSEQUENT WRITING and ENABLE READING).
	 */		
	template<class T> void FileHandler<T> :: flush() {
		done = true;

		//Open the file for read:
		if(nextPageIDToRead == 0) {
			inStreamRegularFile.open(fileName.c_str(), ifstream::binary);
			readPageBuffer = new char[pageSize];
		}


		previousIOTime = 0.0;


		//Other serialization techniques:
		if (currentOffset > 0) {
		 	//Timer start:
			auto beginTime = chrono::steady_clock::now();

			//FIle IO:
			outStreamRegularFile.write(writePageBuffer, pageSize);
			outStreamRegularFile.close();

			//Timer stop:
			previousIOTime = chrono::duration<float>(chrono::steady_clock::now() - beginTime).count(); 
		}
		else {
			currentPageNumber--;
		}
			
		//Free memory:
		delete [] writePageBuffer;
	}
	




	/*
	 * Read pages from file as buffer.
	 * arg(i, n) means read pages with objects with indices [i, (i+1), (i+2), ..., (i+n-1)].
	 * 
     *  NOTE: This API is supposed to read sequentially only.
	 *
	 */	
	template<class T> char * FileHandler<T> :: readPageFromFile(int id) {
		//If page is already in RAM: Use from RAM:
		if ((nextPageIDToRead -1) == id) {
			return readPageBuffer;
		}

		//Page not in RAM: Disk IO:
		if (nextPageIDToRead != id) {
			inStreamRegularFile.seekg((long)id * (long)pageSize);
			nextPageIDToRead = id;
		}

		//Timer Start:
		auto beginTime = chrono::steady_clock::now();

		inStreamRegularFile.read(readPageBuffer,pageSize);

		//Timer stop:
		previousIOTime += chrono::duration<float>(chrono::steady_clock::now() - beginTime).count(); 
		
		//Update page read indicator appropriately:
		nextPageIDToRead++;


		return readPageBuffer;
	}



	/*
	 * Read pages from file as buffer.
	 * arg(i, n) means read pages with objects with indices [i, (i+1), (i+2), ..., (i+n-1)].
	 */	
/*
	template<class T> void FileHandler<T> :: readPagesFromFile(int i, int n) {
		int startPage = primaryIndex.at(i);
		int endPage  = primaryIndex.at(i+n-1);
		int startIndex = 0, endIndex = 0;

		
		//Clear previous info:
		readPages.clear();
	
		//Read the appropriate pages.		
		inStreamRegularFile.seekg((long)startPage * (long)pageSize);
		for (int j = startPage; j <= endPage; j++) {
			char * curBuffer = new char[pageSize];

			//Timer Start:
			auto beginTime = chrono::steady_clock::now();
			inStreamRegularFile.read (curBuffer,pageSize);
			previousIOTime += chrono::duration<float>(chrono::steady_clock::now() - beginTime).count();
			//Timer Stop:

			readPages.push_back(curBuffer);
		}		
	}

*/


	/*
	 * arg(i, n) means return objects with indices [i, (i+1), (i+2), ..., (i+n-1)].
	 */
	template<class T> void FileHandler<T> :: getObjectsFromFile(int i, int n, vector<T*>& curObjectList) {
		if (!done) {
			throw invalid_argument("Reading not allowed before committing.");
		}

		//IO timer:
	 	previousIOTime = 0.0;


	 	//InPlace:
	 	if (serializationType == 2) {
	 		//Bug fix: Initialize the buffer with the existing page: 
			char * curBuffer = readPageBuffer;

			char *tempInPlaceBuffer = new char [pageSize];
			int tempBytesUsed = 0;
			int tempObjectSize = 0;

			//Iterate over all objects that you aspire to read.
			for (int j = i; j < (i+n); j++) {

				//Go to appropriate page using "primary" index.
				if ((nextPageIDToRead -1) != primaryIndex.at(j)) {
					curBuffer = readPageFromFile(primaryIndex.at(j));
				}


				//Object Size:
				tempObjectSize = 0;
				if ((j+1) < primaryIndex.size()) {
					if(primaryIndex.at(j) == primaryIndex.at(j+1)) {
						tempObjectSize = secondaryIndex.at(j+1) - secondaryIndex.at(j);
					}
					else {
						tempObjectSize = pageSize - secondaryIndex.at(j);
					}
				}
				else {
					tempObjectSize = pageSize - secondaryIndex.at(j);
				}


				//Write into the temp buffer:
				if((tempObjectSize + tempBytesUsed) <= pageSize) {
					memcpy(tempInPlaceBuffer + tempBytesUsed, curBuffer + secondaryIndex.at(j), tempObjectSize);
					curObjectList.push_back((T *)(tempInPlaceBuffer + tempBytesUsed));
					tempBytesUsed += tempObjectSize;
				}
				else {
					archivePages.push_back(tempInPlaceBuffer);
					tempInPlaceBuffer = new char[pageSize];
					tempBytesUsed = 0;
					memcpy(tempInPlaceBuffer + tempBytesUsed, curBuffer + secondaryIndex.at(j), tempObjectSize);
					curObjectList.push_back((T *)(tempInPlaceBuffer + tempBytesUsed));
					tempBytesUsed = tempObjectSize;
				}
			}

			//Flush: The last page used:
			if (tempBytesUsed) {
				archivePages.push_back(tempInPlaceBuffer);
			}

	 	}
	 	// if (serializationType == 2) {
	 	// 	// Record the IO time separately:
			// readPagesFromFile(i, n);

			// //Extract the objects from those pages:
			// int startPage = primaryIndex.at(i);
			

			// //Iterate over all objects that you aspire to read.
			// for (int j = i; j < (i+n); j++) {
			// 	//Go to appropriate page using "primary" index.
			// 	char * curBuffer = readPages.at(primaryIndex.at(j) - startPage);
				
			// 	//Read the object using a secondary index.
			// 	//Deserialize object based on preference:
			// 	curObjectList.push_back((T *)(curBuffer + secondaryIndex.at(j)));
			// }
	 	// }
	 	//Other than InPlace: HandCoded/Proto/Boost:
	 	else {
			T * object = NULL;
			T * temp = new T;

			//Bug fix: Initialize the buffer with the existing page: 
			char * curBuffer = readPageBuffer;

			//Iterate over all objects that you aspire to read.
			for (int j = i; j < (i+n); j++) {

				//Go to appropriate page using "primary" index.
				if ((nextPageIDToRead -1) != primaryIndex.at(j)) {
					curBuffer = readPageFromFile(primaryIndex.at(j));
				}
				
				//Read the object using a secondary index.
				//Deserialize object based on preference:
				switch(serializationType) {
					case 0:	object = new T; object->deserialize(curBuffer + secondaryIndex.at(j)); break;
					case 1: object = new T; object->deserialize(curBuffer + secondaryIndex.at(j)); break;
					
					//BOOST codes:
					case 3: 
					temp->deserializeBoost(curBuffer + secondaryIndex.at(j), object);
					archiveVector.push_back(temp->getArchive());
					archiveStream.push_back(temp->getStream());
					break;

					default: throw invalid_argument("Invalid serialization type.");
				}
				curObjectList.push_back(object);
			}


			//Memory Leak: 
			delete temp;
	 	}
	}






	//Size of serialized data on disk/memory: 
	template<class T>
	long FileHandler<T>::getSizeHC (void) {
		if (!done) {
			throw invalid_argument("Operation not allowed before committing.");
		}

		long size;
		//Integer elements:
		size = 5 * sizeof(int);
		//Double elements:
		size += 1 * sizeof(double);

		//Vectors:
		size += ((primaryIndex.size() + 1) * sizeof(int));
		size += ((secondaryIndex.size() + 1) * sizeof(int));

		//size += sizeof(int);
		//size += ((offsetPointers.size()) * sizeof(long));

		//String sizes: 
		size += 1 * sizeof(int);
		//String Elements:
		size += (fileName.length());		

		//Sanity Check:
		if (size <= 0) {
			cout << "Size of file in getSizeHC(): " << size << endl; 
		}

		return size;
	}		


	//Hand Coded C++ serialization:
	template<class T>
	char * FileHandler<T>::serializeHC(void) {
		//First determine the size of object:
		long sizeofObject = this->getSizeHC();

		//Now, serialize the object.
		char * buffer = new char[sizeofObject];	

		//Sanity Check:
		if (buffer == NULL) {
			cout << "new[] failed in serializeHC()" << endl;
		}

		char * tempBuffer = buffer;
		
		//Copy Integers:
		tempBuffer = this->copyInt(tempBuffer, currentPageNumber);	
		tempBuffer = this->copyInt(tempBuffer, currentOffset);
		tempBuffer = this->copyInt(tempBuffer, pageSize);
		tempBuffer = this->copyInt(tempBuffer, serializationType);
		tempBuffer = this->copyInt(tempBuffer, countObject);

		//Copy Doubles:		
		tempBuffer = this->copyDouble(tempBuffer, previousIOTime);	


		//Copy vectors:
		int primaryIndexSize = primaryIndex.size();
		tempBuffer = this->copyInt(tempBuffer, primaryIndexSize);
		for (int i = 0; i < primaryIndex.size(); i++) {
			tempBuffer = this->copyInt(tempBuffer, primaryIndex.at(i));
		}

		int secondaryIndexSize = secondaryIndex.size();
		tempBuffer = this->copyInt(tempBuffer, secondaryIndexSize);
		for (int i = 0; i < secondaryIndex.size(); i++) {
			tempBuffer = this->copyInt(tempBuffer, secondaryIndex.at(i));
		}		


		// int offsetPointersSize = offsetPointers.size();
		// tempBuffer = this->copyInt(tempBuffer, offsetPointersSize);
		// for (int i = 0; i < offsetPointers.size(); i++) {
		// 	tempBuffer = this->copyLong(tempBuffer, offsetPointers.at(i));
		// }

		//Copy Strings:
		tempBuffer = this->copyString(tempBuffer, fileName);		

		return buffer;
	}



	//Hand Coded C++ deserialization:
	template<class T>
	long FileHandler<T>::deserializeHC(char * buffer) {
		long bytesRead = 0;
		
		//Integers:
		currentPageNumber = parseInt(buffer+bytesRead); bytesRead += sizeof(currentPageNumber);	
		currentOffset = parseInt(buffer+bytesRead); bytesRead += sizeof(currentOffset);	
		pageSize = parseInt(buffer+bytesRead); bytesRead += sizeof(pageSize);	
		serializationType = parseInt(buffer+bytesRead); bytesRead += sizeof(serializationType);	
		countObject = parseInt(buffer+bytesRead); bytesRead += sizeof(countObject);	
		
		//Doubles:
		previousIOTime = parseDouble(buffer+bytesRead); bytesRead += sizeof(double);


		//Vectors:
		int primaryIndexSize = parseInt(buffer+bytesRead); bytesRead += sizeof(primaryIndexSize);
		for (int i = 0; i < primaryIndexSize; i++) {
			primaryIndex.push_back(parseInt(buffer+bytesRead));
			bytesRead += sizeof(int);
		}

		int secondaryIndexSize = parseInt(buffer+bytesRead); bytesRead += sizeof(secondaryIndexSize);
		for (int i = 0; i < secondaryIndexSize; i++) {
			secondaryIndex.push_back(parseInt(buffer+bytesRead));
			bytesRead += sizeof(int);
		}


		// int offsetPointersSize = parseInt(buffer+bytesRead); bytesRead += sizeof(offsetPointersSize);
		// for (int i = 0; i < offsetPointersSize; i++) {
		// 	offsetPointers.push_back(parseLong(buffer+bytesRead));
		// 	bytesRead += sizeof(long);
		// }


		//Parse Strings:
		parseString(buffer+bytesRead, fileName); bytesRead += (sizeof(int) + fileName.length());


		//Sanity Check:
		if (bytesRead <= 0) {
			cout << "Size of bytesRead in deserializeHC(): " << bytesRead << endl; 
		}

		return  bytesRead;
	}	







	//Number of network pages to be sent from disk:
	template<class T> 
	long FileHandler<T>::getNetworkPageCount(int numObjects) {
		long contentSize = 0;
		//PROTOBUF, BOOST, HANDCODED, INPLACE:
		if (primaryIndex.size() > numObjects) {
			contentSize = (1 + primaryIndex.at(numObjects - 1)) * (long)pageSize;
		}

		//Return pagecount:
		if (contentSize % networkPageSize == 0) {
			return (contentSize/networkPageSize);
		}
		else {
			return (1 + (contentSize/networkPageSize));
		}
	}


	//Simply reads the next 20MB from file:
	template<class T>
	void FileHandler<T>::getNextNetworkPage(char * networkBuffer) {
		//PROTOBUF, BOOST, HANDCODED, INPLACE:
		if (primaryIndex.size() > 0) {
			inStreamRegularFile.read(networkBuffer, networkPageSize);
		}
	}


	template<class T>
	int FileHandler<T>::getObjectSize(int j) {
		int tempObjectSize = 0;
		//Next Object is there?
		if ((j+1) < primaryIndex.size()) {
			if(primaryIndex.at(j) == primaryIndex.at(j+1)) {
				tempObjectSize = secondaryIndex.at(j+1) - secondaryIndex.at(j);
			}
			else {
				tempObjectSize = pageSize - secondaryIndex.at(j);
			}
		}
		else {
			tempObjectSize = pageSize - secondaryIndex.at(j);
		}

		return tempObjectSize;
	}


	template<class T>
	int FileHandler<T>::getNextNetworkPage(vector <T *>& objectList, int startIndex, int numObjects, char * networkBuffer) {

		//Other serialization techniques:	
		int curObjectSize;
		int currentOffset;

		// cout << "startIndex: " << startIndex << endl;
		// cout << "numObjects: " << numObjects << endl;

		// Empty Buffer;
		currentOffset = 0;
		char * tempBuffer = networkBuffer;


		//Need to copy number of objects in page at beginning:
		currentOffset += sizeof(int);

		bool pageFinished = false;
		int previousOffset = 0;

		int i = 0;
		for (i = startIndex; i < numObjects; i++) {

			//InPlace:
			if (serializationType == 2) {
				curObjectSize = getObjectSize(i);
				
				//Copy object size:
				memcpy(tempBuffer + currentOffset, &curObjectSize, sizeof(int));
				currentOffset += sizeof(int);

				//Copy object:
				memcpy(tempBuffer + currentOffset, (char *)objectList.at(i), curObjectSize);
				currentOffset += curObjectSize;
			}
			else {
				//Save current offset: Need to write object size:
				previousOffset = currentOffset;

				// Keep Space for Object Size:
				currentOffset += sizeof(int);

				curObjectSize = 0;
				objectList.at(i)->serialize(tempBuffer + currentOffset, curObjectSize);
				currentOffset += curObjectSize;

				// Copy object size at appropriate place:
				memcpy(tempBuffer + previousOffset, &curObjectSize, sizeof(int));
			}

			// cout << i << ", " << curObjectSize << endl;

			//Violated page boundary?
			if (currentOffset > networkPageSize) {
				// cout << "Page Break!" << endl;
				break;
			}
			//Else, object fits in current page:
		}	


		// Write total number of objects in page:
		int totalObjectInPage = i - startIndex;
		memcpy(networkBuffer, &totalObjectInPage, sizeof(int));


		//This is the index to be read next:
		return i;
	}



	template<class T>
	int FileHandler<T>::parseNetworkPage(vector <T *>& curObjectList, char * networkBuffer) {

		char *tempBuffer = networkBuffer;
		int bytesRead = 0;


		// Total number of objects in this page:
		int numObjects = parseInt(tempBuffer+bytesRead); bytesRead += sizeof(int);
		int currentObjectSize = 0;


		//DELETE:
		cout << "parseNetworkPage:: " << curObjectList.size() << endl;


		T * object = NULL;
		T * temp;

		//Only for BOOST:
		if(serializationType == 3) {
			temp = new T;
		}


		for (int j = 0; j < numObjects; j++) {		
			// Parse object size first:
			currentObjectSize = parseInt(tempBuffer+bytesRead); bytesRead += sizeof(int);


			cout << j << ", " << currentObjectSize << endl;

			//Deserialize object based on preference:
			switch(serializationType) {
				//HandCoded:
				case 0:	object = new T; object->deserialize(tempBuffer+bytesRead); 
					curObjectList.push_back(object);
					break;
				
				case 1: object = new T; object->deserialize(tempBuffer+bytesRead); 
					curObjectList.push_back(object);
					break;
				
				//InPlace:	
				case 2: 
					curObjectList.push_back((T *)(tempBuffer+bytesRead));
					break;

				//BOOST codes:
				case 3: 
				temp->deserializeBoost(tempBuffer+bytesRead, object);
				archiveVector.push_back(temp->getArchive());
				archiveStream.push_back(temp->getStream());
				curObjectList.push_back(object);
				break;

				default: throw invalid_argument("Invalid serialization type.");
			}


			// Skip "currentObjectSize" bytes:
			bytesRead += currentObjectSize;		
		}

		//Memory Leak: 
		if(serializationType == 3) {
				delete temp;
		}

		cout << "Total Objects Parsed: " << curObjectList.size() << endl;
		cout << curObjectList.at(0)->toString() << endl;

	}






#endif