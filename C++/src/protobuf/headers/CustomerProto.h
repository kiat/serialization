#ifndef CUSTOMERPROTO_H
#define CUSTOMERPROTO_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "OrderProto.h"
#include "CustomerP.pb.h"

using namespace std;


	class CustomerProto : public RootData {
		private:
			//For protocol buffers:
			bool isProtoSerialized;
			proto::CustomerP * proto;

		public:
			//Default destructor:
			~CustomerProto() {
				if(isProtoSerialized) {
					delete proto;
				}
			}

			//Copy constructor:
			CustomerProto(const CustomerProto &obj);

			//Default constructor:
			CustomerProto();

			//Constructor with arguments:
			CustomerProto(vector<OrderProto*> orders, int custKey, string name, string address, int nationKey, string phone, double accbal, string mktsegment, string comment);		
	

			//Basic setters and getters:
			proto::CustomerP_OrderP * getOrders(int index);
			int getOrdersCount();

			int getCustKey();
			void setCustKey(int custKey);
			string getName();
			void setName(string name);
			string getAddress();
			void setAddress(string address);
			int getNationKey();
			void setNationKey(int nationKey);
			string getPhone();
			void setPhone(string phone);
			double getAccbal() ;
			void setAccbal(double accbal) ;
			string getMktsegment();
			void setMktsegment(string mktsegment);
			string getComment();
			void setComment(string comment);



			//C++: Use explicitly for printing.
			string toString();
			
			//Used to generate objects:
			static vector<CustomerProto *> generateObjects(int number);
			
			//Protocol buffer serialization:
			char * serialize(char * buffer, int& objectSize);
			//Protocol buffer de-serialization:		
			int deserialize(char * buffer) ;	

			//Implement your own custom comparator:
			bool operator< (CustomerProto& other);	


			//Internal comparators:
			bool LineItemComparator(proto::CustomerP_OrderP_LineItemP *a, proto::CustomerP_OrderP_LineItemP *b);
			bool OrderComparator(proto::CustomerP_OrderP *a, proto::CustomerP_OrderP *b);
	};




#endif