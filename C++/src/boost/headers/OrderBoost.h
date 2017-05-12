#ifndef ORDERBOOST_H
#define ORDERBOOST_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "RootData.h"
#include "LineItemBoost.h"
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
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



	class OrderBoost : public RootData {

		private:
			vector<LineItemBoost*> lineItems;
			int orderKey;
			int custKey;
			string orderStatus;
			double totalPrice;
			string orderDate;
			string orderPriority;
			string clerk;
			int shipPriority;
			string comment;

			//Boost Serialization:
		    friend class boost::serialization::access;
		    
		    //Serialize method for boost:
		    template<class Archive>
		    void serialize(Archive & ar, const unsigned int version)
		    {
		        // Simply list all the fields to be serialized/deserialized.
				ar & lineItems;
				ar & orderKey;
				ar & custKey;
				ar & orderStatus;
				ar & totalPrice;
				ar & orderDate;
				ar & orderPriority;
				ar & clerk;
				ar & shipPriority;
				ar & comment;
		    }



		public:
			//Default destructor:
			~OrderBoost() {				
				// for (int i = 0; i < lineItems.size(); i++) {
				// 	delete lineItems.at(i);
				// }			
			}
						
			//Default constructor:
			OrderBoost();

			//Constructor with arguments:
			OrderBoost(vector<LineItemBoost*> lineItems, int orderkey, int custkey, string orderstatus, double totalprice, string orderdate, string orderpriority, string clerk, int shippriority, string comment);


			//Basic setters and getters:
			vector<LineItemBoost*> getLineItems();
			void setLineItems(vector<LineItemBoost*> lineItems);
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
			static vector<OrderBoost *> generateObjects(int number);
			

			//Boost serialization:
			char * serialize(char * buffer, int &objectSize);
			
			//Boost deserialization:
			//OrderBoost* deserialize(char *);

			//Implement your own custom comparator:
			bool operator< (OrderBoost& other);


	};







#endif