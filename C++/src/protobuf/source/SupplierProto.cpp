#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "SupplierProto.h"
#include "SupplierP.pb.h"
using namespace std;


	// CREATE TABLE [dbo].[SUPPLIER](
	// [S_SUPPKEY] [int] NOT NULL,
	// [S_NAME] [char](25) NOT NULL,
	// [S_ADDRESS] [varchar](40) NOT NULL,
	// [S_NATIONKEY] [int] NOT NULL,
	// [S_PHONE] [char](15) NOT NULL,
	// [S_ACCTBAL] [decimal](15, 2) NOT NULL,
	// [S_COMMENT] [varchar](101) NOT NULL
	// );



	//Default constructor:
	SupplierProto::SupplierProto() {
		//Protocol buffer object:
		this->isProtoSerialized = false;
	}

	//Constructor with arguments:
	SupplierProto::SupplierProto(int supplierKey, string name, string address, int nationKey, string phone, double accbal, string comment) {
		proto = new proto::SupplierP;

		proto->set_supplierkey(supplierKey);
		proto->set_name(name);
		proto->set_address(address);
		proto->set_nationkey(nationKey);
		proto->set_phone(phone);
		proto->set_accbal(accbal);
		proto->set_comment(comment);

		//Proto object has been created:
		isProtoSerialized = true;		
	}


	//Getters and Setters Follow:
	int SupplierProto::getSupplierKey() {
		return this->proto->supplierkey();
	}

	string SupplierProto::getName() {
		return this->proto->name();
	}

	string SupplierProto::getAddress() {
		return this->proto->address();
	}

	int SupplierProto::getNationKey() {
		return this->proto->nationkey();
	}

	string SupplierProto::getPhone() {
		return this->proto->phone();
	}

	double SupplierProto::getAccbal() {
		return this->proto->accbal();
	}

	string SupplierProto::getComment() {
		return this->proto->comment();
	}

	//C++: Explicit call needed for printing content:
	string SupplierProto::toString() {
		string stringS =  itos(this->getSupplierKey()) + " - " + this->getName() + " - " + this->getAddress() + " - " + itos(this->getNationKey()) + " - " +  this->getPhone() + " - " + dtos(this->getAccbal()) + " - " + this->getComment();	  
		return stringS;
	}	



	//Use for generating objects in bulk:
	vector<SupplierProto *> SupplierProto::generateObjects(int number) {
		vector<SupplierProto *> objectList;
		for (int i = 0; i < number; i++) {
			SupplierProto * tmp = new SupplierProto(i, "Name-" + itos(i), "Address-" + itos(i+1), i, "Phone-" + itos(i+2), 0.1 * i, "Comment-" + itos(i+3));
			objectList.push_back(tmp);
		}
		return objectList;
	}

	/*
	 * Functions related to Protocol Buffer Serialization.
	 */

	//Protocol buffer serialization:
	char * SupplierProto::serialize(char * buffer, int& objectSize) {

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
	int SupplierProto::deserialize(char * buffer) {	
		int sizeofObject = 0;
		int bytesRead = 0;
		
		//Size of protobuf object:
		sizeofObject = parseInt(buffer+bytesRead);
		bytesRead += sizeof(sizeofObject);	
		
		//Standard protobuf serialization:
		this->proto = new proto::SupplierP;
		this->proto->ParseFromArray(buffer+bytesRead, sizeofObject - sizeof(int));

		//Memory Leak:
		isProtoSerialized = true;

		return sizeofObject;
	}



	//Implement your own custom comparator:
	bool SupplierProto::operator< (SupplierProto& other) {	
		return (getSupplierKey() < other.getSupplierKey());
	}



