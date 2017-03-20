#ifndef SUPPLIER_H
#define SUPPLIER_H


#include <vector>
#include "RootData.h"
#include "SupplierIP.h"

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


class Supplier : public RootData {

	private:
		int supplierKey;
		string name;
		string address;
		int nationKey;
		string phone;
		double accbal;
		string comment;


		//For in place objects:
		SupplierIP * inplace;
		
		//From the normal C++ object, create a protocol buffer object.
		SupplierIP * createInPlaceObject();		



	public:	
		//Default destructor:
		~Supplier() {
		}
		
		//Default constructor:
		Supplier();
		//Constructor with arguments:
		Supplier(int supplierKey, string name, string address, int nationKey, string phone, double accbal, string comment);


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
		static vector<Supplier *> generateObjects(int number);

		//Hand Coded C++ serialization:
		//char * serialize(int& objectSize);

		//New API: Writes directly to File Page:
		char * serialize(char* buffer, int &objectSize);


		//Hand Coded C++ de-serialization:		
		int deserialize(char * buffer) ;

		//Implement your own custom comparator:
		bool operator< (Supplier& other);

		//In place serialization:
		long serializeInPlace(char * fileStart);	
};


#endif