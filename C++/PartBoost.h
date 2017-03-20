#ifndef PARTBOOST_H
#define PARTBOOST_H


#include<iostream>
#include<cstring>
#include <vector>
#include <stdexcept>
#include "RootData.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

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

class PartBoost : public RootData {

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


		 //Memory Leak:
		 boost::archive::text_iarchive * freeArchive;
		 stringstream * freeStream;

		//Boost Serialization:
	    friend class boost::serialization::access;
	    
	    //Serialize method for boost:
	    template<class Archive>
	    void serialize(Archive & ar, const unsigned int version)
	    {
	        // Simply list all the fields to be serialized/deserialized.
			ar & partKey;
			ar & name;
			ar & mfgr;
			ar & brand;
			ar & type;
			ar & size;
			ar & container;
			ar & retailPrice;
			ar & comment;
	    }



	public:

	boost::archive::text_iarchive * getArchive() {
		return freeArchive;
	}

	stringstream* getStream(){
		return freeStream;
	}


	void freePointedObjects() {					
	}

	//Default destructor:
	~PartBoost() {
	}

	//Copy constructor:
	PartBoost(const PartBoost &obj);
			
	//Default constructor:
	PartBoost();
	//Constructor with arguments:
	PartBoost(int partKey, string name, string mfgr, string brand, string type, int size, string container, double retailPrice, string comment);
	
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
	static vector<PartBoost *> generateObjects(int number);
	
	//Boost serialization:
	char * serialize(char * buffer, int &objectSize);


	//Boost deserialization:
	void deserializeBoost(char *, PartBoost *&);

	//Implement your own custom comparator:
	bool operator< (PartBoost& other);
};



#endif
