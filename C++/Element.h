#ifndef ELEMENT_H
#define ELEMENT_H


#include <vector>
#include "RootData.h"
#include "ElementIP.h"
using namespace std;


	class Element : public RootData {

		private:
			int position;
			double value;


			//For in place objects:
			ElementIP * inplace;

			//From the normal C++ object, create a protocol buffer object.
			ElementIP * createInPlaceObject();	
					

		public:

			//Default destructor:
			~Element() {
			}
					
			//Default constructor:
			Element();

			//Constructor with arguments:
			Element(int position, double value);

			//Basic setters and getters:
			int getPosition();
			double getValue();

			//C++: Use explicitly for printing.
			string toString();
			
			//Used to generate objects:
			static vector<Element *> generateObjects(int number);
			
			//Hand Coded C++ serialization:
			//New API: Writes directly to File Page:
			char * serialize(char* buffer, int &objectSize);



			//Hand Coded C++ de-serialization:		
			int deserialize(char * buffer) ;

			//Implement your own custom comparator:
			bool operator< (Element& other);


			//In place serialization: Requires in place file start:
			long serializeInPlace(char * fileStart);
	};



#endif