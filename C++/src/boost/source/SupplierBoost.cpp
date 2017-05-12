#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "SupplierBoost.h"
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
	SupplierBoost::SupplierBoost() {
	}

	//Constructor with arguments:
	SupplierBoost::SupplierBoost(int supplierKey, string name, string address, int nationKey, string phone, double accbal, string comment) {
		this->supplierKey = supplierKey;
		this->name = name;
		this->address = address;
		this->nationKey = nationKey;
		this->phone = phone;
		this->accbal = accbal;
		this->comment = comment;
	}


	//Getters and Setters Follow:
	int SupplierBoost::getSupplierKey() {
		return supplierKey;
	}

	void SupplierBoost::setSupplierKey (int supplierKey) {
		this->supplierKey = supplierKey;
	}

	string SupplierBoost::getName() {
		return name;
	}

	void SupplierBoost::setName(string name) {
		this->name = name;
	}

	string SupplierBoost::getAddress() {
		return address;
	}

	void SupplierBoost::setAddress(string address) {
		this->address = address;
	}

	int SupplierBoost::getNationKey() {
		return nationKey;
	}

	void SupplierBoost::setNationKey(int nationKey) {
		this->nationKey = nationKey;
	}

	string SupplierBoost::getPhone() {
		return phone;
	}

	void SupplierBoost::setPhone(string phone) {
		this->phone = phone;
	}

	double SupplierBoost::getAccbal() {
		return accbal;
	}

	void SupplierBoost::setAccbal(double accbal) {
		this->accbal = accbal;
	}

	string SupplierBoost::getComment() {
		return comment;
	}

	void SupplierBoost::setComment(string comment) {
		this->comment = comment;
	}



	//C++: Explicit call needed for printing content:
	string SupplierBoost::toString() {
		string stringS =  itos(this->getSupplierKey()) + " - " + this->getName() + " - " + this->getAddress() + " - " + itos(this->getNationKey()) + " - " +  this->getPhone() + " - " + dtos(this->getAccbal()) + " - " + this->getComment();	  
		return stringS;
	}	



	//Use for generating objects in bulk:
	vector<SupplierBoost *> SupplierBoost::generateObjects(int number) {
		vector<SupplierBoost *> objectList;
		for (int i = 0; i < number; i++) {
			SupplierBoost * tmp = new SupplierBoost(i, "Name-" + itos(i), "Address-" + itos(i+1), i, "Phone-" + itos(i+2), 0.1 * i, "Comment-" + itos(i+3));
			objectList.push_back(tmp);
		}
		return objectList;
	}



	//Implement your own custom comparator:
	bool SupplierBoost::operator< (SupplierBoost& other) {	
		return (supplierKey < other.supplierKey);
	}


	//Boost serialization:
	char * SupplierBoost::serialize(char * buffer, int& objectSize) {
		
		//Serialize:
		SupplierBoost* T = this;

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

/*
	//Boost deserialization:
	SupplierBoost* SupplierBoost::deserialize(char * buffer) {
		int bytesRead = 0;		
		//Parse String:
		string object = parseString(buffer+bytesRead);
		bytesRead += (sizeof(int) + object.length());

		//Boost deserialization:
		SupplierBoost* T;
		stringstream rs;
    	rs << object;

		boost::archive::text_iarchive ia(rs, boost::archive::no_header);
  		ia >> T;

    	return T;
	}
*/