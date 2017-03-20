#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "Order.h"
#include "LineItem.h"
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
	Order::Order() {	
	}


	//Constructor with arguments:
	Order::Order(vector<LineItem*> lineItems, int orderKey, int custKey, string orderStatus, double totalPrice, string orderDate, string orderPriority, string clerk, int shipPriority, string comment) {
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
	vector<LineItem*> Order::getLineItems(){
		return lineItems;
	}

	void Order::setLineItems(vector<LineItem*> lineItems) {
		this->lineItems = lineItems;
	}

	int Order::getOrderKey() {
		return orderKey;
	}

	void Order::setOrderKey(int orderKey) {
		this->orderKey = orderKey;
	}

	int Order::getCustKey() {
		return custKey;
	}

	void Order::setCustKey(int custKey) {
		this->custKey = custKey;
	}

	double Order::getTotalPrice() {
		return totalPrice;
	}

	void Order::setTotalPrice(double totalPrice) {
		this->totalPrice = totalPrice;
	}


	string Order::getOrderStatus() {
		return orderStatus;
	}

	 void Order::setOrderStatus(string orderStatus) {
		this->orderStatus = orderStatus;
	}

	string Order::getOrderDate() {
		return orderDate;
	}

	void Order::setOrderDate(string orderDate) {
		this->orderDate = orderDate;
	}

	string Order::getOrderPriority() {
		return orderPriority;
	}

	void Order::setOrderPriority(string orderPriority) {
		this->orderPriority = orderPriority;
	}

	string Order::getClerk() {
		return clerk;
	}

	void Order::setClerk(string clerk) {
		this->clerk = clerk;
	}

	int Order::getShipPriority() {
		return shipPriority;
	}

	void Order::setShipPriority(int shipPriority) {
		this->shipPriority = shipPriority;
	}

	string Order::getComment() {
		return comment;
	}

	void Order::setComment(string comment) {
		this->comment = comment;
	}			



	//C++: Explicit call needed for printing content:
	string Order::toString() {
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
	vector<Order *> Order::generateObjects(int number) {
		//Limiting the number of line items.
		int numLineItems = 4;
		vector<Order *> orderList;
		for (int j = 0; j < number; j++) {
			vector<LineItem *> lineItemList = LineItem::generateObjects(2 + (j%numLineItems));
			Order * t = new Order(lineItemList, j, j+1, "OrderStatus-" + itos(j+2),  j+3.0, "OrderDate-" + itos(j+4), "OrderPriority-" + itos(j+5), "Clerk-" + itos(j+6), j+7, "comment-" + itos(j+8));
			orderList.push_back(t);
		}
		return orderList;
	}


	/*
	 * Functions related to Hand Coded Serialization.
	 */
	char * Order::serialize(char * buffer, int& objectSize) {

		//First determine the size of object:
		//Integer elements:
		objectSize = 3 * sizeof(int);
		//Double elements:
		objectSize += 1 * sizeof(double);
		//String sizes: 
		objectSize += 5 * sizeof(int);
		//String Elements:
		objectSize += (orderStatus.length() + orderDate.length() + orderPriority.length() + clerk.length() + comment.length());		
		
		//Vector of lineitems:
		objectSize += sizeof(int);

		//Now, serialize the object.
		char * tempBuffer = buffer;
		
		//Copy Integers:
		tempBuffer = copyInt(tempBuffer, orderKey);	
		tempBuffer = copyInt(tempBuffer, custKey);	
		tempBuffer = copyInt(tempBuffer, shipPriority);

		//Copy Doubles:		
		tempBuffer = copyDouble(tempBuffer, totalPrice);

		//Copy Strings:
		tempBuffer = copyString(tempBuffer, orderStatus);		
		tempBuffer = copyString(tempBuffer, orderDate);
		tempBuffer = copyString(tempBuffer, orderPriority);
		tempBuffer = copyString(tempBuffer, clerk);
		tempBuffer = copyString(tempBuffer, comment);


		//Copy encapsulated objects:
		int numLineItems =  lineItems.size();
		tempBuffer = copyInt(tempBuffer, numLineItems);	


		//New code:
		int lineItemSize;
		for (int i = 0; i < lineItems.size(); i++) {
			lineItemSize = 0;
			lineItems.at(i)->serialize(tempBuffer, lineItemSize);
			tempBuffer += lineItemSize;
			objectSize += lineItemSize;
		}

		return buffer;
	}




	int Order::deserialize(char * buffer) {
		int bytesRead = 0;
		
		//Integers:
		orderKey = parseInt(buffer+bytesRead); bytesRead += sizeof(orderKey);	
		custKey = parseInt(buffer+bytesRead); bytesRead += sizeof(custKey);	
		shipPriority = parseInt(buffer+bytesRead); bytesRead += sizeof(shipPriority);		

		//Doubles:
		totalPrice = parseDouble(buffer+bytesRead); bytesRead += sizeof(totalPrice);

		//Strings:
		parseString(buffer+bytesRead, orderStatus); bytesRead += (sizeof(int) + orderStatus.length());
		parseString(buffer+bytesRead, orderDate); bytesRead += (sizeof(int) + orderDate.length());
		parseString(buffer+bytesRead, orderPriority); bytesRead += (sizeof(int) + orderPriority.length());
		parseString(buffer+bytesRead, clerk); bytesRead += (sizeof(int) + clerk.length());
		parseString(buffer+bytesRead, comment); bytesRead += (sizeof(int) + comment.length());


		//Encapsulated objects:
		int numLineItems = parseInt(buffer+bytesRead); bytesRead += sizeof(numLineItems);	

		LineItem * tempLineItem;
		for(int i = 0; i < numLineItems; i++) {
			tempLineItem = new LineItem();
			bytesRead += tempLineItem->deserialize(buffer+bytesRead);
			lineItems.push_back(tempLineItem);
		}

		return  bytesRead;
	}	




	//Implement your own custom comparator:
	bool Order::operator< (Order& other) {

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



	//Create a inplace object:
	OrderIP * Order::createInPlaceObject() {
		return new OrderIP(lineItems, lineItems.size(), orderKey, custKey, orderStatus, totalPrice, orderDate, orderPriority, clerk, shipPriority, comment);			
	}

	//In place serialization:
	long Order::serializeInPlace(char * fileStart) {
		//In place object:
		this->inplace = this->createInPlaceObject();	

		//Return pointer:
		return ((long)inplace - (long)fileStart);
	}