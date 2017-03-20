#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "Part.h"

using namespace std;

//CREATE TABLE PART  ( P_PARTKEY     INTEGER NOT NULL,
//        P_NAME        VARCHAR(55) NOT NULL,
//        P_MFGR        CHAR(25) NOT NULL,
//        P_BRAND       CHAR(10) NOT NULL,
//        P_TYPE        VARCHAR(25) NOT NULL,
//        P_SIZE        INTEGER NOT NULL,
//        P_CONTAINER   CHAR(10) NOT NULL,
//        P_RETAILPRICE DECIMAL(15,2) NOT NULL,
//        P_COMMENT     VARCHAR(23) NOT NULL );

	//Copy constructor:
	Part::Part(const Part &obj) {
		throw invalid_argument("Part: Not copy constructible.");
	}


	//Default constructor:
	Part::Part() {		
	}
	
	//Constructor with arguments:
	Part::Part(int partKey, string name, string mfgr, string brand, string type, int size, string container, double retailPrice, string comment) {
		this->partKey = partKey;
		this->name = name;
		this->mfgr = mfgr;
		this->brand = brand;
		this->type = type;
		this->size = size;
		this->container = container;
		this->retailPrice = retailPrice;
		this->comment = comment;
	}

	//Getters and Setters Follow:
	int Part::getKey() {
		return partKey;
	}
	
	void Part::setKey(int partKey) {
		this->partKey = partKey;
	}	
	
	string Part::getName() {
		return name;
	}

	void Part::setName(string name) {
		this->name = name;
	}

	string Part::getBrand() {
		return brand;
	}

	void Part::setBrand(string brand) {
		this->brand = brand;
	}

	string Part::getMfgr() {
		return mfgr;
	}

	void Part::setMfgr(string mfgr) {
		this->mfgr = mfgr;
	}

	string Part::getType() {
		return type;
	}

	void Part::setType(string type) {
		this->type = type;
	}

	int Part::getSize() {
		return size;
	}

	void Part::setSize(int size) {
		this->size = size;
	}

	string Part::getContainer() {
		return container;
	}

	void Part::setContainer(string container) {
		this->container = container;
	}

	double Part::getRetailPrice() {
		return retailPrice;
	}

	void Part::setRetailPrice(double retailPrice) {
		this->retailPrice = retailPrice;
	}

	string Part::getComment() {
		return comment;
	}

	void Part::setComment(string comment) {
		this->comment = comment;
	}	
	
	
	//C++: Explicit call needed for printing content:
	string Part::toString() {
		string stringS =  itos(this->getKey()) + " - " + this->getName() + " - " + this->getMfgr() + " - " + this->getBrand() + " - " + this->getType() + " - " + this->getComment() + " - " + this->getContainer()  + " - " + dtos(this->getRetailPrice()) + " - " + itos(this->getSize());	  
		return stringS;
	}	
	
	
	//Use for generating objects in bulk:
	vector<Part *> Part::generateObjects(int number) {
		vector<Part *> objectList;
		for (int i = 0; i < number; i++) {
			Part * tmp = new Part(i, "Name-" + itos(i), "MFGR-" + itos(i+1), "Brand-" + itos(i+2), "Type-" + itos(i+3), i+4, "Container-" + itos(i+5), i+6.1, "Comment-" + itos(i+7));
			objectList.push_back(tmp);
		}
		return objectList;
	}
	

	/*
	 * Functions related to Hand Coded Serialization.
	 */

	//Hand Coded C++ serialization:
	char * Part::serialize(char * buffer, int &objectSize) {
		//First determine the size of object:
		//Integer elements:
		objectSize = 2 * sizeof(int);
		//Double elements:
		objectSize += 1 * sizeof(double);
		//String sizes: 
		objectSize += 6 * sizeof(int);
		//String Elements:
		objectSize += (name.length() + mfgr.length() + brand.length() + type.length() + container.length() + comment.length());		


		//Now, serialize the object.
		char * tempBuffer = buffer;
		
		//Copy Integers:
		tempBuffer = copyInt(tempBuffer, partKey);	
		tempBuffer = copyInt(tempBuffer, size);	
		//Copy Doubles:		
		tempBuffer = copyDouble(tempBuffer, retailPrice);		
		//Copy Strings:
		tempBuffer = copyString(tempBuffer, name);		
		tempBuffer = copyString(tempBuffer, mfgr);
		tempBuffer = copyString(tempBuffer, brand);
		tempBuffer = copyString(tempBuffer, type);
		tempBuffer = copyString(tempBuffer, comment);
		tempBuffer = copyString(tempBuffer, container);
		
		return buffer;
	}


	
	int Part::deserialize(char * buffer) {
		int bytesRead = 0;
		
		//Key:
		partKey = parseInt(buffer+bytesRead);
		bytesRead += sizeof(partKey);	
		
		//Size:
		size = parseInt(buffer+bytesRead);
		bytesRead += sizeof(size);		
		
		//Retail Price:
		retailPrice = parseDouble(buffer+bytesRead);
		bytesRead += sizeof(retailPrice);
			
		//Parse Strings:
		parseString(buffer+bytesRead, name); bytesRead += (sizeof(int) + name.length());		
		parseString(buffer+bytesRead, mfgr); bytesRead += (sizeof(int) + mfgr.length());		
		parseString(buffer+bytesRead, brand); bytesRead += (sizeof(int) + brand.length());		
		parseString(buffer+bytesRead, type); bytesRead += (sizeof(int) + type.length());		
		parseString(buffer+bytesRead, comment); bytesRead += (sizeof(int) + comment.length());			
		parseString(buffer+bytesRead, container); bytesRead += (sizeof(int) + container.length());

		return  bytesRead;
	}	
	
	


	//Implement your own custom comparator:
	bool Part::operator< (Part& other) {	
		return (partKey < other.partKey);
	}



	//Create a inplace object:
	PartIP * Part::createInPlaceObject() {
		return new PartIP(partKey, name, mfgr, brand, type, size, container, retailPrice, comment);		
	}

	//In place serialization:
	long Part::serializeInPlace(char * fileStart) {
		//In place object:
		this->inplace = this->createInPlaceObject();

		//Return pointer:
		return ((long) inplace - (long)fileStart);
	}