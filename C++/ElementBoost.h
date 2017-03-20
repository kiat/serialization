#ifndef ELEMENTBOOST_H
#define ELEMENTBOOST_H


#include <vector>
#include "RootData.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;


	class ElementBoost : public RootData {

		private:
			int position;
			double value;

			 //Memory Leak:
			 boost::archive::text_iarchive * freeArchive;
			 stringstream * freeStream;

			//Boost Serializtion:
		    friend class boost::serialization::access;
		    
		    //Serialize method for boost:
		    template<class Archive>
		    void serialize(Archive & ar, const unsigned int version)
		    {
		        // Simply list all the fields to be serialized/deserialized.
		        ar & position;
		        ar & value; 
		    }


		public:

			boost::archive::text_iarchive * getArchive() {
				return freeArchive;
			}

			stringstream* getStream(){
				return freeStream;
			}

			void freePointedObjects() {					
			}

			//Default destructor:
			~ElementBoost() {
			}
					
			//Default constructor:
			ElementBoost();

			//Constructor with arguments:
			ElementBoost(int position, double value);

			//Basic setters and getters:
			int getPosition();
			double getValue();


			//C++: Use explicitly for printing.
			string toString();
			
			//Used to generate objects:
			static vector<ElementBoost *> generateObjects(int number);
			
			//Boost serialization:
			//New API:
			char * serialize(char * buffer, int &objectSize);

			//Boost deserialization:
			void deserializeBoost(char *, ElementBoost *&);

			//Implement your own custom comparator:
			bool operator< (ElementBoost& other);

	};



#endif