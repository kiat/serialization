#ifndef SUPPLIERPROTO_H
#define SUPPLIERPROTO_H


#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "RootData.h"
#include "SupplierP.pb.h"
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


class SupplierProto : public RootData {

	private:
		//For protocol buffers:
		bool isProtoSerialized;
		proto::SupplierP * proto;

	public:	

		//Default destructor:
		~SupplierProto() {
			if(isProtoSerialized) {
				delete proto;
			}	
		}
		
		//Default constructor:
		SupplierProto();
		//Constructor with arguments:
		SupplierProto(int supplierKey, string name, string address, int nationKey, string phone, double accbal, string comment);


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
		static vector<SupplierProto *> generateObjects(int number);

		//Protocol buffer serialization:
		char * serialize(char * buffer, int& objectSize);
		//Protocol buffer de-serialization:		
		int deserialize(char * buffer) ;	
		
		//Implement your own custom comparator:
		bool operator< (SupplierProto& other);
};


#endif