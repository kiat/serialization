#ifndef CUSTOMERBOOST_H
#define CUSTOMERBOOST_H


#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "OrderBoost.h"
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;


	class CustomerBoost : public RootData {
		private:
			 vector<OrderBoost*> orders;
			 int custKey;
			 string name;
			 string address;
			 int nationKey;
			 string phone;
			 double accbal;
			 string mktsegment;
			 string comment;


			 //Memory Leak:
			 boost::archive::text_iarchive * freeArchive;
			 stringstream * freeStream;


			//Boost Serialization:
		    friend class boost::serialization::access;
		    
		    //Serialize method for boost:
		    template<class Archive>
		    void serialize(Archive & ar, const unsigned int version)
		    {
		        // Simply list all the fields to be serialized/deserialized.
				 ar & orders;
				 ar & custKey;
				 ar & name;
				 ar & address;
				 ar & nationKey;
				 ar & phone;
				 ar & accbal;
				 ar & mktsegment;
				 ar & comment;
		    }



		public:


			boost::archive::text_iarchive * getArchive() {
				return freeArchive;
			}

			stringstream* getStream(){
				return freeStream;
			}


			void freePointedObjects() {
				for (int i = 0; i < orders.size(); i++) {
					delete orders.at(i);
				}						
			}

			//Default destructor:
			~CustomerBoost() {
			}

			//Copy constructor:
			CustomerBoost(const CustomerBoost &obj);

			//Default constructor:
			CustomerBoost();

			//Constructor with arguments:
			CustomerBoost(vector<OrderBoost*> orders, int custKey, string name, string address, int nationKey, string phone, double accbal, string mktsegment, string comment);		
	

			//Basic setters and getters:
			vector<OrderBoost*> getOrderBoosts();
			void setOrderBoosts(vector<OrderBoost*> orders);
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
			static vector<CustomerBoost *> generateObjects(int number);

			//Boost serialization:
			char * serialize(char * buffer, int &objectSize);

			//Boost deserialization:
			void deserializeBoost(char *, CustomerBoost *&);


			//Implement your own custom comparator:
			bool operator< (CustomerBoost& other);	

	};




#endif