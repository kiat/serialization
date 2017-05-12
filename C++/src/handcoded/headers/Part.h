#ifndef PART_H
#define PART_H


#include<iostream>
#include<cstring>
#include <vector>
#include <stdexcept>
#include "RootData.h"
#include "PartIP.h"

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


/*
 *	Contains only the declaration of Item Part Class.
 */

class Part : public RootData {

	private:
		int partKey;
		string name;
		string mfgr;
		string brand;
		string type;
		int size;
		string container;
		double retailPrice;
		string comment;


		//For in place objects:
		PartIP * inplace;

		//From the normal C++ object, create a protocol buffer object.
		PartIP * createInPlaceObject();			

	public:

	//Default destructor:
	~Part() {
	}

	//Copy constructor:
	Part(const Part &obj);
			
	//Default constructor:
	Part();
	//Constructor with arguments:
	Part(int partKey, string name, string mfgr, string brand, string type, int size, string container, double retailPrice, string comment);
	
	//Basic setters and getters:
	int getKey();
	void setKey(int partKey);
	string getName();
	void setName(string name);
	string getBrand();
	void setBrand(string brand);
	string getMfgr();
	void setMfgr(string mfgr);
	string getType();
	void setType(string type);
	int getSize();
	void setSize(int size);
	string getContainer();
	void setContainer(string container);
	double getRetailPrice();
	void setRetailPrice(double retailPrice);
	string getComment();
	void setComment(string comment);
	
	//C++: Use explicitly for printing.
	string toString();
	
	//Used to generate objects:
	static vector<Part *> generateObjects(int number);
	
	//Hand Coded C++ serialization:
	// char * serialize(int &objectSize);

	//New API: Writes directly to File Page:
	char * serialize(char* buffer, int &objectSize);

	
	//Hand Coded C++ de-serialization:		
	int deserialize(char * buffer) ;

	//Implement your own custom comparator:
	bool operator< (Part& other);


	//In place serialization:
	long serializeInPlace(char * fileStart);
};



#endif
