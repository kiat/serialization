#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "LineItemProto.h"

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
	LineItemProto::LineItemProto(const LineItemProto &obj) {
		throw invalid_argument("LineItemProto: Not copy constructible.");
	}

	//Default constructor:
	LineItemProto::LineItemProto() {
		//Protocol buffer object:
		this->isProtoSerialized = false;
	}


	//Constructor with arguments:
	LineItemProto::LineItemProto(string name, int orderKey, SupplierProto *supplier, PartProto *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
	string lineStatus, string shipDate, string commitDate, string receiptDate, string shipinStruct, string shipMode, string comment){

		//Create proto object
		proto = new proto::LineItemP;

		//Primitive member variables:
		proto->set_name(name);
		proto->set_orderkey(orderKey);
		proto->set_linenumber(lineNumber);
		proto->set_quantity(quantity);
		proto->set_extendedprice(extendedPrice);
		proto->set_discount(discount);
		proto->set_tax(tax);
		proto->set_returnflag(returnFlag);
		proto->set_linestatus(lineStatus);
		proto->set_shipdate(shipDate);
		proto->set_commitdate(commitDate);
		proto->set_receiptdate(receiptDate);
		proto->set_shipinstruct(shipinStruct);
		proto->set_shipmode(shipMode);
		proto->set_comment(comment);


		//Supplier:
		proto::LineItemP_SupplierP * s = proto->mutable_supplierp();
		s->set_supplierkey(supplier->getSupplierKey());
		s->set_name(supplier->getName());
		s->set_address(supplier->getAddress());
		s->set_nationkey(supplier->getNationKey());
		s->set_phone(supplier->getPhone());
		s->set_accbal(supplier->getAccbal());
		s->set_comment(supplier->getComment());


		//Part:
		proto::LineItemP_PartP * r = proto->mutable_partp();
		r->set_partkey(part->getKey());
		r->set_name(part->getName());
		r->set_mfgr(part->getMfgr());
		r->set_brand(part->getBrand());
		r->set_type(part->getType());
		r->set_size(part->getSize());
		r->set_container(part->getContainer());
		r->set_retailprice(part->getRetailPrice());
		r->set_comment(part->getComment());

		//Protocol buffer object:
		this->isProtoSerialized = true;

    }



	//Getters and Setters Follow:
	string LineItemProto::getName() {
		return this->proto->name();
	}

	int LineItemProto::getOrderKey() {
		return this->proto->orderkey();
	}

	int LineItemProto::getLineNumber() {
		return this->proto->linenumber();
	}

 	double LineItemProto::getQuantity() {
		return this->proto->quantity();
	}

	double LineItemProto::getExtendedPrice() {
		return this->proto->extendedprice();
	}

	double LineItemProto::getDiscount() {
		return this->proto->discount();
	}

	double LineItemProto::getTax() {
		return this->proto->tax();
	}

	string LineItemProto::getReturnFlag() {
		return this->proto->returnflag();
	}

	string LineItemProto::getLineStatus() {
		return this->proto->linestatus();
	}

	string LineItemProto::getShipDate() {
		return this->proto->shipdate();
	}

	string LineItemProto::getCommitDate() {
		return this->proto->commitdate();
	}

	string LineItemProto::getReceiptDate() {
		return this->proto->receiptdate();
	}

	string LineItemProto::getShipinStruct() {
		return this->proto->shipinstruct();
	}

	string LineItemProto::getShipMode() {
		return this->proto->shipmode();
	}

	string LineItemProto::getComment() {
		return this->proto->comment();
	}


	proto::LineItemP_PartP * LineItemProto::getPart() {
		return this->proto->mutable_partp();
	}

	proto::LineItemP_SupplierP * LineItemProto::getSupplier() {
		return this->proto->mutable_supplierp();
	}


	//C++: Explicit call needed for printing content:
	string LineItemProto::toString() {
		string stringS = this->getName() + " - " + to_string(this->getOrderKey())  + " - " + to_string(this->getLineNumber()) + " - " + to_string(this->getQuantity()) + " - " + to_string(this->getExtendedPrice())
		 + " - " + to_string(this->getDiscount())  + " - " + to_string(this->getTax()) + " - " + this->getReturnFlag() + " - " + this->getShipDate() + " - " + this->getShipinStruct() + " - " + this->getShipMode()
		 + " - " + this->getComment() + " - " + this->getLineStatus() + " - " + this->getCommitDate() + " - " + this->getReceiptDate();
		 //+  " - " + "-supplier-" + this->getSupplier()->toString() + "-part-" + this->getPart()->toString(); 

		return stringS;
	}

	//Use for generating objects in bulk:
	vector<LineItemProto *> LineItemProto::generateObjects(int number) {
		vector<LineItemProto *> objectList;
		for (int i = 0; i < number; i++) {
			
			SupplierProto* tempSupplier = new SupplierProto(i+2, "Name-" + to_string(i+2), "Address-" + to_string(i+2), i+2, "Phone-" + to_string(i+2), 0.1 * (i+2), "Comment-" + to_string(i+2));
			PartProto* tempPart = new PartProto(i+3, "Name-" + to_string(i+3), "MFGR-" + to_string(i+3), "Brand-" + to_string(i+3), "Type-" + to_string(i+3), i+3, "Container-" + to_string(i+3), i+3.0, "Comment-" + to_string(i+3));
			
			LineItemProto * tmp = new LineItemProto("Name-" + to_string(i), i+1, tempSupplier, tempPart, i+4, i+5.0, i+6.0, i+7.0, i+8.0, "a" + to_string(i+9), "a" + to_string(i+10), "Data-" + to_string(i+11), "Data-" + to_string(i+12), "Data-" + to_string(i+13), "a" + to_string(i+14), "a" + to_string(i+15), "comment" + to_string(i+16));
			objectList.push_back(tmp);

			delete tempSupplier;
			delete tempPart;
		}
		return objectList;
	}


	/*
	 * Functions related to Protocol Buffer Serialization.
	 */

	//Protocol buffer serialization:
	char * LineItemProto::serialize(char * buffer, int& objectSize) {

		//First determine the size of protobuf object:
		objectSize = sizeof(int) + this->proto->ByteSize();

		//Now, serialize the object.	
		char * tempBuffer = buffer;
		
		//Copy total size. 
		//This is weird. Protobuf doesn't understand end of an object in a buffer!
		tempBuffer = copyInt(tempBuffer, objectSize);	

		//Standard protobuf serialization:
		this->proto->SerializeToArray(tempBuffer, objectSize - sizeof(int));
		
		return buffer;
	}


	//Protocol buffer de-serialization:		
	int LineItemProto::deserialize(char * buffer) {
		int sizeofObject = 0;
		int bytesRead = 0;
		
		//Size of protobuf object:
		sizeofObject = parseInt(buffer+bytesRead);
		bytesRead += sizeof(sizeofObject);	
		
		//Standard protobuf serialization:
		this->proto = new proto::LineItemP;
		this->proto->ParseFromArray(buffer+bytesRead, sizeofObject - sizeof(int));

		//Memory Leak:
		isProtoSerialized = true;

		return sizeofObject;
	}


	//Implement your own custom comparator:
	bool LineItemProto::operator< (LineItemProto& other) {
		//Duplicate detection:
		if (getLineNumber() == other.getLineNumber()) {
			// They have the same "lineNumber"
			if (proto->mutable_partp()->partkey() < other.getPart()->partkey()) {
				return true;
			} 
			else if (proto->mutable_partp()->partkey() > other.getPart()->partkey()) {
				return false;
			} 
			else {
				// They have the same "partKey"
				if (proto->mutable_supplierp()->supplierkey() < other.getSupplier()->supplierkey()) {
					return true;
				} 
				else if (proto->mutable_supplierp()->supplierkey() > other.getSupplier()->supplierkey()) {
					return false;
				} 
				else {
					// They have the same "partKey" and "supplierKey"
					return false;
				}
			}
		}

		return getLineNumber() < other.getLineNumber();	
	}







