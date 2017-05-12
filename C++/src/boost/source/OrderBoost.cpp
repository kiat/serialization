#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "OrderBoost.h"
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
	OrderBoost::OrderBoost() {	
	}


	//Constructor with arguments:
	OrderBoost::OrderBoost(vector<LineItemBoost*> lineItems, int orderKey, int custKey, string orderStatus, double totalPrice, string orderDate, string orderPriority, string clerk, int shipPriority, string comment) {
		this->lineItems = lineItems;
		this->orderKey = orderKey;
		this->custKey = custKey;
		this->orderStatus = orderStatus;
		this->totalPrice = totalPrice;
		this->orderDate = orderDate;
		this->orderPriority = orderPriority;
		this->clerk = clerk;
		this->shipPriority = shipPriority;
		this->comment = comment;
	}


	//Getters and Setters Follow:
	vector<LineItemBoost*> OrderBoost::getLineItems(){
		return lineItems;
	}

	void OrderBoost::setLineItems(vector<LineItemBoost*> lineItems) {
		this->lineItems = lineItems;
	}

	int OrderBoost::getOrderKey() {
		return orderKey;
	}

	void OrderBoost::setOrderKey(int orderKey) {
		this->orderKey = orderKey;
	}

	int OrderBoost::getCustKey() {
		return custKey;
	}

	void OrderBoost::setCustKey(int custKey) {
		this->custKey = custKey;
	}

	double OrderBoost::getTotalPrice() {
		return totalPrice;
	}

	void OrderBoost::setTotalPrice(double totalPrice) {
		this->totalPrice = totalPrice;
	}


	string OrderBoost::getOrderStatus() {
		return orderStatus;
	}

	 void OrderBoost::setOrderStatus(string orderStatus) {
		this->orderStatus = orderStatus;
	}

	string OrderBoost::getOrderDate() {
		return orderDate;
	}

	void OrderBoost::setOrderDate(string orderDate) {
		this->orderDate = orderDate;
	}

	string OrderBoost::getOrderPriority() {
		return orderPriority;
	}

	void OrderBoost::setOrderPriority(string orderPriority) {
		this->orderPriority = orderPriority;
	}

	string OrderBoost::getClerk() {
		return clerk;
	}

	void OrderBoost::setClerk(string clerk) {
		this->clerk = clerk;
	}

	int OrderBoost::getShipPriority() {
		return shipPriority;
	}

	void OrderBoost::setShipPriority(int shipPriority) {
		this->shipPriority = shipPriority;
	}

	string OrderBoost::getComment() {
		return comment;
	}

	void OrderBoost::setComment(string comment) {
		this->comment = comment;
	}			



	//C++: Explicit call needed for printing content:
	string OrderBoost::toString() {
		string stringS = itos(this->getOrderKey()) + " - " + itos(this->getCustKey())  + " - " + this->getOrderStatus() + " - " + dtos(this->getTotalPrice()) + " - " + this->getOrderDate()+ " - " + this->getOrderPriority()
		 + " - " + this->getClerk() + " - " + itos(this->getShipPriority()) + " - " + this->getComment() ; 
		;
	
		//All lineitems:
		for (int i = 0; i < lineItems.size(); i++) {
			stringS += " LineItem " + itos(i) + " : " + lineItems.at(i)->toString();
		}

		return stringS;
	}





	//Use for generating objects in bulk:
	vector<OrderBoost *> OrderBoost::generateObjects(int number) {
		//Limiting the number of line items.
		int numLineItems = 4;
		vector<OrderBoost *> orderList;
		for (int j = 0; j < number; j++) {
			vector<LineItemBoost *> lineItemList = LineItemBoost::generateObjects(2 + (j%numLineItems));
			OrderBoost * t = new OrderBoost(lineItemList, j, j+1, "OrderStatus-" + itos(j+2),  j+3.0, "OrderDate-" + itos(j+4), "OrderPriority-" + itos(j+5), "Clerk-" + itos(j+6), j+7, "comment-" + itos(j+8));
			orderList.push_back(t);
		}
		return orderList;
	}




	//Implement your own custom comparator:
	bool OrderBoost::operator< (OrderBoost& other) {

		if (orderKey == other.orderKey) {
			bool result = false;

			if (lineItems.size() == other.getLineItems().size()) {
				for (int i = 0; i < lineItems.size(); i++) {
					result = lineItems.at(i) < (other.getLineItems().at(i));
					if (result) return true;
				}
			}

			return lineItems.size() < other.getLineItems().size();		
		}

		return orderKey < other.orderKey;	
	}




	//Boost serialization:
	char * OrderBoost::serialize(char * buffer, int& objectSize) {
		//Serialize:
		OrderBoost * T = this;

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

/*
	//Boost deserialization:
	OrderBoost* OrderBoost::deserialize(char * buffer) {
		int bytesRead = 0;		
		//Parse String:
		string object = parseString(buffer+bytesRead);
		bytesRead += (sizeof(int) + object.length());

		//Boost deserialization:
		OrderBoost * T;
		stringstream rs;
    	rs << object;

		boost::archive::text_iarchive ia(rs, boost::archive::no_header);
  		ia >> T;

    	return T;
	}
*/