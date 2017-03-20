#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "OrderProto.h"
#include "OrderP.pb.h"
using namespace std;


	// CREATE TABLE ORDERS ( O_ORDERKEY INTEGER NOT NULL,
	// O_CUSTKEY INTEGER NOT NULL,
	// O_ORDERSTATUS CHAR(1) NOT NULL,
	// O_TOTALPRICE DECIMAL(15,2) NOT NULL,
	// O_ORDERDATE DATE NOT NULL,
	// O_ORDERPRIORITY CHAR(15) NOT NULL,
	// O_CLERK CHAR(15) NOT NULL,
	// O_SHIPPRIORITY INTEGER NOT NULL,
	// O_COMMENT VARCHAR(79) NOT NULL);



	//Default constructor:
	OrderProto::OrderProto() {
		//Protocol buffer object:
		this->isProtoSerialized = false;
	}


	//Constructor with arguments:
	OrderProto::OrderProto(vector<LineItemProto*> lineItems, int orderKey, int custKey, string orderStatus, double totalPrice, string orderDate, string orderPriority, string clerk, int shipPriority, string comment) {

		proto = new proto::OrderP;

		//Primitive member variables:
		proto->set_orderkey(orderKey);
		proto->set_custkey(custKey);
		proto->set_orderstatus(orderStatus);
		proto->set_totalprice(totalPrice);
		proto->set_orderdate(orderDate);
		proto->set_orderpriority(orderPriority);
		proto->set_clerk(clerk);
		proto->set_shippriority(shipPriority);
		proto->set_comment(comment);


		//All lineitems.
		for (int i = 0; i < lineItems.size(); i++) {
			proto::OrderP_LineItemP * l = proto->add_lineitem();

			l->set_name(lineItems.at(i)->getName());
			l->set_orderkey(lineItems.at(i)->getOrderKey());
			l->set_linenumber(lineItems.at(i)->getLineNumber());
			l->set_quantity(lineItems.at(i)->getQuantity());
			l->set_extendedprice(lineItems.at(i)->getExtendedPrice());
			l->set_discount(lineItems.at(i)->getDiscount());
			l->set_tax(lineItems.at(i)->getTax());
			l->set_returnflag(lineItems.at(i)->getReturnFlag());
			l->set_linestatus(lineItems.at(i)->getLineStatus());
			l->set_shipdate(lineItems.at(i)->getShipDate());
			l->set_commitdate(lineItems.at(i)->getCommitDate());
			l->set_receiptdate(lineItems.at(i)->getReceiptDate());
			l->set_shipinstruct(lineItems.at(i)->getShipinStruct());
			l->set_shipmode(lineItems.at(i)->getShipMode());
			l->set_comment(lineItems.at(i)->getComment());

			//Supplier (inside lineitem):
			proto::OrderP_LineItemP_SupplierP * s = l->mutable_supplierp();
			s->set_supplierkey(lineItems.at(i)->getSupplier()->supplierkey());
			s->set_name(lineItems.at(i)->getSupplier()->name());
			s->set_address(lineItems.at(i)->getSupplier()->address());
			s->set_nationkey(lineItems.at(i)->getSupplier()->nationkey());
			s->set_phone(lineItems.at(i)->getSupplier()->phone());
			s->set_accbal(lineItems.at(i)->getSupplier()->accbal());
			s->set_comment(lineItems.at(i)->getSupplier()->comment());


			//Part (inside lineitem):
			proto::OrderP_LineItemP_PartP * r = l->mutable_partp();
			r->set_partkey(lineItems.at(i)->getPart()->partkey());
			r->set_name(lineItems.at(i)->getPart()->name());
			r->set_mfgr(lineItems.at(i)->getPart()->mfgr());
			r->set_brand(lineItems.at(i)->getPart()->brand());
			r->set_type(lineItems.at(i)->getPart()->type());
			r->set_size(lineItems.at(i)->getPart()->size());
			r->set_container(lineItems.at(i)->getPart()->container());
			r->set_retailprice(lineItems.at(i)->getPart()->retailprice());
			r->set_comment(lineItems.at(i)->getPart()->comment());	
		}

		//Protocol buffer object:
		this->isProtoSerialized = true;
	}


	//Getters and Setters Follow:
	proto::OrderP_LineItemP * OrderProto::getLineItems(int index) {
		return this->proto->mutable_lineitem(index);
	}
	
	int OrderProto::getLineItemsCount() {
		return this->proto->lineitem_size();
	}

	int OrderProto::getOrderKey() {
		return this->proto->orderkey();
	}

	int OrderProto::getCustKey() {
		return this->proto->custkey();
	}

	double OrderProto::getTotalPrice() {
		return this->proto->totalprice();
	}

	string OrderProto::getOrderStatus() {
		return this->proto->orderstatus();
	}

	string OrderProto::getOrderDate() {
		return this->proto->orderdate();
	}

	string OrderProto::getOrderPriority() {
		return this->proto->orderpriority();
	}


	string OrderProto::getClerk() {
		return this->proto->clerk();
	}

	int OrderProto::getShipPriority() {
		return this->proto->shippriority();
	}

	string OrderProto::getComment() {
		return this->proto->comment();
	}


	//C++: Explicit call needed for printing content:
	string OrderProto::toString() {
		string stringS = itos(this->getOrderKey()) + " - " + itos(this->getCustKey())  + " - " + this->getOrderStatus() + " - " + dtos(this->getTotalPrice()) + " - " + this->getOrderDate()+ " - " + this->getOrderPriority()
		 + " - " + this->getClerk() + " - " + itos(this->getShipPriority()) + " - " + this->getComment() ; 
		;
	
		//All lineitems:
		// for (int i = 0; i < lineItems.size(); i++) {
		// 	stringS += " LineItem " + itos(i) + " : " + lineItems.at(i)->toString();
		// }

		return stringS;
	}


	//Use for generating objects in bulk:
	vector<OrderProto *> OrderProto::generateObjects(int number) {
		//Limiting the number of line items.
		int numLineItems = 4;
		vector<OrderProto *> orderList;
		for (int j = 0; j < number; j++) {
			vector<LineItemProto *> lineItemList = LineItemProto::generateObjects(2 + (j%numLineItems));
			OrderProto * t = new OrderProto(lineItemList, j, j+1, "OrderStatus-" + itos(j+2),  j+3.0, "OrderDate-" + itos(j+4), "OrderPriority-" + itos(j+5), "Clerk-" + itos(j+6), j+7, "comment-" + itos(j+8));
			orderList.push_back(t);


			for (int k = 0; k < lineItemList.size(); k++) {
				delete lineItemList.at(k);
			}
		}
		return orderList;
	}

	/*
	 * Functions related to Protocol Buffer Serialization.
	 */
	//Protocol buffer serialization:
	char * OrderProto::serialize(char * buffer, int &objectSize) {

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
	int OrderProto::deserialize(char * buffer) {
		
		int sizeofObject = 0;
		int bytesRead = 0;
		
		//Size of protobuf object:
		sizeofObject = parseInt(buffer+bytesRead);
		bytesRead += sizeof(sizeofObject);	
		
		//Standard protobuf serialization:
		this->proto = new proto::OrderP;
		this->proto->ParseFromArray(buffer+bytesRead, sizeofObject - sizeof(int));

		//Memory Leak:
		isProtoSerialized = true;

		return sizeofObject;
	}



	//Duplicate:
	bool OrderProto::LineItemComparator(proto::OrderP_LineItemP *a, proto::OrderP_LineItemP *b) {
		//Duplicate detection:
		if (a->linenumber() == b->linenumber()) {
			// They have the same "lineNumber"
			if (a->mutable_partp()->partkey() < b->mutable_partp()->partkey()) {
				return true;
			} 
			else if (a->mutable_partp()->partkey() > b->mutable_partp()->partkey()) {
				return false;
			} 
			else {
				// They have the same "partKey"
				if (a->mutable_supplierp()->supplierkey() < b->mutable_supplierp()->supplierkey()) {
					return true;
				} 
				else if (a->mutable_supplierp()->supplierkey() > b->mutable_supplierp()->supplierkey()) {
					return false;
				} 
				else {
					// They have the same "partKey" and "supplierKey"
					return false;
				}
			}
		}

		return a->linenumber() < b->linenumber();		
	}




	// //Implement your own custom comparator:
	bool OrderProto::operator< (OrderProto& other) {

		if (getOrderKey() == other.getOrderKey()) {
			bool result = false;

			if (getLineItemsCount() == other.getLineItemsCount()) {
				for (int i = 0; i < getLineItemsCount(); i++) {

					result = LineItemComparator(getLineItems(i), other.getLineItems(i));
					if (result) return true;
				}
			}

			return getLineItemsCount() < other.getLineItemsCount();		
		}

		return getOrderKey() < other.getOrderKey();	
	}



