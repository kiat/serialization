#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "PartBoost.h"

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
	PartBoost::PartBoost(const PartBoost &obj) {
		throw invalid_argument("PartBoost: Not copy constructible.");
	}

	//Default constructor:
	PartBoost::PartBoost() {		
	}
	
	//Constructor with arguments:
	PartBoost::PartBoost(int partKey, string name, string mfgr, string brand, string type, int size, string container, double retailPrice, string comment) {
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
	int PartBoost::getKey() {
		return partKey;
	}
	
	void PartBoost::setKey(int partKey) {
		this->partKey = partKey;
	}	
	
	string PartBoost::getName() {
		return name;
	}

	void PartBoost::setName(string name) {
		this->name = name;
	}

	string PartBoost::getBrand() {
		return brand;
	}

	void PartBoost::setBrand(string brand) {
		this->brand = brand;
	}

	string PartBoost::getMfgr() {
		return mfgr;
	}

	void PartBoost::setMfgr(string mfgr) {
		this->mfgr = mfgr;
	}

	string PartBoost::getType() {
		return type;
	}

	void PartBoost::setType(string type) {
		this->type = type;
	}

	int PartBoost::getSize() {
		return size;
	}

	void PartBoost::setSize(int size) {
		this->size = size;
	}

	string PartBoost::getContainer() {
		return container;
	}

	void PartBoost::setContainer(string container) {
		this->container = container;
	}

	double PartBoost::getRetailPrice() {
		return retailPrice;
	}

	void PartBoost::setRetailPrice(double retailPrice) {
		this->retailPrice = retailPrice;
	}

	string PartBoost::getComment() {
		return comment;
	}

	void PartBoost::setComment(string comment) {
		this->comment = comment;
	}	
	
	
	//C++: Explicit call needed for printing content:
	string PartBoost::toString() {
		string stringS =  itos(this->getKey()) + " - " + this->getName() + " - " + this->getMfgr() + " - " + this->getBrand() + " - " + this->getType() + " - " + this->getComment() + " - " + this->getContainer()  + " - " + dtos(this->getRetailPrice()) + " - " + itos(this->getSize());	  
		return stringS;
	}	
	
	
	//Use for generating objects in bulk:
	vector<PartBoost *> PartBoost::generateObjects(int number) {
		vector<PartBoost *> objectList;
		for (int i = 0; i < number; i++) {
			PartBoost * tmp = new PartBoost(i, "Name-" + itos(i), "MFGR-" + itos(i+1), "Brand-" + itos(i+2), "Type-" + itos(i+3), i+4, "Container-" + itos(i+5), i+6.1, "Comment-" + itos(i+7));
			objectList.push_back(tmp);
		}
		return objectList;
	}
	

	//Implement your own custom comparator:
	bool PartBoost::operator< (PartBoost& other) {	
		return (partKey < other.partKey);
	}



	//Boost serialization:
	char * PartBoost::serialize(char * buffer, int& objectSize) {

		//Serialize:
		PartBoost * T = this;

		//Serializer:
		stringstream ss;
		boost::archive::text_oarchive oa(ss, boost::archive::no_header);

		//Serialize:
		oa << T;

		//First determine the size of object:
		objectSize = ss.str().size() + sizeof(int);

		//Now, serialize the object.
		char * tempBuffer = buffer;
		
		//Copy String:
		tempBuffer = copyString(tempBuffer, ss.str());
		
		return buffer;
	}



	//Boost deserialization:
	void PartBoost::deserializeBoost(char * buffer, PartBoost *& boostObject) {
		//Use this subsequently:
		char* tempBuffer = buffer;
		
		//Parse "length" of the string.
		int sizeofString;
		memcpy(&sizeofString, tempBuffer, sizeof(int));
		tempBuffer += sizeof(int);


		//Create stream on heap: Keep stream alive:
		stringstream *rs = new stringstream();
		rs->write(tempBuffer, sizeofString);

		//Create archive on heap: Keep stream alive:
		boost::archive::text_iarchive * ia = new boost::archive::text_iarchive(*rs, boost::archive::no_header);
		(*ia) >> boostObject;

		//Assign the streams: 
		freeArchive = ia;
		freeStream = rs;
	}

