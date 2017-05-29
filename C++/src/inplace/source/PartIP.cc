#include "PartIP.h"
using namespace std;


	//Default constructor:
	PartIP::PartIP() {
		throw invalid_argument("PartIP: Not default constructible.");
	}

	//Copy constructor:
	PartIP::PartIP(const PartIP &obj) {
		throw invalid_argument("PartIP: Not copy constructible.");
	}

	//Constructor with arguments:
	PartIP::PartIP(int partKey, string name, string mfgr, string brand, string type, int size, string container, double retailPrice, string comment) {
		this->partKey = partKey;
		this->name = malloc <char> (strlen (name.c_str ()) + 1); strcpy (this->name, name.c_str ());
		this->mfgr = malloc <char> (strlen (mfgr.c_str ()) + 1); strcpy (this->mfgr, mfgr.c_str ());
		this->brand = malloc <char> (strlen (brand.c_str ()) + 1); strcpy (this->brand, brand.c_str ());
		this->type = malloc <char> (strlen (type.c_str ()) + 1); strcpy (this->type, type.c_str ());
		this->size = size;
		this->container = malloc <char> (strlen (container.c_str ()) + 1); strcpy (this->container, container.c_str ());
		this->retailPrice = retailPrice;
		this->comment = malloc <char> (strlen (comment.c_str ()) + 1); strcpy (this->comment, comment.c_str ());
	}

	//Getters and Setters Follow:
	int PartIP::getKey() {
		return partKey;
	}
	
	string PartIP::getName() {
		return string(name);
	}

	string PartIP::getBrand() {
		return string(brand);
	}

	string PartIP::getMfgr() {
		return string(mfgr);
	}

	string PartIP::getType() {
		return string(type);
	}

	int PartIP::getSize() {
		return size;
	}

	string PartIP::getContainer() {
		return string(container);
	}

	double PartIP::getRetailPrice() {
		return retailPrice;
	}

	string PartIP::getComment() {
		return string(comment);
	}
	
	//C++: Explicit call needed for printing content:
	string PartIP::toString() {
		string stringS =  to_string(this->getKey()) + " - " + this->getName() + " - " + this->getMfgr() + " - " + this->getBrand() + " - " + this->getType() + " - " + this->getComment() + " - " + this->getContainer()  + " - " + to_string(this->getRetailPrice()) + " - " + to_string(this->getSize());	  
		return stringS;
	}


	//Implement your own custom comparator:
	bool PartIP::operator< (PartIP& other) {	
		return (partKey < other.partKey);
	}
