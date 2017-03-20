#ifndef SUPPLIERBOOST_H
#define SUPPLIERBOOST_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "RootData.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;

/*
 *  Contains only the declaration of Supplier schema as it is specified in TPC-H benchmark
 */


	// CREATE TABLE [dbo].[SUPPLIER](
	// [S_SUPPKEY] [int] NOT NULL,
	// [S_NAME] [char](25) NOT NULL,
	// [S_ADDRESS] [varchar](40) NOT NULL,
	// [S_NATIONKEY] [int] NOT NULL,
	// [S_PHONE] [char](15) NOT NULL,
	// [S_ACCTBAL] [decimal](15, 2) NOT NULL,
	// [S_COMMENT] [varchar](101) NOT NULL
	// );


class SupplierBoost : public RootData {

	private:
		int supplierKey;
		string name;
		string address;
		int nationKey;
		string phone;
		double accbal;
		string comment;

		//Boost Serialization:
	    friend class boost::serialization::access;
	    
	    //Serialize method for boost:
	    template<class Archive>
	    void serialize(Archive & ar, const unsigned int version)
	    {
	        // Simply list all the fields to be serialized/deserialized.
			ar &  supplierKey;
			ar &  name;
			ar &  address;
			ar &  nationKey;
			ar &  phone;
			ar &  accbal;
			ar &  comment;
	    }


	public:	
		//Default destructor:
		~SupplierBoost() {
		}
		
		//Default constructor:
		SupplierBoost();
		//Constructor with arguments:
		SupplierBoost(int supplierKey, string name, string address, int nationKey, string phone, double accbal, string comment);


		//Basic setters and getters:
		int getSupplierKey();
		void setSupplierKey(int supplierKey);
	 	string getName();
		void setName(string name);
		string getAddress();
		void setAddress(string address);
		int getNationKey();	
		void setNationKey(int nationKey);
		string getPhone(); 
		void setPhone(string phone);
		double getAccbal();
		void setAccbal(double accbal);
		string getComment();
		void setComment(string comment);


		//C++: Use explicitly for printing.
		string toString();

		//Used to generate objects:
		static vector<SupplierBoost *> generateObjects(int number);

		//Boost serialization:
		char * serialize(char * buffer, int& objectSize);


		//Boost deserialization:
		//SupplierBoost* deserialize(char *);

		//Implement your own custom comparator:
		bool operator< (SupplierBoost& other);

};


#endif