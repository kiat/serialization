#ifndef SUPPLIERIP_H
#define SUPPLIERIP_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "Object.h"
using namespace std;

class SupplierIP : public Object {

	private:
		int supplierKey;
		offset_ptr <char> name;
		offset_ptr <char> address;
		int nationKey;
		offset_ptr <char> phone;
		double accbal;
		offset_ptr <char> comment;

	public:	
		//Default constructor:
		SupplierIP();
		
		//Constructor with arguments:
		SupplierIP(int supplierKey, string name, string address, int nationKey, string phone, double accbal, string comment);


		//Basic setters and getters:
		int getSupplierKey();
	 	string getName();
		string getAddress();
		int getNationKey();	
		string getPhone(); 
		double getAccbal();
		string getComment();

		//C++: Use explicitly for printing.
		string toString();

		//Implement your own custom comparator:
		bool operator< (SupplierIP& other);

};

#endif