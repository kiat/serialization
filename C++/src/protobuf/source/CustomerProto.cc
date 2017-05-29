#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "CustomerProto.h"
using namespace std;

	//Copy constructor:
	CustomerProto::CustomerProto(const CustomerProto &obj) {
		throw invalid_argument("CustomerProto: Not copy constructible.");
	}
	
	//Default constructor:
	CustomerProto::CustomerProto() {
		//Protocol buffer object:
		this->isProtoSerialized = false;
	}


	//Constructor with arguments:
	CustomerProto::CustomerProto(vector<OrderProto*> orders, int custKey, string name, string address, int nationKey, string phone, double accbal, string mktsegment, string comment) {
		proto = new proto::CustomerP;

		//Primitive member variables:
		proto->set_custkey(custKey);
		proto->set_name(name);
		proto->set_address(address);
		proto->set_nationkey(nationKey);
		proto->set_phone(phone);
		proto->set_accbal(accbal);
		proto->set_mktsegment(mktsegment);
		proto->set_comment(comment);


		//All orders.
		for (int i = 0; i < orders.size(); i++) {
			proto::CustomerP_OrderP * order = proto->add_order();

			order->set_orderkey(orders.at(i)->getOrderKey());
			order->set_custkey(orders.at(i)->getCustKey());
			order->set_orderstatus(orders.at(i)->getOrderStatus());
			order->set_totalprice(orders.at(i)->getTotalPrice());
			order->set_orderdate(orders.at(i)->getOrderDate());
			order->set_orderpriority(orders.at(i)->getOrderPriority());
			order->set_clerk(orders.at(i)->getClerk());
			order->set_shippriority(orders.at(i)->getShipPriority());
			order->set_comment(orders.at(i)->getComment());

			//All lineitems.
			for (int j = 0; j < orders.at(i)->getLineItemsCount(); j++) {
				proto::CustomerP_OrderP_LineItemP * l = order->add_lineitem();
				proto::OrderP_LineItemP * curLineItem = orders.at(i)->getLineItems(j);
				
				l->set_name(curLineItem->name());
				l->set_orderkey(curLineItem->orderkey());
				l->set_linenumber(curLineItem->linenumber());
				l->set_quantity(curLineItem->quantity());
				l->set_extendedprice(curLineItem->extendedprice());
				l->set_discount(curLineItem->discount());
				l->set_tax(curLineItem->tax());
				l->set_returnflag(curLineItem->returnflag());
				l->set_linestatus(curLineItem->linestatus());
				l->set_shipdate(curLineItem->shipdate());
				l->set_commitdate(curLineItem->commitdate());
				l->set_receiptdate(curLineItem->receiptdate());
				l->set_shipinstruct(curLineItem->shipinstruct());
				l->set_shipmode(curLineItem->shipmode());
				l->set_comment(curLineItem->comment());


				//Supplier (inside lineitem):
				proto::CustomerP_OrderP_LineItemP_SupplierP * s = l->mutable_supplierp();
				s->set_supplierkey(curLineItem->mutable_supplierp()->supplierkey());
				s->set_name(curLineItem->mutable_supplierp()->name());
				s->set_address(curLineItem->mutable_supplierp()->address());
				s->set_nationkey(curLineItem->mutable_supplierp()->nationkey());
				s->set_phone(curLineItem->mutable_supplierp()->phone());
				s->set_accbal(curLineItem->mutable_supplierp()->accbal());
				s->set_comment(curLineItem->mutable_supplierp()->comment());


				//Part (inside lineitem):
				proto::CustomerP_OrderP_LineItemP_PartP * r = l->mutable_partp();
				r->set_partkey(curLineItem->mutable_partp()->partkey());
				r->set_name(curLineItem->mutable_partp()->name());
				r->set_mfgr(curLineItem->mutable_partp()->mfgr());
				r->set_brand(curLineItem->mutable_partp()->brand());
				r->set_type(curLineItem->mutable_partp()->type());
				r->set_size(curLineItem->mutable_partp()->size());
				r->set_container(curLineItem->mutable_partp()->container());
				r->set_retailprice(curLineItem->mutable_partp()->retailprice());
				r->set_comment(curLineItem->mutable_partp()->comment());	
			}
		}

		//Protocol buffer object:
		this->isProtoSerialized = true;
	}


	//Getters and Setters Follow:
	proto::CustomerP_OrderP * CustomerProto::getOrders(int index) {
		return this->proto->mutable_order(index);
	}

	int CustomerProto::getOrdersCount() {
		return this->proto->order_size();
	}

	int CustomerProto::getCustKey() {
		return this->proto->custkey();
	}

	string CustomerProto::getName() {
		return this->proto->name();
	}

	string CustomerProto::getAddress() {
		return this->proto->address();
	}

	int CustomerProto::getNationKey() {
		return this->proto->nationkey();
	}

	string CustomerProto::getPhone() {
		return this->proto->phone();
	}

	double CustomerProto::getAccbal() {
		return this->proto->accbal();
	}

	string CustomerProto::getMktsegment() {
		return this->proto->mktsegment();
	}

	string CustomerProto::getComment() {
		return this->proto->comment();
	}

	//C++: Explicit call needed for printing content:
	string CustomerProto::toString() {
		string stringS = itos(this->getCustKey())  + " - " + this->getName() + " - " + this->getAddress() +  " - "  + itos(this->getNationKey())   + " - " + this->getPhone() + " - " + dtos(this->getAccbal())  + " - " + this->getMktsegment() + " - " + this->getComment();
		
		//All orders:
		// for (int i = 0; i < orders.size(); i++) {
		// 	stringS += " Order " + itos(i) + " : " + orders.at(i)->toString();
		// }

		return stringS;
	}


	//Use for generating objects in bulk:
	vector<CustomerProto *> CustomerProto::generateObjects(int number) {
		//Limiting the number of orders.
		int numOrders = 2;
		vector<CustomerProto *> customerList;

		for (int j = 0; j < number; j++) {
			vector<OrderProto *> orderList =  OrderProto::generateObjects(2 + (j%numOrders));

			CustomerProto * t = new CustomerProto(orderList, j, "Name-" + itos(j+1) , "Address-" + itos(j+2),  j+3, "Phone-" + itos(j+4), j+5.0, "MarketSegment-" + itos(j+6), "Comment-" + itos(j+8));
			customerList.push_back(t);


			for (int k = 0; k < orderList.size(); k++) {
				delete orderList.at(k);
			}
		}
		return customerList;
	}


	/*
	 * Functions related to Protocol Buffer Serialization.
	 */

	//Protocol buffer serialization:
