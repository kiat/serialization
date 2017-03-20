#ifndef LINEITEMBOOST_H
#define LINEITEMBOOST_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "RootData.h"
#include "SupplierBoost.h"
#include "PartBoost.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;



	// CREATE TABLE [dbo].[LINEITEM](
	// [L_ORDERKEY] [int] NOT NULL,
	// [L_PARTKEY] [int] NOT NULL,
	// [L_SUPPKEY] [int] NOT NULL,
	// [L_LINENUMBER] [int] NOT NULL,
	// [L_QUANTITY] [decimal](15, 2) NOT NULL,
	// [L_EXTENDEDPRICE] [decimal](15, 2) NOT NULL,
	// [L_DISCOUNT] [decimal](15, 2) NOT NULL,
	// [L_TAX] [decimal](15, 2) NOT NULL,
	// [L_RETURNFLAG] [char](1) NOT NULL,
	// [L_LINESTATUS] [char](1) NOT NULL,
	// [L_SHIPDATE] [date] NOT NULL,
	// [L_COMMITDATE] [date] NOT NULL,
	// [L_RECEIPTDATE] [date] NOT NULL,
	// [L_SHIPINSTRUCT] [char](25) NOT NULL,
	// [L_SHIPMODE] [char](10) NOT NULL,
	// [L_COMMENT] [varchar](44) NOT NULL
	// );


	class LineItemBoost : public RootData {

	private:
		string name;
		int orderKey;

		SupplierBoost *supplier;
		PartBoost *part;

		int lineNumber;
		double quantity;
		double extendedPrice;
		double discount;
		double tax;
		string returnFlag;
		string lineStatus;

		string shipDate;
		string commitDate;
		string receiptDate;

		string shipinStruct;
		string shipMode;
		string comment;

		//Problem: Memory leak: When to free Supplier, Parts:
		bool hasPart;
		bool hasSupplier;


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
			ar & name;
			ar & orderKey;

			ar & supplier;
			ar & part;

			ar & lineNumber;
			ar & quantity;
			ar & extendedPrice;
			ar & discount;
			ar & tax;
			ar & returnFlag;
			ar & lineStatus;

			ar & shipDate;
			ar & commitDate;
			ar & receiptDate;

			ar & shipinStruct;
			ar & shipMode;
			ar & comment;	

			//Prevent Memory Leak:
			ar & hasPart;
			ar & hasSupplier;	
	    }


	public:	


		boost::archive::text_iarchive * getArchive() {
			return freeArchive;
		}

		stringstream* getStream(){
			return freeStream;
		}


		void freePointedObjects() {
			if (hasPart) {
				delete part;
			}

			if (hasSupplier) {
				delete supplier;
			}				
		}


		//Default destructor:
		~LineItemBoost() {
		}

		//Copy constructor:
		LineItemBoost(const LineItemBoost &obj);
		
		//Default constructor:
		LineItemBoost();

		//Constructor with arguments:
		LineItemBoost(string name, int orderKey, SupplierBoost *supplier, PartBoost *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
			string lineStatus, string shipDate, string commitDate, string receiptDate, string shipinStruct, string shipMode, string comment);
		

		//Set booleans:
		// void setHasPart(bool value) {
		// 	hasPart = value;
		// }

		// void setHasSupplier(bool value) {
		// 	hasSupplier = value;
		// }


		//Basic setters and getters:
		string getName();
		void setName(string name);
		int getOrderKey();
		void setOrderKey(int orderKey);
		SupplierBoost* getSupplier();
		void setSupplier(SupplierBoost* supplier);
		int getLineNumber();
		void setLineNumber(int lineNumber);
		double getQuantity();
		void setQuantity(double quantity);
		double getExtendedPrice() ;
		void setExtendedPrice(double extendedPrice);
		double getDiscount();
		void setDiscount(double discount);
		double getTax();
		void setTax(double tax);
		string getReturnFlag();
		void setReturnFlag(string returnFlag);
		string getLineStatus();
		void setLineStatus(string lineStatus);
		string getShipDate();
		void setShipDate(string shipDate);
		string getCommitDate();
		void setCommitDate(string commitDate);
		string getReceiptDate();
		void setReceiptDate(string receiptDate);
		string getShipinStruct();
		void setShipinStruct(string shipinStruct);
		string getShipMode();
		void setShipMode(string shipMode);
		string getComment();
		void setComment(string comment);
		PartBoost* getPart();
		void setPart(PartBoost* part);

		//C++: Use explicitly for printing.
		string toString();
		
		//Used to generate objects:
		static vector<LineItemBoost *> generateObjects(int number);


		//Boost serialization:
		char * serialize(char * buffer, int &objectSize);

		//Boost deserialization:
		void deserializeBoost(char *, LineItemBoost *&);
		
		//Copy all content:
		void copyContent(LineItemBoost *);

		//Implement your own custom comparator:
		bool operator< (LineItemBoost& other);
	};



#endif