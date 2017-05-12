#ifndef ORDERIP_H
#define ORDERIP_H


#include<iostream>
#include<cstring>
#include <sstream>
#include "Object.h"
#include "LineItemIP.h"
#include "LineItem.h"
using namespace std;


	class OrderIP : public Object {

		private:
			offset_ptr <LineItemIP> lineItems;
			int numLineItemsSupplied;
			int orderKey;
			int custKey;
			offset_ptr <char> orderStatus;
			double totalPrice;
			offset_ptr <char> orderDate;
			offset_ptr <char> orderPriority;
			offset_ptr <char> clerk;
			int shipPriority;
			offset_ptr <char> comment;

		public:
			//Default constructor:
			OrderIP();
			
			//Constructor with arguments:
			OrderIP(vector<LineItem*> lineItems, int numLineItemsSupplied, int orderkey, int custkey, string orderstatus, double totalprice, string orderdate, string orderpriority, string clerk, int shippriority, string comment);


			// To be used alongside default constructor:
			void setUp(vector<LineItem*> lineItems, int numLineItemsSupplied, int orderkey, int custkey, string orderstatus, double totalprice, string orderdate, string orderpriority, string clerk, int shippriority, string comment);


			//Basic setters and getters:
			LineItemIP * getLineItems();
			int getLineItemsCount();
			int getOrderKey();
			int getCustKey();
			double getTotalPrice();
			string getOrderStatus();
			string getOrderDate();
			string getOrderPriority();
			string getClerk();
			int getShipPriority();
			string getComment();

			//C++: Use explicitly for printing.
			string toString();


			//Implement your own custom comparator:
			bool operator< (OrderIP& other);
	};

#endif