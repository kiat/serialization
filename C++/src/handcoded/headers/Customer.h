#ifndef CUSTOMER_H
#define CUSTOMER_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "Order.h"
#include "CustomerIP.h"

using namespace std;


	class Customer : public RootData {
		private:
			 vector<Order*> orders;
			 int custKey;
			 string name;
			 string address;
			 int nationKey;
			 string phone;
			 double accbal;
			 string mktsegment;
			 string comment;


			//For in place objects:
			CustomerIP * inplace;

			//From the normal C++ object, create a protocol buffer object.
			CustomerIP * createInPlaceObject();	

		public:

			//Default destructor:
			~Customer() {
				for (int i = 0; i < orders.size(); i++) {
					delete orders.at(i);
				}
			}

			//Copy constructor:
			Customer(const Customer &obj);

			//Default constructor:
			Customer();

			//Constructor with arguments:
			Customer(vector<Order*> orders, int custKey, string name, string address, int nationKey, string phone, double accbal, string mktsegment, string comment);		
	

			//Basic setters and getters:
			vector<Order*> getOrders();
			void setOrders(vector<Order*> orders);
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
			static vector<Customer *> generateObjects(int number);
			
			//Hand Coded C++ serialization:
			//New API: Writes directly to File Page:
			char * serialize(char* buffer, int &objectSize);


			//Hand Coded C++ de-serialization:		
			int deserialize(char * buffer) ;

			//Implement your own custom comparator:
			bool operator< (Customer& other);	


			//In place serialization:
			long serializeInPlace(char * fileStart);
	};




#endif