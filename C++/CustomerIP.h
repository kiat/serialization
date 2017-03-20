#ifndef CUSTOMERIP_H
#define CUSTOMERIP_H


#include<iostream>
#include<cstring>
#include <sstream>
#include "Object.h"
#include "OrderIP.h"
#include "Order.h"
using namespace std;


	class CustomerIP : public Object {
		private:
			 offset_ptr <OrderIP> orders;
			 int numOrderSupplied;
			 int custKey;
			 offset_ptr <char> name;
			 offset_ptr <char> address;
			 int nationKey;
			 offset_ptr <char> phone;
			 double accbal;
			 offset_ptr <char> mktsegment;
			 offset_ptr <char> comment;

		public:
			//Default constructor:
			CustomerIP();
			
			//Copy constructor:
			CustomerIP(const CustomerIP &obj);
			
			//Constructor with arguments:
			CustomerIP(vector<Order*> orders, int numOrderSupplied, int custKey, string name, string address, int nationKey, string phone, double accbal, string mktsegment, string comment);		
	
			//Basic getters:
			OrderIP* getOrders();
			int getOrderSuppliedCount();
			int getCustKey();			
			string getName();			
			string getAddress();			
			int getNationKey();			
			string getPhone();			
			double getAccbal();			
			string getMktsegment();			
			string getComment();	

			//C++: Use explicitly for printing.
			string toString();
			
			//Implement your own custom comparator:
			bool operator< (CustomerIP& other);	
	};

#endif