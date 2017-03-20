#ifndef PARTPROTO_H
#define PARTPROTO_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "RootData.h"
#include "PartP.pb.h"
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

class PartProto : public RootData {

	private:
		//For protocol buffers:
		bool isProtoSerialized;
		proto::PartP * proto;

	public:

	//Default destructor:
	~PartProto() {
		if(isProtoSerialized) {
			delete proto;
		}	
	}

	//Copy constructor:
	PartProto(const PartProto &obj);
			
	//Default constructor:
	PartProto();
	//Constructor with arguments:
	PartProto(int partKey, string name, string mfgr, string brand, string type, int size, string container, double retailPrice, string comment);
	
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
	static vector<PartProto *> generateObjects(int number);
	
	//Protocol buffer serialization:
	char * serialize(char * buffer, int& objectSize);
	//Protocol buffer de-serialization:		
	int deserialize(char * buffer) ;	


	//Implement your own custom comparator:
	bool operator< (PartProto& other);
};



#endif