/*
	char * CustomerProto::serialize(int& objectSize) {

		//First determine the size of protobuf object:
		objectSize = sizeof(int) + this->proto->ByteSize();

		//Now, serialize the object.
		char * buffer = new char[objectSize];	
		char * tempBuffer = buffer;
		
		//Copy total size. 
		//This is weird. Protobuf doesn't understand end of an object in a buffer!
		tempBuffer = copyInt(tempBuffer, objectSize);	

		//Standard protobuf serialization:
		this->proto->SerializeToArray(tempBuffer, objectSize - sizeof(int));
		
		return buffer;
	}
*/

	char * CustomerProto::serialize(char * buffer, int& objectSize) {

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
	int CustomerProto::deserialize(char * buffer) {
		
		int sizeofObject = 0;
		int bytesRead = 0;
		
		//Size of protobuf object:
		sizeofObject = parseInt(buffer+bytesRead);
		bytesRead += sizeof(sizeofObject);	
		
		//Standard protobuf serialization:
		this->proto = new proto::CustomerP;
		this->proto->ParseFromArray(buffer+bytesRead, sizeofObject - sizeof(int));

		//Memory Leak:
		isProtoSerialized = true;

		return sizeofObject;
	}



	//Internal comparators:
	bool CustomerProto::LineItemComparator(proto::CustomerP_OrderP_LineItemP *a, proto::CustomerP_OrderP_LineItemP *b) {
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

	bool CustomerProto::OrderComparator (proto::CustomerP_OrderP *a, proto::CustomerP_OrderP *b) {
		if (a->orderkey() == b->orderkey()) {
			bool result = false;

			if (a->lineitem_size() == b->lineitem_size()) {
				for (int i = 0; i < a->lineitem_size(); i++) {
					result = LineItemComparator(a->mutable_lineitem(i), b->mutable_lineitem(i));
					if (result) return true;
				}
			}

			return a->lineitem_size() < b->lineitem_size();		
		}

		return a->orderkey() < b->orderkey();	
	}


	//Implement your own custom comparator:
	bool CustomerProto::operator< (CustomerProto& other) {
		if (getCustKey() == other.getCustKey()) {
			bool result = false;

			if (getOrdersCount() == other.getOrdersCount()) {
				for (int i = 0; i < getOrdersCount(); i++) {
					result = OrderComparator(getOrders(i), other.getOrders(i));
					if (result) return true;
				}
			}

			return getOrdersCount() < other.getOrdersCount();		
		}

		return getCustKey() < other.getCustKey();	
	}