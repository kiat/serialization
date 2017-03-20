#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "ElementProto.h"

using namespace std;


	//Default constructor:
	ElementProto::ElementProto() {		
	}

	//Constructor with arguments:
	ElementProto::ElementProto(int position, double value) {
		proto = new proto::ElementP;
		proto->set_position(position);
		proto->set_value(value);

		//Proto object has been created:
		isProtoSerialized = true;
	}


	//Getters and Setters Follow:
	int ElementProto::getPosition() {
		return this->proto->position();	
	}

	double ElementProto::getValue() {
		return this->proto->value();
	}

	//C++: Explicit call needed for printing content:
	string ElementProto::toString() {
		string stringS = "(" + itos(getPosition()) + "," + dtos(getValue()) + ")\n";
		return stringS;
	}


	//Use for generating objects in bulk:
	vector<ElementProto *> ElementProto::generateObjects(int number) {
		vector<ElementProto *> objectList;
		for (int i = 0; i < number; i++) {
			ElementProto * tmp = new ElementProto(i, 0.1*i);
			objectList.push_back(tmp);
		}
		return objectList;
	}


	
	//Protocol buffer serialization:
	char * ElementProto::serialize(char * buffer, int &objectSize) {

		//First determine the size of protobuf object:
		objectSize = sizeof(int) + this->proto->ByteSize();

		//Now, serialize the object.	
		char * tempBuffer = buffer;
		
		//Copy total size. 
		//This is weird. Protobuf doesn't understand end of an object in a buffer!
		tempBuffer = copyInt(tempBuffer, objectSize);	

		//Standard protobuf serialization:
		this->proto->SerializeToArray(tempBuffer, objectSize - sizeof(int));
		
		return buffer;
	}


	//Protocol buffer de-serialization:		
	int ElementProto::deserialize(char * buffer) {
		
		int sizeofObject = 0;
		int bytesRead = 0;
		
		//Size of protobuf object:
		sizeofObject = parseInt(buffer+bytesRead);
		bytesRead += sizeof(int);	
		
		//Standard protobuf serialization:
		this->proto = new proto::ElementP;
		this->proto->ParseFromArray(buffer+bytesRead, sizeofObject - sizeof(int));

		//Memory Leak:
		isProtoSerialized = true;

		return sizeofObject;
	}


	//Implement your own custom comparator:
	bool ElementProto::operator< (ElementProto& other) {
		return getPosition() < other.getPosition();	
	}

