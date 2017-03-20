#ifndef ORDER_H
#define ORDER_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "RootData.h"
#include "LineItem.h"
#include "OrderIP.h"

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



	class Order : public RootData {

		private:
			vector<LineItem*> lineItems;
			int orderKey;
			int custKey;
			string orderStatus;
			double totalPrice;
			string orderDate;
			string orderPriority;
			string clerk;
			int shipPriority;
			string comment;

			//For in place objects:
			OrderIP * inplace;

			//From the normal C++ object, create a protocol buffer object.
			OrderIP * createInPlaceObject();	

		public:
	
			//Default destructor:
			~Order() {
				for (int i = 0; i < lineItems.size(); i++) {
					delete lineItems.at(i);
				}				
			}
						
			//Default constructor:
			Order();

			//Constructor with arguments:
			Order(vector<LineItem*> lineItems, int orderkey, int custkey, string orderstatus, double totalprice, string orderdate, string orderpriority, string clerk, int shippriority, string comment);


			//Basic setters and getters:
			vector<LineItem*> getLineItems();
			void setLineItems(vector<LineItem*> lineItems);
			int getOrderKey();
			void setOrderKey(int orderKey);
			int getCustKey();
			void setCustKey(int custKey);
			double getTotalPrice();
			void setTotalPrice(double totalPrice);
			string getOrderStatus();
			void setOrderStatus(string orderStatus);
			string getOrderDate();
			void setOrderDate(string orderDate);

			string getOrderPriority();
			void setOrderPriority(string orderPriority);
			string getClerk();
			void setClerk(string clerk);

			int getShipPriority();
			void setShipPriority(int shipPriority);
			string getComment();
			void setComment(string comment);


			//C++: Use explicitly for printing.
			string toString();
			
			//Used to generate objects:
			static vector<Order *> generateObjects(int number);
			
			//Hand Coded C++ serialization:	
			//New API: Writes directly to File Page:
			char * serialize(char* buffer, int &objectSize);


			//Hand Coded C++ de-serialization:		
			int deserialize(char * buffer) ;


			//Implement your own custom comparator:
			bool operator< (Order& other);


			//In place serialization:
			long serializeInPlace(char * fileStart);
	};

#endif