#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "LineItemBoost.h"
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


	//Copy constructor:
	LineItemBoost::LineItemBoost(const LineItemBoost &obj) {
		throw invalid_argument("LineItemBoost: Not copy constructible.");
	}

	//Default constructor:
	LineItemBoost::LineItemBoost() {
		//Memory Leak:
		this->hasPart = false;
		this->hasSupplier = false;	
	}


	//Constructor with arguments:
	LineItemBoost::LineItemBoost(string name, int orderKey, SupplierBoost *supplier, PartBoost *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
	string lineStatus, string shipDate, string commitDate, string receiptDate, string shipinStruct, string shipMode, string comment){
		this->name = name;
		this->orderKey = orderKey;
		this->supplier = supplier;
		this->part = part;
		this->lineNumber = lineNumber;
		this->quantity = quantity;
		this->extendedPrice = extendedPrice;
		this->discount = discount;
		this->tax = tax;
		this->returnFlag = returnFlag;
		this->lineStatus = lineStatus;
		this->shipDate = shipDate;
		this->commitDate = commitDate;
		this->receiptDate = receiptDate;
		this->shipinStruct = shipinStruct;
		this->shipMode = shipMode;
		this->comment = comment;

		//Memory Leak:
		this->hasPart = true;	
		this->hasSupplier = true;
    }



	//Getters and Setters Follow:
	string LineItemBoost::getName() {
		return name;
	}

	void LineItemBoost::setName(string name) {
		this->name = name;
	}

	int LineItemBoost::getOrderKey() {
		return orderKey;
	}

	void LineItemBoost::setOrderKey(int orderKey) {
		this->orderKey = orderKey;
	}

	int LineItemBoost::getLineNumber() {
		return lineNumber;
	}

	void LineItemBoost::setLineNumber(int lineNumber) {
		this->lineNumber = lineNumber;
	}

 	double LineItemBoost::getQuantity() {
		return quantity;
	}

	void LineItemBoost::setQuantity(double quantity) {
		this->quantity = quantity;
	}

	double LineItemBoost::getExtendedPrice() {
		return extendedPrice;
	}

	void LineItemBoost::setExtendedPrice(double extendedPrice) {
		this->extendedPrice = extendedPrice;
	}

	double LineItemBoost::getDiscount() {
		return discount;
	}

	 void LineItemBoost::setDiscount(double discount) {
		this->discount = discount;
	}

	double LineItemBoost::getTax() {
		return tax;
	}

	void LineItemBoost::setTax(double tax) {
		this->tax = tax;
	}

	string LineItemBoost::getReturnFlag() {
		return returnFlag;
	}

	void LineItemBoost::setReturnFlag(string returnFlag) {
		this->returnFlag = returnFlag;
	}

	string LineItemBoost::getLineStatus() {
		return lineStatus;
	}

	void LineItemBoost::setLineStatus(string lineStatus) {
		this->lineStatus = lineStatus;
	}

	string LineItemBoost::getShipDate() {
		return shipDate;
	}

	void LineItemBoost::setShipDate(string shipDate) {
		this->shipDate = shipDate;
	}

	string LineItemBoost::getCommitDate() {
		return commitDate;
	}

	void LineItemBoost::setCommitDate(string commitDate) {
		this->commitDate = commitDate;
	}

	string LineItemBoost::getReceiptDate() {
		return receiptDate;
	}

	void LineItemBoost::setReceiptDate(string receiptDate) {
		this->receiptDate = receiptDate;
	}

	string LineItemBoost::getShipinStruct() {
		return shipinStruct;
	}

	void LineItemBoost::setShipinStruct(string shipinStruct) {
		this->shipinStruct = shipinStruct;
	}

	string LineItemBoost::getShipMode() {
		return shipMode;
	}

	void LineItemBoost::setShipMode(string shipMode) {
		this->shipMode = shipMode;
	}

	string LineItemBoost::getComment() {
		return comment;
	}

	void LineItemBoost::setComment(string comment) {
		this->comment = comment;
	}


	PartBoost* LineItemBoost::getPart() {
		return part;
	}

	void LineItemBoost::setPart(PartBoost* part) {
		this->part = part;
		this->hasPart = true;
	}

	SupplierBoost* LineItemBoost::getSupplier() {
		return supplier;
	}

	void LineItemBoost::setSupplier(SupplierBoost* supplier) {
		this->supplier = supplier;
		this->hasSupplier = true;
	}



	//C++: Explicit call needed for printing content:
	string LineItemBoost::toString() {
		string stringS = this->getName() + " - " + to_string(this->getOrderKey())  + " - " + to_string(this->getLineNumber()) + " - " + to_string(this->getQuantity()) + " - " + to_string(this->getExtendedPrice())
		 + " - " + to_string(this->getDiscount())  + " - " + to_string(this->getTax()) + " - " + this->getReturnFlag() + " - " + this->getShipDate() + " - " + this->getShipinStruct() + " - " + this->getShipMode() 
		 + " - " + this->getComment() + " - " + this->getLineStatus() + " - " + this->getCommitDate() + " - " + this->getReceiptDate()  +  " - " + "-supplier-" + this->getSupplier()->toString() + "-part-" + this->getPart()->toString(); 
		;

		return stringS;
	}


	//Use for generating objects in bulk:
	vector<LineItemBoost *> LineItemBoost::generateObjects(int number) {
		vector<LineItemBoost *> objectList;
		for (int i = 0; i < number; i++) {
			LineItemBoost * tmp = new LineItemBoost("Name-" + to_string(i), i+1, new SupplierBoost(i+2, "Name-" + to_string(i+2), "Address-" + to_string(i+2), i+2, "Phone-" + to_string(i+2), 0.1 * (i+2), "Comment-" + to_string(i+2)), new PartBoost(i+3, "Name-" + to_string(i+3), "MFGR-" + to_string(i+3), "Brand-" + to_string(i+3), "Type-" + to_string(i+3), i+3, "Container-" + to_string(i+3), i+3.0, "Comment-" + to_string(i+3)), i+4, i+5.0, i+6.0, i+7.0, i+8.0, "a" + to_string(i+9), "a" + to_string(i+10), "Data-" + to_string(i+11), "Data-" + to_string(i+12), "Data-" + to_string(i+13), "a" + to_string(i+14), "a" + to_string(i+15), "comment" + to_string(i+16));
			objectList.push_back(tmp);
		}
		return objectList;
	}


	//Implement your own custom comparator:
	bool LineItemBoost::operator< (LineItemBoost& other) {
		//Duplicate detection:
		if (lineNumber == other.lineNumber) {
			// They have the same "lineNumber"
			if (part->getKey() < other.getPart()->getKey()) {
				return true;
			} 
			else if (part->getKey() > other.getPart()->getKey()) {
				return false;
			} 
			else {
				// They have the same "partKey"
				if (supplier->getSupplierKey() < other.getSupplier()->getSupplierKey()) {
					return true;
				} 
				else if (supplier->getSupplierKey() > other.getSupplier()->getSupplierKey()) {
					return false;
				} 
				else {
					// They have the same "partKey" and "supplierKey"
					return false;
				}
			}
		}

		return lineNumber < other.lineNumber;	
	}


	//Boost serialization:
	char * LineItemBoost::serialize(char * buffer, int& objectSize) {
		//Serialize:
		LineItemBoost * T = this;

		//Serializer:
		stringstream ss;
		boost::archive::text_oarchive oa(ss, boost::archive::no_header);

		//Serialize:
		oa << T;

		//First determine the size of object:
		objectSize = ss.str().size() + sizeof(int);

		//Now, serialize the object.
		char * tempBuffer = buffer;
		
		//Copy String:
		tempBuffer = copyString(tempBuffer, ss.str());
		
		return buffer;
	}


	//Boost deserialization:
	void LineItemBoost::deserializeBoost(char * buffer, LineItemBoost *& boostObject) {
		//Use this subsequently:
		char* tempBuffer = buffer;
		
		//Parse "length" of the string.
		int sizeofString;
		memcpy(&sizeofString, tempBuffer, sizeof(int));
		tempBuffer += sizeof(int);


		//Create stream on heap: Keep stream alive:
		stringstream *rs = new stringstream();
		rs->write(tempBuffer, sizeofString);

		//Create archive on heap: Keep stream alive:
		boost::archive::text_iarchive * ia = new boost::archive::text_iarchive(*rs, boost::archive::no_header);
		(*ia) >> boostObject;

		//Assign the streams: 
		freeArchive = ia;
		freeStream = rs;		
	}
