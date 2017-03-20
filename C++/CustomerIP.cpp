#include "CustomerIP.h"
using namespace std;

	//Default constructor:
	CustomerIP::CustomerIP() {
		throw invalid_argument("CustomerIP: Not default constructible.");
	}

	//Copy constructor:
	CustomerIP::CustomerIP(const CustomerIP &obj) {
		throw invalid_argument("CustomerIP: Not copy constructible.");
	}		

	//Constructor with arguments:
	CustomerIP::CustomerIP(vector<Order*> orders, int numOrderSupplied, int custKey, string name, string address, int nationKey, string phone, double accbal, string mktsegment, string comment) {
		this->orders = new OrderIP [numOrderSupplied];
		for (int i = 0; i < numOrderSupplied; i++) {
			(this->orders)[i].setUp(orders.at(i)->getLineItems(), orders.at(i)->getLineItems().size(), orders.at(i)->getOrderKey(), orders.at(i)->getCustKey(), orders.at(i)->getOrderStatus(), orders.at(i)->getTotalPrice(), orders.at(i)->getOrderDate(), orders.at(i)->getOrderPriority(), orders.at(i)->getClerk(), orders.at(i)->getShipPriority(), orders.at(i)->getComment());
		}

		// this->orders = orders;
		this->numOrderSupplied = numOrderSupplied;
		this->custKey = custKey;
		this->name = malloc <char> (strlen (name.c_str ()) + 1);	strcpy (this->name, name.c_str ());
		this->address = malloc <char> (strlen (address.c_str ()) + 1); strcpy (this->address, address.c_str ());
		this->nationKey = nationKey;
		this->phone = malloc <char> (strlen (phone.c_str ()) + 1); strcpy (this->phone, phone.c_str ());
		this->accbal = accbal;
		this->mktsegment = malloc <char> (strlen (mktsegment.c_str ()) + 1); strcpy (this->mktsegment, mktsegment.c_str ());
		this->comment = malloc <char> (strlen (comment.c_str ()) + 1); strcpy (this->comment, comment.c_str ());
	}


	//Getters and Setters Follow:
	OrderIP* CustomerIP::getOrders() {
		return orders;
	}

	int CustomerIP::getOrderSuppliedCount(){
		return numOrderSupplied;
	}

	int CustomerIP::getCustKey() {
		return custKey;
	}

	string CustomerIP::getName() {
		return string(name);
	}

	string CustomerIP::getAddress() {
		return string(address);
	}

	int CustomerIP::getNationKey() {
		return nationKey;
	}

	string CustomerIP::getPhone() {
		return string(phone);
	}

	double CustomerIP::getAccbal() {
		return accbal;
	}

	string CustomerIP::getMktsegment() {
		return string(mktsegment);
	}

	string CustomerIP::getComment() {
		return string(comment);
	}

	//C++: Explicit call needed for printing content:
	string CustomerIP::toString() {
		string stringS = to_string(this->getCustKey())  + " - " + this->getName() + " - " + this->getAddress() +  " - "  + to_string(this->getNationKey())   + " - " + this->getPhone() + " - " + to_string(this->getAccbal())  + " - " + this->getMktsegment() + " - " + this->getComment();
		
		//All orders:
		for (int i = 0; i < numOrderSupplied; i++) {
			stringS += " Order " + to_string(i) + " : " + orders[i].toString();
		}

		return stringS;
	}


	//Implement your own custom comparator:
	bool CustomerIP::operator< (CustomerIP& other) {
		if (custKey == other.custKey) {
			bool result = false;

			if (getOrderSuppliedCount() == other.getOrderSuppliedCount()) {
				for (int i = 0; i < getOrderSuppliedCount(); i++) {
					result = orders[i]< (other.getOrders()[i]);
					if (result) return true;
				}
			}

			return getOrderSuppliedCount() < other.getOrderSuppliedCount();		
		}

		return custKey < other.custKey;	
	}
