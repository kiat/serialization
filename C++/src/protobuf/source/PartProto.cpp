#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "PartProto.h"

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
	PartProto::PartProto(const PartProto &obj) {
		throw invalid_argument("PartProto: Not copy constructible.");
	}


	//Default constructor:
	PartProto::PartProto() {
		//Protocol buffer object:
		this->isProtoSerialized = false;
	}
	
	//Constructor with arguments:
	PartProto::PartProto(int partKey, string name, string mfgr, string brand, string type, int size, string container, double retailPrice, string comment) {
		proto = new proto::PartP;
		proto->set_partkey(partKey);
		proto->set_name(name);
		proto->set_mfgr(mfgr);
		proto->set_brand(brand);
		proto->set_type(type);
		proto->set_size(size);
		proto->set_container(container);
		proto->set_retailprice(retailPrice);
		proto->set_comment(comment);

		//Proto object has been created:
		isProtoSerialized = true;
	}

	//Getters and Setters Follow:
	int PartProto::getKey() {
		return this->proto->partkey();
	}
	
	string PartProto::getName() {
		return this->proto->name();
	}

	string PartProto::getBrand() {
		return this->proto->brand();
	}

	string PartProto::getMfgr() {
		return this->proto->mfgr();
	}

	string PartProto::getType() {
		return this->proto->type();
	}

	int PartProto::getSize() {
		return this->proto->size();
	}

	string PartProto::getContainer() {
		return this->proto->container();
	}

	double PartProto::getRetailPrice() {
		return this->proto->retailprice();
	}

	string PartProto::getComment() {
		return this->proto->comment();
	}
	
	//C++: Explicit call needed for printing content:
	string PartProto::toString() {
		string stringS =  itos(this->getKey()) + " - " + this->getName() + " - " + this->getMfgr() + " - " + this->getBrand() + " - " + this->getType() + " - " + this->getComment() + " - " + this->getContainer()  + " - " + dtos(this->getRetailPrice()) + " - " + itos(this->getSize());	  
		return stringS;
	}	
	
	
	//Use for generating objects in bulk:
	vector<PartProto *> PartProto::generateObjects(int number) {
		vector<PartProto *> objectList;
		for (int i = 0; i < number; i++) {
			PartProto * tmp = new PartProto(i, "Name-" + itos(i), "MFGR-" + itos(i+1), "Brand-" + itos(i+2), "Type-" + itos(i+3), i+4, "Container-" + itos(i+5), i+6.1, "Comment-" + itos(i+7));
			objectList.push_back(tmp);
		}
		return objectList;
	}
	

	/*
	 * Functions related to Protocol Buffer Serialization.
	 */

	
	//Protocol buffer serialization:
	char * PartProto::serialize(char * buffer, int& objectSize) {

		//First determine the size of protobuf object:
		objectSize = sizeof(int) + this->proto->ByteSize();

		//Now, serialize the object.	
		char * tempBuffer = buffer;
		
		//Copy total size. 
		//This is weird. Protobuf doesn't understand end of an object in a buffer!
		tempBuffer = copyInt(tempBuffer, objectSize);	

		//Standard protobuf serialization:
		this->proto->SerializeToArray(tempBuffer, objectSize - sizeof(int));
		
		return buffer;
	}


	//Protocol buffer de-serialization:		
	int PartProto::deserialize(char * buffer) {
		
		int sizeofObject = 0;
		int bytesRead = 0;
		
		//Size of protobuf object:
		sizeofObject = parseInt(buffer+bytesRead);
		bytesRead += sizeof(sizeofObject);	
		
		//Standard protobuf serialization:
		this->proto = new proto::PartP;
		this->proto->ParseFromArray(buffer+bytesRead, sizeofObject - sizeof(int));

		//Memory Leak:
		isProtoSerialized = true;

		return sizeofObject;
	}


	//Implement your own custom comparator:
	bool PartProto::operator< (PartProto& other) {	
		return (getKey() < other.getKey());
	}


