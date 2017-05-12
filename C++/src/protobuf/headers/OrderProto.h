#ifndef ORDERPROTO_H
#define ORDERPROTO_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "RootData.h"
#include "LineItemProto.h"
#include "OrderP.pb.h"
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



	class OrderProto : public RootData {

		private:
			//For protocol buffers:
			bool isProtoSerialized;			
			proto::OrderP * proto;

		public:
		
			//Default destructor:
			~OrderProto() {		
				if(isProtoSerialized) {
					delete proto;
				}	
			}
						
			//Default constructor:
			OrderProto();

			//Constructor with arguments:
			OrderProto(vector<LineItemProto*> lineItems, int orderkey, int custkey, string orderstatus, double totalprice, string orderdate, string orderpriority, string clerk, int shippriority, string comment);

			//Basic setters and getters:
			proto::OrderP_LineItemP * getLineItems(int index);
			int getLineItemsCount();


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
			static vector<OrderProto *> generateObjects(int number);
			

			//Protocol buffer serialization:
			char * serialize(char * buffer, int &objectSize);
			//Protocol buffer de-serialization:		
			int deserialize(char * buffer);

			//Implement your own custom comparator:
			bool operator< (OrderProto& other);

			//Duplicate: Internal LineItem comparator:
			bool LineItemComparator(proto::OrderP_LineItemP *a, proto::OrderP_LineItemP *b);
	};



#endif