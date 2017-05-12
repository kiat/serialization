#ifndef LINEITEMPROTO_H
#define LINEITEMPROTO_H


#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "RootData.h"
#include "SupplierProto.h"
#include "PartProto.h"
#include "LineItemP.pb.h"

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


	class LineItemProto : public RootData {

	private:

		//For protocol buffers:
		bool isProtoSerialized;
		proto::LineItemP * proto;

	public:	

		//Default destructor:
		~LineItemProto() {
			if(isProtoSerialized) {
				delete proto;
			}
		}

		//Copy constructor:
		LineItemProto(const LineItemProto &obj);
		
		//Default constructor:
		LineItemProto();

		//Constructor with arguments:
		LineItemProto(string name, int orderKey, SupplierProto *supplier, PartProto *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
			string lineStatus, string shipDate, string commitDate, string receiptDate, string shipinStruct, string shipMode, string comment);
		

		//Basic setters and getters:
		string getName();
		void setName(string name);
		int getOrderKey();
		void setOrderKey(int orderKey);
		proto::LineItemP_SupplierP * getSupplier();
		// void setSupplier(Supplier* supplier);
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
		proto::LineItemP_PartP * getPart();
		// void setPart(Part* part);

		//C++: Use explicitly for printing.
		string toString();
		
		//Used to generate objects:
		static vector<LineItemProto *> generateObjects(int number);
		
		//Protocol buffer serialization:
		char * serialize(char * buffer, int& objectSize);
		//Protocol buffer de-serialization:		
		int deserialize(char * buffer) ;	

		//Implement your own custom comparator:
		bool operator< (LineItemProto& other);
	
	};



#endif