#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm> 
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "SparseVectors.h"
using namespace std;

#define networkPageSize  20971520L

	/*
	 * count: Number of elements in the sparse vector.
	 * dimensions: Maximum number of elements possible.
	 * count <= dimensions
	 */

	void SparseVectors::generateSparseVector(int count, int dimensions, int serializationType) {
		if (count > dimensions) {
			throw invalid_argument("Sparse Vectors: Invalid inputs for generation.");
		}
		else if (count < 0) {
			throw invalid_argument("Sparse Vectors: Number of elements is negative.");
		}
		else if(dimensions < 0) {
			throw invalid_argument("Sparse Vectors: Maximum dimension is negative.");
		}


		//Initialize:
		this->count = count;
		this->dimensions = dimensions;
		this->serializationType = serializationType;


		// Initialize seed with time: So that we get different vectors:
		unsigned int time_ui = (unsigned int)( time(NULL) );
		srand( time_ui );
		cout << "Time UI: " << time_ui << endl;
		sleep(1);

		// Positions:
		unordered_set<int> randomIndices;
		vector<int> sortedIndices; 
		
		for (int i = 0; i < count; i++) {
			int randomPosition = rand()%dimensions;
			
			while(randomIndices.find(randomPosition) != randomIndices.end()){
				randomPosition = rand()%dimensions;
			}
			randomIndices.insert(randomPosition);
			sortedIndices.push_back(randomPosition);
		}
		

		// Create and setup INPLACE buffer:
		if (serializationType == 2) {
			long totalSize = 16 * count;
			inPlaceBuffer = new char[totalSize];
			Object::allocator.setUp(inPlaceBuffer, totalSize);

		}


		//Sort the vector:
		sort(sortedIndices.begin(), sortedIndices.end());
		for(int i = 0; i < sortedIndices.size(); i++) {
			double randomValue = (double)rand() / RAND_MAX;

			Element * temp;
			ElementIP * tempIP;
			ElementProto * tempProto;
			ElementBoost * tempBoost;

			switch(serializationType) {
				case 0:
					temp = new Element(sortedIndices.at(i), randomValue);
					elementHandCodedObjects.push_back(temp);
					break;
				case 1:
					tempProto = new ElementProto(sortedIndices.at(i), randomValue);
					elementProtoObjects.push_back(tempProto);
					break;
				case 2:
					tempIP = new ElementIP(sortedIndices.at(i), randomValue);
					elementInPlaceObjects.push_back(tempIP);
					break;

				case 3:
					tempBoost = new ElementBoost(sortedIndices.at(i), randomValue);
					elementBoostObjects.push_back(tempBoost);
					break;
				default: break;
			}
		}
	}


	
	void SparseVectors::aggergateSparseVector(SparseVectors * vectorOne, SparseVectors * vectorTwo, SparseVectors * vectorThree) {
		cout << "aggergateSparseVector:" << endl;
		// cout << vectorOne->getSize() << endl;
		// cout << vectorTwo->getSize() << endl;
		// cout << vectorThree->getSize() << endl;

		// cout << vectorOne->getSerializationType() << endl;
		// cout << vectorTwo->getSerializationType() << endl;
		// cout << vectorThree->getSerializationType() << endl;


		this->setUp(vectorOne->getSerializationType());

		// Create and setup INPLACE buffer:
		if (this->getSerializationType() == 2) {
			long totalSize = 16 * vectorOne->getSize() * 4;

			cout << "totalSize: " << totalSize << endl;

			inPlaceBuffer = new char[totalSize];
			Object::allocator.setUp(inPlaceBuffer, totalSize);
		}

		int indexOne = 0, indexTwo = 0, indexThree = 0;
		int minPosition = -1;
		double value = 0.0;
		while((indexOne < vectorOne->getSize()) || (indexTwo < vectorTwo->getSize()) || (indexThree < vectorThree->getSize())) {

			minPosition = -1;

			if (indexOne < vectorOne->getSize()) {
				minPosition = vectorOne->getPosition(indexOne);
			}

			if (indexTwo < vectorTwo->getSize()) {
				if (minPosition > 0) {
					minPosition = min(minPosition, vectorTwo->getPosition(indexTwo));
				}
				else {
					minPosition = vectorTwo->getPosition(indexTwo);
				}
			}

			if (indexThree < vectorThree->getSize()) {
				if (minPosition > 0) {
					minPosition = min(minPosition, vectorThree->getPosition(indexThree));
				}
				else {
					minPosition = vectorThree->getPosition(indexThree);
				}			
			}

			
			// cout << "minPosition: " << minPosition << endl;

			value = 0.0;
			if (indexOne < vectorOne->getSize()) {
				if (minPosition == vectorOne->getPosition(indexOne)) {
					value += vectorOne->getValue(indexOne);
					indexOne++;
				}
			}

			if (indexTwo < vectorTwo->getSize()) {
				if (minPosition == vectorTwo->getPosition(indexTwo)) {
					value += vectorTwo->getValue(indexTwo);
					indexTwo++;
				}
			}

			if (indexThree < vectorThree->getSize()) {
				if (minPosition == vectorThree->getPosition(indexThree)) {
					value += vectorThree->getValue(indexThree);
					indexThree++;
				}
			}

			//Add new element:
			// cout << "CurSize: " << this->getSize() << endl;
			this->addElement(minPosition, value);

		}
	}


	int SparseVectors::getNextNetworkPage(int startIndex, char * networkBuffer) {

		int numObjects = this->getSize();

		//Other serialization techniques:	
		int curObjectSize;
		int currentOffset;

		cout << "startIndex: " << startIndex << endl;
		cout << "numObjects: " << numObjects << endl;


		// Empty Buffer;
		currentOffset = 0;
		char * tempBuffer = networkBuffer;

		//Need to copy number of objects in page at beginning:
		currentOffset += sizeof(int);



		int i = 0;
		

		//Hand Coded:
		if (serializationType == 0) {
			for (i = startIndex; i < numObjects; i++) {
				curObjectSize = 0;
				elementHandCodedObjects.at(i)->serialize(tempBuffer + currentOffset, curObjectSize); 
			
				currentOffset += curObjectSize;
				//Violated page boundary?
				if (currentOffset > networkPageSize) {
					cout << "Page Break!" << endl;
					break;
				}
			}
		}


		if (serializationType == 1) {
			for (i = startIndex; i < numObjects; i++) {
				curObjectSize = 0;
				elementProtoObjects.at(i)->serialize(tempBuffer + currentOffset, curObjectSize); 
			
				currentOffset += curObjectSize;
				//Violated page boundary?
				if (currentOffset > networkPageSize) {
					cout << "Page Break!" << endl;
					break;
				}
			}
		}




		if (serializationType == 2) {
			for (i = startIndex; i < numObjects; i++) {
				curObjectSize = 16;
				memcpy(tempBuffer + currentOffset, (char *)elementInPlaceObjects.at(i), curObjectSize);
			

				currentOffset += curObjectSize;
				//Violated page boundary?
				if (currentOffset > networkPageSize) {
					cout << "Page Break!" << endl;
					break;
				}
			}
		}

		//BOOST:
		if (serializationType == 3) {
					
			stringstream ss;
			boost::archive::binary_oarchive oa(ss, boost::archive::no_header);

			int curPosition;
			double curValue;			

			for (i = startIndex; i < numObjects; i++) {
				curObjectSize = 12;
				curPosition = elementBoostObjects.at(i)->getPosition();
				oa << curPosition;

				curValue = elementBoostObjects.at(i)->getValue();
				oa << curValue;

				currentOffset += curObjectSize;
				//Violated page boundary?
				if (currentOffset > networkPageSize) {
					cout << "Page Break!" << endl;
					break;
				}
			}

			//Copy on buffer:
			memcpy(networkBuffer + sizeof(int), ss.str().c_str(), ss.str().size());
		}


		// Write total number of objects in page:
		int totalObjectInPage = i - startIndex;
		memcpy(networkBuffer, &totalObjectInPage, sizeof(int));


		cout << "totalObjectInPage:: " << totalObjectInPage << endl;

		//This is the index to be read next:
		return i;
	}


	void SparseVectors::parseNetworkPage(char * networkBuffer) {
		cout << "parseNetworkPage:: " << endl;			

		char *tempBuffer = networkBuffer;
		int bytesRead = 0;


		// Total number of objects in this page:
		int numObjects = *(int *)(tempBuffer+bytesRead); 
		bytesRead += sizeof(int);
		
		
		// cout << "numObjects:: " << numObjects << endl;
		// cout << "serializationType:: " << serializationType << endl;

		// Hand Coded:
		if (serializationType == 0) {
			Element * objectElement = NULL;

			for (int j = 0; j < numObjects; j++) {	
				objectElement = new Element(); 
				objectElement->deserialize(tempBuffer+bytesRead); 
				elementHandCodedObjects.push_back(objectElement);

				// Object Size = 12 bytes (fixed)
				bytesRead += 12;
			}
		}

		//Proto Buf:
		if (serializationType == 1) {
			ElementProto * objectElementProto = NULL;

			int currentObjectSize = 0;
			for (int j = 0; j < numObjects; j++) {
				objectElementProto = new ElementProto(); 
				currentObjectSize = objectElementProto->deserialize(tempBuffer+bytesRead); 
				elementProtoObjects.push_back(objectElementProto);

				// ObjectSize = Variable:
				bytesRead += currentObjectSize;
			}
		}

		//InPlace:
		if (serializationType == 2) {
			long totalSize = 16 * numObjects;
			inPlaceBuffer = new char[totalSize];
			Object::allocator.setUp(inPlaceBuffer, totalSize);
			archiveInPlaceBuffer.push_back(inPlaceBuffer);

			//Copy only the relevant info:
			memcpy(inPlaceBuffer, tempBuffer + bytesRead, totalSize);

			bytesRead = 0;
			for (int j = 0; j < numObjects; j++) {	
				elementInPlaceObjects.push_back((ElementIP *)(inPlaceBuffer+bytesRead));
				
				// Object Size = 16 bytes (fixed)
				bytesRead += 16;
			}
		}


		// Boost:
		if (serializationType == 3) {
			//String Stream:
			stringstream rs;
			rs.write(tempBuffer + bytesRead, networkPageSize - sizeof(int));
			
			//Create archive on heap: Keep stream alive:
			boost::archive::binary_iarchive ia (rs, boost::archive::no_header);


			ElementBoost * objectElementBoost = NULL;

			int curPosition;
			double curValue;
			for (int j = 0; j < numObjects; j++) {	
				ia >> curPosition;
				ia >> curValue;
				objectElementBoost = new ElementBoost(curPosition, curValue);
				elementBoostObjects.push_back(objectElementBoost);
			}
		}
	}



// int main() {
	

// 	SparseVectors * l = new SparseVectors();
// 	l->generateSparseVector(100, 1000, 2);
// 	//l->printInfo();
// 	cout << l->getSize() << endl;

// 	SparseVectors * m = new SparseVectors();
// 	m->generateSparseVector(100, 1000, 2);
// 	//m->printInfo();
// 	cout << m->getSize() << endl;

// 	SparseVectors * n = new SparseVectors();
// 	n->generateSparseVector(100, 1000, 2);
// 	// n->printInfo();
// 	cout << n->getSize() << endl;


// 	SparseVectors * p = new SparseVectors();
// 	p->aggergateSparseVector(l, m, n);
// 	//p->printInfo();
// 	cout << p->getSize() << endl;

// 	// s->generateSparseVector(10, 100, 1);
// 	// s->generateSparseVector(10, 100, 2);
// 	// s->generateSparseVector(10, 100, 3);

// 	return 0;
// }