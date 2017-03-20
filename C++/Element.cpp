#include<iostream>
#include<cstring>
#include <vector>
#include "Element.h"

using namespace std;


	//Default constructor:
	Element::Element() {
	}

	//Constructor with arguments:
	Element::Element(int position, double value) {
		this->position = position;
		this->value = value;
	}


	//Getters and Setters Follow:
	int Element::getPosition() {
		return position;
	}

	double Element::getValue() {
		return value;
	}

	//C++: Explicit call needed for printing content:
	string Element::toString() {
		string stringS = "(" + itos(position) + "," + dtos(value) + ")\n";
		return stringS;
	}


	//Use for generating objects in bulk:
	vector<Element *> Element::generateObjects(int number) {
		vector<Element *> objectList;
		for (int i = 0; i < number; i++) {
			Element * tmp = new Element(i, 0.1*i);
			objectList.push_back(tmp);
		}
		return objectList;
	}

	//Hand Coded C++ serialization:
	char * Element::serialize(char* buffer, int &objectSize) {

		//First determine the size of object:
		objectSize = sizeof(int) + sizeof(double);

		//Now, serialize the object.
		char * tempBuffer = buffer;
		
		//Copy Integer:
		tempBuffer = copyInt(tempBuffer, position);
		//Copy Double:	
		tempBuffer = copyDouble(tempBuffer, value);	

		
		return buffer;
	}



	//Hand Coded C++ de-serialization:
	int Element::deserialize(char * buffer) {
		int bytesRead = 0;

		//Parse Integer:
		position = parseInt(buffer+bytesRead);
		bytesRead += sizeof(position);	
		
		//Parse Double:
		value = parseDouble(buffer+bytesRead);
		bytesRead += sizeof(value);		
		
		return  bytesRead;
	}	
	


	//Implement your own custom comparator:
	bool Element::operator< (Element& other) {
		return position < other.position;	
	}





	//Create a inplace object:
	ElementIP * Element::createInPlaceObject() {
		return new ElementIP(position, value);			
	}

	//In place serialization:
	long Element::serializeInPlace(char * fileStart) {
		//In place object:
		this->inplace = this->createInPlaceObject();

		//Return pointer:
		return ((long)inplace - (long)fileStart);
	}