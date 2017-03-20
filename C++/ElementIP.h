#ifndef ELEMENTIP_H
#define ELEMENTIP_H

#include "Object.h"
using namespace std;


	class ElementIP : public Object {

		private:
			int position;
			double value;

		public:

			//Default constructor:
			ElementIP();

			//Constructor with arguments:
			ElementIP(int position, double value);

			//Basic setters and getters:
			int getPosition();
			double getValue();

			//C++: Use explicitly for printing.
			string toString();
			
			int getSizeObject();
	};

#endif