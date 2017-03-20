#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "CustomerBoost.h"
#include "OrderBoost.h"
using namespace std;

	//Copy constructor:
	CustomerBoost::CustomerBoost(const CustomerBoost &obj) {
		throw invalid_argument("CustomerBoost: Not copy constructible.");
	}
	
	//Default constructor:
	CustomerBoost::CustomerBoost() {			
	}


	//Constructor with arguments:
	CustomerBoost::CustomerBoost(vector<OrderBoost*> orders, int custKey, string name, string address, int nationKey, string phone, double accbal, string mktsegment, string comment) {
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
	vector<OrderBoost*> CustomerBoost::getOrderBoosts() {
		return orders;
	}

	void CustomerBoost::setOrderBoosts(vector<OrderBoost*> orders) {
		this->orders = orders;
	}

	int CustomerBoost::getCustKey() {
		return custKey;
	}

	void CustomerBoost::setCustKey(int custKey) {
		this->custKey = custKey;
	}

	string CustomerBoost::getName() {
		return name;
	}

	void CustomerBoost::setName(string name) {
		this->name = name;
	}

	string CustomerBoost::getAddress() {
		return address;
	}

	void CustomerBoost::setAddress(string address) {
		this->address = address;
	}

	int CustomerBoost::getNationKey() {
		return nationKey;
	}

	void CustomerBoost::setNationKey(int nationKey) {
		this->nationKey = nationKey;
	}

	string CustomerBoost::getPhone() {
		return phone;
	}

	void CustomerBoost::setPhone(string phone) {
		this->phone = phone;
	}


	double CustomerBoost::getAccbal() {
		return accbal;
	}

	void CustomerBoost::setAccbal(double accbal) {
		this->accbal = accbal;
	}

	string CustomerBoost::getMktsegment() {
		return mktsegment;
	}

	void CustomerBoost::setMktsegment(string mktsegment) {
		this->mktsegment = mktsegment;
	}

	string CustomerBoost::getComment() {
		return comment;
	}

	void CustomerBoost::setComment(string comment) {
		this->comment = comment;
	}			



	//C++: Explicit call needed for printing content:
	string CustomerBoost::toString() {
		string stringS = itos(this->getCustKey())  + " - " + this->getName() + " - " + this->getAddress() +  " - "  + itos(this->getNationKey())   + " - " + this->getPhone() + " - " + dtos(this->getAccbal())  + " - " + this->getMktsegment() + " - " + this->getComment();
		
		//All orders:
		for (int i = 0; i < orders.size(); i++) {
			stringS += " OrderBoost " + itos(i) + " : " + orders.at(i)->toString();
		}

		return stringS;
	}



	//Use for generating objects in bulk:
	vector<CustomerBoost *> CustomerBoost::generateObjects(int number) {
		//Limiting the number of orders.
		int numOrderBoosts = 2;
		vector<CustomerBoost *> customerList;

		for (int j = 0; j < number; j++) {
			vector<OrderBoost *> orderList =  OrderBoost::generateObjects(2 + (j%numOrderBoosts));

			CustomerBoost * t = new CustomerBoost(orderList, j, "Name-" + itos(j+1) , "Address-" + itos(j+2),  j+3, "Phone-" + itos(j+4), j+5.0, "MarketSegment-" + itos(j+6), "Comment-" + itos(j+8));
			customerList.push_back(t);
		}
		return customerList;
	}


	//Boost serialization:
	char * CustomerBoost::serialize(char * buffer, int& objectSize) {
		//Serialize:
		CustomerBoost * T = this;

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
	void CustomerBoost::deserializeBoost(char * buffer, CustomerBoost *& boostObject) {
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
