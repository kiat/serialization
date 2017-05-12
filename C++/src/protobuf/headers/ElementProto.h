#ifndef ELEMENTPROTO_H
#define ELEMENTPROTO_H

#include <vector>
#include "RootData.h"
#include "ElementP.pb.h"
using namespace std;


	class ElementProto : public RootData {

		private:
			//For protocol buffers:
			bool isProtoSerialized;
			proto::ElementP * proto;

		public:
			//Default destructor:
			~ElementProto() {
				if(isProtoSerialized) {
					delete proto;
				}	
			}
					
			//Default constructor:
			ElementProto();

			//Constructor with arguments:
			ElementProto(int position, double value);

			//Basic setters and getters:
			int getPosition();
			double getValue();


			//C++: Use explicitly for printing.
			string toString();
			
			//Used to generate objects:
			static vector<ElementProto *> generateObjects(int number);
			

			//Protocol buffer serialization:
			char * serialize(char * buffer, int &objectSize);
			//Protocol buffer de-serialization:		
			int deserialize(char * buffer);
			
			//Implement your own custom comparator:
			bool operator< (ElementProto& other);
	};



#endif