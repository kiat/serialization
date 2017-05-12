#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "Supplier.h"
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
	Supplier::Supplier() {		
	}


	//Constructor with arguments:
	Supplier::Supplier(int supplierKey, string name, string address, int nationKey, string phone, double accbal, string comment) {
		this->supplierKey = supplierKey;
		this->name = name;
		this->address = address;
		this->nationKey = nationKey;
		this->phone = phone;
		this->accbal = accbal;
		this->comment = comment;		
	}


	//Getters and Setters Follow:
	int Supplier::getSupplierKey() {
		return supplierKey;
	}

	void Supplier::setSupplierKey (int supplierKey) {
		this->supplierKey = supplierKey;
	}

	string Supplier::getName() {
		return name;
	}

	void Supplier::setName(string name) {
		this->name = name;
	}

	string Supplier::getAddress() {
		return address;
	}

	void Supplier::setAddress(string address) {
		this->address = address;
	}

	int Supplier::getNationKey() {
		return nationKey;
	}

	void Supplier::setNationKey(int nationKey) {
		this->nationKey = nationKey;
	}

	string Supplier::getPhone() {
		return phone;
	}

	void Supplier::setPhone(string phone) {
		this->phone = phone;
	}

	double Supplier::getAccbal() {
		return accbal;
	}

	void Supplier::setAccbal(double accbal) {
		this->accbal = accbal;
	}

	string Supplier::getComment() {
		return comment;
	}

	void Supplier::setComment(string comment) {
		this->comment = comment;
	}



	//C++: Explicit call needed for printing content:
	string Supplier::toString() {
		string stringS =  itos(this->getSupplierKey()) + " - " + this->getName() + " - " + this->getAddress() + " - " + itos(this->getNationKey()) + " - " +  this->getPhone() + " - " + dtos(this->getAccbal()) + " - " + this->getComment();	  
		return stringS;
	}	



	//Use for generating objects in bulk:
	vector<Supplier *> Supplier::generateObjects(int number) {
		vector<Supplier *> objectList;
		for (int i = 0; i < number; i++) {
			Supplier * tmp = new Supplier(i, "Name-" + itos(i), "Address-" + itos(i+1), i, "Phone-" + itos(i+2), 0.1 * i, "Comment-" + itos(i+3));
			objectList.push_back(tmp);
		}
		return objectList;
	}



	/*
	 * Functions related to Hand Coded Serialization.
	 */

	//Hand Coded C++ serialization:
	char * Supplier::serialize(char * buffer, int& objectSize) {

		//First determine the size of object:
		//Integer elements:
		objectSize = 2 * sizeof(int);
		//Double elements:
		objectSize += 1 * sizeof(double);
		//String sizes: 
		objectSize += 4 * sizeof(int);
		//String Elements:
		objectSize += (name.length() + address.length() + phone.length() + comment.length());	


		//Now, serialize the object.
		char * tempBuffer = buffer;
		
		//Copy Integers:
		tempBuffer = copyInt(tempBuffer, supplierKey);	
		tempBuffer = copyInt(tempBuffer, nationKey);	
		//Copy Doubles:		
		tempBuffer = copyDouble(tempBuffer, accbal);		
		//Copy Strings:
		tempBuffer = copyString(tempBuffer, name);		
		tempBuffer = copyString(tempBuffer, address);
		tempBuffer = copyString(tempBuffer, phone);
		tempBuffer = copyString(tempBuffer, comment);

		return buffer;
	}



	int Supplier::deserialize(char * buffer) {
		int bytesRead = 0;
		
		//supplierKey:
		supplierKey = parseInt(buffer+bytesRead);
		bytesRead += sizeof(supplierKey);	
		
		//nationKey:
		nationKey = parseInt(buffer+bytesRead);
		bytesRead += sizeof(nationKey);		
		
		//Account Balance:
		accbal = parseDouble(buffer+bytesRead);
		bytesRead += sizeof(accbal);
			
		//Parse Strings:
		parseString(buffer+bytesRead, name); 		bytesRead += (sizeof(int) + name.length());		
		parseString(buffer+bytesRead, address);		bytesRead += (sizeof(int) + address.length());		
		parseString(buffer+bytesRead, phone);		bytesRead += (sizeof(int) + phone.length());		
		parseString(buffer+bytesRead, comment);		bytesRead += (sizeof(int) + comment.length());

		return  bytesRead;
	}	


	//Implement your own custom comparator:
	bool Supplier::operator< (Supplier& other) {	
		return (supplierKey < other.supplierKey);
	}


	//Create a inplace object:
	SupplierIP * Supplier::createInPlaceObject() {
		return new SupplierIP(supplierKey, name, address, nationKey, phone, accbal, comment);		
	}

	//In place serialization:
	long Supplier::serializeInPlace(char * fileStart) {
		//In place object:
		this->inplace = this->createInPlaceObject();

		//Return pointer:
		return ((long) inplace - (long)fileStart);
	}