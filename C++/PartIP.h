#ifndef PARTIP_H
#define PARTIP_H


#include <iostream>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <vector>
#include "Object.h"
using namespace std;


class PartIP : public Object {

	private:
		int partKey;
		offset_ptr <char> name;
		offset_ptr <char> mfgr;
		offset_ptr <char> brand;
		offset_ptr <char> type;
		int size;
		offset_ptr <char> container;
		double retailPrice;
		offset_ptr <char> comment;

	public:	

	//Default constructor:
	PartIP();

	//Copy constructor:
	PartIP(const PartIP &obj);

	//Constructor with arguments:
	PartIP(int partKey, string name, string mfgr, string brand, string type, int size, string container, double retailPrice, string comment);
	
	//Basic setters and getters:
	int getKey();
	string getName();
	string getBrand();
	string getMfgr();
	string getType();
	int getSize();
	string getContainer();
	double getRetailPrice();
	string getComment();

	//C++: Use explicitly for printing.
	string toString();
	
	//Implement your own custom comparator:
	bool operator< (PartIP& other);
};	

#endif
