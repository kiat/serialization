#include "SupplierIP.h"
using namespace std;

	//Default constructor:
	SupplierIP::SupplierIP() {
		throw invalid_argument("SupplierIP: Not default constructible.");
	}

	//Constructor with arguments:
	SupplierIP::SupplierIP(int supplierKey, string name, string address, int nationKey, string phone, double accbal, string comment) {
		this->supplierKey = supplierKey;
		this->name = malloc <char> (strlen (name.c_str ()) + 1); strcpy (this->name, name.c_str ());
		this->address = malloc <char> (strlen (address.c_str ()) + 1); strcpy (this->address, address.c_str ());
		this->nationKey = nationKey;
		this->phone = malloc <char> (strlen (phone.c_str ()) + 1); strcpy (this->phone, phone.c_str ());
		this->accbal = accbal;
		this->comment = malloc <char> (strlen (comment.c_str ()) + 1); strcpy (this->comment, comment.c_str ());
	}


	//Getters and Setters Follow:
	int SupplierIP::getSupplierKey() {
		return supplierKey;
	}

	string SupplierIP::getName() {
		return string(name);
	}

	string SupplierIP::getAddress() {
		return string(address);
	}

	int SupplierIP::getNationKey() {
		return nationKey;
	}

	string SupplierIP::getPhone() {
		return string(phone);
	}

	double SupplierIP::getAccbal() {
		return accbal;
	}

	string SupplierIP::getComment() {
		return string(comment);
	}

	//C++: Explicit call needed for printing content:
	string SupplierIP::toString() {
		string stringS =  to_string(this->getSupplierKey()) + " - " + this->getName() + " - " + this->getAddress() + " - " + to_string(this->getNationKey()) + " - " +  this->getPhone() + " - " + to_string(this->getAccbal()) + " - " + this->getComment();	  
		return stringS;
	}	

	//Implement your own custom comparator:
	bool SupplierIP::operator< (SupplierIP& other) {	
		return (supplierKey < other.supplierKey);
	}
