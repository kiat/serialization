#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "LineItem.h"
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
	LineItem::LineItem(const LineItem &obj) {
		throw invalid_argument("LineItem: Not copy constructible.");
	}

	//Default constructor:
	LineItem::LineItem() {
		//Memory Leak:
		this->hasPart = false;
		this->hasSupplier = false;	
	}


	//Constructor with arguments:
	LineItem::LineItem(string name, int orderKey, Supplier *supplier, Part *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
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
	string LineItem::getName() {
		return name;
	}

	void LineItem::setName(string name) {
		this->name = name;
	}

	int LineItem::getOrderKey() {
		return orderKey;
	}

	void LineItem::setOrderKey(int orderKey) {
		this->orderKey = orderKey;
	}

	int LineItem::getLineNumber() {
		return lineNumber;
	}

	void LineItem::setLineNumber(int lineNumber) {
		this->lineNumber = lineNumber;
	}

 	double LineItem::getQuantity() {
		return quantity;
	}

	void LineItem::setQuantity(double quantity) {
		this->quantity = quantity;
	}

	double LineItem::getExtendedPrice() {
		return extendedPrice;
	}

	void LineItem::setExtendedPrice(double extendedPrice) {
		this->extendedPrice = extendedPrice;
	}

	double LineItem::getDiscount() {
		return discount;
	}

	 void LineItem::setDiscount(double discount) {
		this->discount = discount;
	}

	double LineItem::getTax() {
		return tax;
	}

	void LineItem::setTax(double tax) {
		this->tax = tax;
	}

	string LineItem::getReturnFlag() {
		return returnFlag;
	}

	void LineItem::setReturnFlag(string returnFlag) {
		this->returnFlag = returnFlag;
	}

	string LineItem::getLineStatus() {
		return lineStatus;
	}

	void LineItem::setLineStatus(string lineStatus) {
		this->lineStatus = lineStatus;
	}

	string LineItem::getShipDate() {
		return shipDate;
	}

	void LineItem::setShipDate(string shipDate) {
		this->shipDate = shipDate;
	}

	string LineItem::getCommitDate() {
		return commitDate;
	}

	void LineItem::setCommitDate(string commitDate) {
		this->commitDate = commitDate;
	}

	string LineItem::getReceiptDate() {
		return receiptDate;
	}

	void LineItem::setReceiptDate(string receiptDate) {
		this->receiptDate = receiptDate;
	}

	string LineItem::getShipinStruct() {
		return shipinStruct;
	}

	void LineItem::setShipinStruct(string shipinStruct) {
		this->shipinStruct = shipinStruct;
	}

	string LineItem::getShipMode() {
		return shipMode;
	}

	void LineItem::setShipMode(string shipMode) {
		this->shipMode = shipMode;
	}

	string LineItem::getComment() {
		return comment;
	}

	void LineItem::setComment(string comment) {
		this->comment = comment;
	}


	Part* LineItem::getPart() {
		return part;
	}

	void LineItem::setPart(Part* part) {
		this->part = part;
		this->hasPart = true;
	}

	Supplier* LineItem::getSupplier() {
		return supplier;
	}

	void LineItem::setSupplier(Supplier* supplier) {
		this->supplier = supplier;
		this->hasSupplier = true;
	}






	//C++: Explicit call needed for printing content:
	string LineItem::toString() {
		string stringS = this->getName() + " - " + to_string(this->getOrderKey())  + " - " + to_string(this->getLineNumber()) + " - " + to_string(this->getQuantity()) + " - " + to_string(this->getExtendedPrice())
		 + " - " + to_string(this->getDiscount())  + " - " + to_string(this->getTax()) + " - " + this->getReturnFlag() + " - " + this->getShipDate() + " - " + this->getShipinStruct() + " - " + this->getShipMode() 
		 + " - " + this->getComment() + " - " + this->getLineStatus() + " - " + this->getCommitDate() + " - " + this->getReceiptDate()  +  " - " + "-supplier-" + this->getSupplier()->toString() + "-part-" + this->getPart()->toString(); 
		;

		return stringS;
	}




	//Use for generating objects in bulk:
	vector<LineItem *> LineItem::generateObjects(int number) {
		vector<LineItem *> objectList;
		for (int i = 0; i < number; i++) {
			LineItem * tmp = new LineItem("Name-" + to_string(i), i+1, new Supplier(i+2, "Name-" + to_string(i+2), "Address-" + to_string(i+2), i+2, "Phone-" + to_string(i+2), 0.1 * (i+2), "Comment-" + to_string(i+2)), new Part(i+3, "Name-" + to_string(i+3), "MFGR-" + to_string(i+3), "Brand-" + to_string(i+3), "Type-" + to_string(i+3), i+3, "Container-" + to_string(i+3), i+3.0, "Comment-" + to_string(i+3)), i+4, i+5.0, i+6.0, i+7.0, i+8.0, "a" + to_string(i+9), "a" + to_string(i+10), "Data-" + to_string(i+11), "Data-" + to_string(i+12), "Data-" + to_string(i+13), "a" + to_string(i+14), "a" + to_string(i+15), "comment" + to_string(i+16));
			objectList.push_back(tmp);
		}
		return objectList;
	}



	/*
	 * Functions related to Hand Coded Serialization.
	 */
	char * LineItem::serialize(char * buffer, int& objectSize) {

		//First determine the size of object:
		//Integer elements:
		objectSize = 2 * sizeof(int);
		//Double elements:
		objectSize += 4 * sizeof(double);
		//String sizes: 
		objectSize += 9 * sizeof(int);
		//String Elements:
		objectSize += (name.length() + returnFlag.length() + lineStatus.length() + shipDate.length() + commitDate.length() + receiptDate.length() + shipinStruct.length() + shipMode.length() + comment.length());		
		

		//Now, serialize the object.
		char * tempBuffer = buffer;
		
		//Copy Integers:
		tempBuffer = copyInt(tempBuffer, orderKey);	
		tempBuffer = copyInt(tempBuffer, lineNumber);	
		//Copy Doubles:		
		tempBuffer = copyDouble(tempBuffer, quantity);
		tempBuffer = copyDouble(tempBuffer, extendedPrice);
		tempBuffer = copyDouble(tempBuffer, discount);
		tempBuffer = copyDouble(tempBuffer, tax);
		//Copy Strings:
		tempBuffer = copyString(tempBuffer, name);		
		tempBuffer = copyString(tempBuffer, returnFlag);
		tempBuffer = copyString(tempBuffer, lineStatus);
		tempBuffer = copyString(tempBuffer, shipDate);
		tempBuffer = copyString(tempBuffer, commitDate);
		tempBuffer = copyString(tempBuffer, receiptDate);
		tempBuffer = copyString(tempBuffer, shipinStruct);
		tempBuffer = copyString(tempBuffer, shipMode);
		tempBuffer = copyString(tempBuffer, comment);


		//Encapsulated objects:
		int encapsulatedSupplierSize;
		int encapsulatedPartSize;

		//Supplier:
		encapsulatedSupplierSize = 0;
		char * supplierBuffer = supplier->serialize(tempBuffer, encapsulatedSupplierSize);
		objectSize += encapsulatedSupplierSize;
		tempBuffer += encapsulatedSupplierSize;

		//Part:
		encapsulatedPartSize = 0;
		char * partBuffer = part->serialize(tempBuffer, encapsulatedPartSize);
		objectSize += encapsulatedPartSize;
		tempBuffer += encapsulatedPartSize;

		return buffer;
	}

	int LineItem::deserialize(char * buffer) {
		int bytesRead = 0;
		
		//Integers:
		orderKey = parseInt(buffer+bytesRead); bytesRead += sizeof(orderKey);	
		lineNumber = parseInt(buffer+bytesRead); bytesRead += sizeof(lineNumber);		

		//Doubles:
		quantity = parseDouble(buffer+bytesRead); bytesRead += sizeof(quantity);
		extendedPrice = parseDouble(buffer+bytesRead); bytesRead += sizeof(extendedPrice);
		discount = parseDouble(buffer+bytesRead); bytesRead += sizeof(discount);
		tax = parseDouble(buffer+bytesRead); bytesRead += sizeof(tax);

		//Strings:
		parseString(buffer+bytesRead, name); bytesRead += (sizeof(int) + name.length());
		parseString(buffer+bytesRead, returnFlag); bytesRead += (sizeof(int) + returnFlag.length());
		parseString(buffer+bytesRead, lineStatus); bytesRead += (sizeof(int) + lineStatus.length());
		parseString(buffer+bytesRead, shipDate); bytesRead += (sizeof(int) + shipDate.length());
		parseString(buffer+bytesRead, commitDate); bytesRead += (sizeof(int) + commitDate.length());
		parseString(buffer+bytesRead, receiptDate); bytesRead += (sizeof(int) + receiptDate.length());
		parseString(buffer+bytesRead, shipinStruct); bytesRead += (sizeof(int) + shipinStruct.length());
		parseString(buffer+bytesRead, shipMode); bytesRead += (sizeof(int) + shipMode.length());
		parseString(buffer+bytesRead, comment); bytesRead += (sizeof(int) + comment.length());


		//Encapsulated objects:
		supplier = new Supplier();
		bytesRead += supplier->deserialize(buffer+bytesRead);

		part = new Part();
		bytesRead += part->deserialize(buffer+bytesRead);

		//Memory Leak:
		hasPart = true;
		hasSupplier = true;

		return  bytesRead;
	}	



	//Implement your own custom comparator:
	bool LineItem::operator< (LineItem& other) {
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



	//Create a inplace object:
	LineItemIP * LineItem::createInPlaceObject() {
		return new LineItemIP(name, orderKey, supplier, part, lineNumber, quantity, extendedPrice, discount, tax, returnFlag,
	lineStatus, shipDate, commitDate, receiptDate, shipinStruct, shipMode, comment);			
	}

	//In place serialization:
	long LineItem::serializeInPlace(char * fileStart) {
		//In place object:
		this->inplace = this->createInPlaceObject();

		//Return pointer:
		return ((long)inplace - (long)fileStart);
	}