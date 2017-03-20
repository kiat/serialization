#ifndef LINEITEMIP_H
#define LINEITEMIP_H


#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "Object.h"
#include "SupplierIP.h"
#include "PartIP.h"
#include "Part.h"
#include "Supplier.h"
using namespace std;


	class LineItemIP : public Object {

	private:
		offset_ptr <char> name;
		int orderKey;
		offset_ptr <SupplierIP> supplier;
		offset_ptr <PartIP> part;
		int lineNumber;
		double quantity;
		double extendedPrice;
		double discount;
		double tax;
		offset_ptr <char> returnFlag;
		offset_ptr <char> lineStatus;
		offset_ptr <char> shipDate;
		offset_ptr <char> commitDate;
		offset_ptr <char> receiptDate;
		offset_ptr <char> shipinStruct;
		offset_ptr <char> shipMode;
		offset_ptr <char> comment;

	public:	
		//Default constructor:
		LineItemIP();

		//Copy constructor:
		LineItemIP(const LineItemIP &obj);

		//Constructor with arguments:
		LineItemIP(string name, int orderKey, Supplier *supplier, Part *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
			string lineStatus, string shipDate, string commitDate, string receiptDate, string shipinStruct, string shipMode, string comment);
		
		// To be used alongside default constructor:
		void setUp(string name, int orderKey, Supplier *supplier, Part *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
			string lineStatus, string shipDate, string commitDate, string receiptDate, string shipinStruct, string shipMode, string comment);


		//Basic getters:
		string getName();
		int getOrderKey();
		SupplierIP* getSupplier();
		int getLineNumber();
		double getQuantity();
		double getExtendedPrice() ;
		double getDiscount();
		double getTax();
		string getReturnFlag();
		string getLineStatus();
		string getShipDate();
		string getCommitDate();
		string getReceiptDate();
		string getShipinStruct();
		string getShipMode();
		string getComment();
		PartIP* getPart();

		//C++: Use explicitly for printing.
		string toString();

		//Implement your own custom comparator:
		bool operator< (LineItemIP& other);
	
	};

#endif