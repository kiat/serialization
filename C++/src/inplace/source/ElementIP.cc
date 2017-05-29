#include "ElementIP.h"
using namespace std;


	//Default constructor:
	ElementIP::ElementIP() {
		throw invalid_argument("ElementIP: Not default constructible.");
	}


	//Constructor with arguments:
	ElementIP::ElementIP(int position, double value) {
		this->position = position;
		this->value = value;
	}

	//Getters and Setters Follow:
	int ElementIP::getPosition() {
		return position;
	}

	double ElementIP::getValue() {
		return value;
	}

	//C++: Explicit call needed for printing content:
	string ElementIP::toString() {
		string stringS = "(" + to_string(position) + "," + to_string(value) + ")\n";
		return stringS;
	}

	
