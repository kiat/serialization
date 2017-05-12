#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <chrono>
#include "Customer.h"
#include "Order.h"
using namespace std;

	//Copy constructor:
	Customer::Customer(const Customer &obj) {
		throw invalid_argument("Customer: Not copy constructible.");
	}
	
	//Default constructor:
	Customer::Customer() {	
	}


	//Constructor with arguments:
	Customer::Customer(vector<Order*> orders, int custKey, string name, string address, int nationKey, string phone, double accbal, string mktsegment, string comment) {
		this->orders = orders;
		this->custKey = custKey;
		this->name = name;
		this->address = address;
		this->nationKey = nationKey;
		this->phone = phone;
		this->accbal = accbal;
		this->mktsegment = mktsegment;
		this->comment = comment;
	}


	//Getters and Setters Follow:
	vector<Order*> Customer::getOrders() {
		return orders;
	}

	void Customer::setOrders(vector<Order*> orders) {
		this->orders = orders;
	}

	int Customer::getCustKey() {
		return custKey;
	}

	void Customer::setCustKey(int custKey) {
		this->custKey = custKey;
	}

	string Customer::getName() {
		return name;
	}

	void Customer::setName(string name) {
		this->name = name;
	}

	string Customer::getAddress() {
		return address;
	}

	void Customer::setAddress(string address) {
		this->address = address;
	}

	int Customer::getNationKey() {
		return nationKey;
	}

	void Customer::setNationKey(int nationKey) {
		this->nationKey = nationKey;
	}

	string Customer::getPhone() {
		return phone;
	}

	void Customer::setPhone(string phone) {
		this->phone = phone;
	}


	double Customer::getAccbal() {
		return accbal;
	}

	void Customer::setAccbal(double accbal) {
		this->accbal = accbal;
	}

	string Customer::getMktsegment() {
		return mktsegment;
	}

	void Customer::setMktsegment(string mktsegment) {
		this->mktsegment = mktsegment;
	}

	string Customer::getComment() {
		return comment;
	}

	void Customer::setComment(string comment) {
		this->comment = comment;
	}			



	//C++: Explicit call needed for printing content:
	string Customer::toString() {
		string stringS = itos(this->getCustKey())  + " - " + this->getName() + " - " + this->getAddress() +  " - "  + itos(this->getNationKey())   + " - " + this->getPhone() + " - " + dtos(this->getAccbal())  + " - " + this->getMktsegment() + " - " + this->getComment();
		
		//All orders:
		for (int i = 0; i < orders.size(); i++) {
			stringS += " Order " + itos(i) + " : " + orders.at(i)->toString();
		}

		return stringS;
	}



	//Use for generating objects in bulk:
	vector<Customer *> Customer::generateObjects(int number) {
		//Limiting the number of orders.
		int numOrders = 2;
		vector<Customer *> customerList;

		for (int j = 0; j < number; j++) {
			vector<Order *> orderList =  Order::generateObjects(2 + (j%numOrders));

			Customer * t = new Customer(orderList, j, "Name-" + itos(j+1) , "Address-" + itos(j+2),  j+3, "Phone-" + itos(j+4), j+5.0, "MarketSegment-" + itos(j+6), "Comment-" + itos(j+8));
			customerList.push_back(t);
		}
		return customerList;
	}




	/*
	 * Functions related to Hand Coded Serialization.
	 */

	//Hand Coded C++ serialization:
	char * Customer::serialize(char* buffer, int& objectSize) {
		//First determine the size of object:
		//Integer elements:
		objectSize = 2 * sizeof(int);
		//Double elements:
		objectSize += 1 * sizeof(double);
		//String sizes: 
		objectSize += 5 * sizeof(int);
		//String Elements:
		objectSize += (name.length() + address.length() + phone.length() + mktsegment.length() + comment.length());		
		
		//Vector of orders:
		objectSize += sizeof(int);

		//Now, serialize the object.
		char * tempBuffer = buffer;
		
		//Copy Integers:
		tempBuffer = copyInt(tempBuffer, custKey);	
		tempBuffer = copyInt(tempBuffer, nationKey);	

		//Copy Doubles:		
		tempBuffer = copyDouble(tempBuffer, accbal);

		//Copy Strings:
		tempBuffer = copyString(tempBuffer, name);		
		tempBuffer = copyString(tempBuffer, address);
		tempBuffer = copyString(tempBuffer, phone);
		tempBuffer = copyString(tempBuffer, mktsegment);
		tempBuffer = copyString(tempBuffer, comment);


		//Copy encapsulated objects:
		int numOrders =  orders.size();
		tempBuffer = copyInt(tempBuffer, numOrders);	


		//New code:
		int orderSize;
		for (int i = 0; i < orders.size(); i++) {
			orderSize = 0;
			orders.at(i)->serialize(tempBuffer, orderSize);
			tempBuffer += orderSize;
			objectSize += orderSize;
		}

		return buffer;
	}


	int Customer::deserialize(char * buffer) {
		int bytesRead = 0;
		
		//Integers:
		custKey = parseInt(buffer+bytesRead); bytesRead += sizeof(custKey);	
		nationKey = parseInt(buffer+bytesRead); bytesRead += sizeof(nationKey);		

		//Doubles:
		accbal = parseDouble(buffer+bytesRead); bytesRead += sizeof(accbal);

		//Strings:
		parseString(buffer+bytesRead, name); bytesRead += (sizeof(int) + name.length());
		parseString(buffer+bytesRead, address); bytesRead += (sizeof(int) + address.length());
		parseString(buffer+bytesRead, phone); bytesRead += (sizeof(int) + phone.length());
		parseString(buffer+bytesRead, mktsegment); bytesRead += (sizeof(int) + mktsegment.length());
		parseString(buffer+bytesRead, comment); bytesRead += (sizeof(int) + comment.length());


		//Encapsulated objects:
		int numOrders = parseInt(buffer+bytesRead); bytesRead += sizeof(numOrders);	

		Order * tempOrder;
		for(int i = 0; i < numOrders; i++) {
			tempOrder = new Order();
			bytesRead += tempOrder->deserialize(buffer+bytesRead);
			orders.push_back(tempOrder);
		}

		return  bytesRead;
	}	




	//Create a inplace object:
	CustomerIP * Customer::createInPlaceObject() {
		return new CustomerIP(orders, orders.size(), custKey, name, address, nationKey, phone, accbal, mktsegment, comment);
	}


	//In place serialization:
	long Customer::serializeInPlace(char * fileStart) {
		//In place object:
		this->inplace = this->createInPlaceObject();	

		//Return pointer:
		return ((long)inplace - (long)fileStart);
	}