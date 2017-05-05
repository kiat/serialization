#ifndef LINEITEM_H
#define LINEITEM_H


#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "RootData.h"
#include "Supplier.h"
#include "Part.h"
#include "LineItemIP.h"

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


	class LineItem : public RootData {

	private:
		string name;
		int orderKey;

		Supplier *supplier;
		Part *part;

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

		//For in place objects:
		LineItemIP * inplace;

		//From the normal C++ object, create a protocol buffer object.
		LineItemIP * createInPlaceObject();	


	public:	

		//Default destructor:
		~LineItem() {
			if (hasPart) {
				delete part;
			}

			if (hasSupplier) {
				delete supplier;
			}
		}

		//Copy constructor:
		LineItem(const LineItem &obj);
		
		//Default constructor:
		LineItem();

		//Constructor with arguments:
		LineItem(string name, int orderKey, Supplier *supplier, Part *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
			string lineStatus, string shipDate, string commitDate, string receiptDate, string shipinStruct, string shipMode, string comment);
		

		//Basic setters and getters:
		string getName();
		void setName(string name);
		int getOrderKey();
		void setOrderKey(int orderKey);
		Supplier* getSupplier();
		void setSupplier(Supplier* supplier);
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
		Part* getPart();
		void setPart(Part* part);

		//C++: Use explicitly for printing.
		string toString();
		
		//Used to generate objects:
		static vector<LineItem *> generateObjects(int number);
		
		//Hand Coded C++ serialization:
		//char * serialize(int& objectSize);

		//New API: Writes directly to File Page:
		char * serialize(char* buffer, int &objectSize);


		//Hand Coded C++ de-serialization:		
		int deserialize(char * buffer) ;


		//Implement your own custom comparator:
		bool operator< (LineItem& other);

		//In place serialization:
		long serializeInPlace(char * fileStart);	
	};



#endif