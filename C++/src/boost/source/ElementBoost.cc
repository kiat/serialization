#include<iostream>
#include<cstring>
#include <sstream>
#include <vector>
#include "ElementBoost.h"

using namespace std;

	//Default constructor:
	ElementBoost::ElementBoost() {	
	}

	//Constructor with arguments:
	ElementBoost::ElementBoost(int position, double value) {
		this->position = position;
		this->value = value;
	}


	//Getters and Setters Follow:
	int ElementBoost::getPosition() {
		return position;
	}

	double ElementBoost::getValue() {
		return value;
	}

	//C++: Explicit call needed for printing content:
	string ElementBoost::toString() {
		string stringS = "(" + itos(position) + "," + dtos(value) + ")\n";
		return stringS;
	}


	//Use for generating objects in bulk:
	vector<ElementBoost *> ElementBoost::generateObjects(int number) {
		vector<ElementBoost *> objectList;
		for (int i = 0; i < number; i++) {
			ElementBoost * tmp = new ElementBoost(i, 0.1*i);
			objectList.push_back(tmp);
		}
		return objectList;
	}


	//Implement your own custom comparator:
	bool ElementBoost::operator< (ElementBoost& other) {
		return position < other.position;	
	}


	//Boost serialization:
	char * ElementBoost::serialize(char * buffer, int &objectSize) {

		//Serialize:
		ElementBoost * T = this;

		//Serializer:
		stringstream ss;
		boost::archive::text_oarchive oa(ss, boost::archive::no_header);

		//Serialize:
		oa << T;

		//First determine the size of object:
		objectSize = ss.str().size() + sizeof(int);

		//Now, serialize the object.
		char * tempBuffer = buffer;
		
		//Copy String:
		tempBuffer = copyString(tempBuffer, ss.str());
		
		return buffer;
	}


	//Boost deserialization:
	void ElementBoost::deserializeBoost(char * buffer, ElementBoost *& boostObject) {
		//Use this subsequently:
		char* tempBuffer = buffer;
		
		//Parse "length" of the string.
		int sizeofString;
		memcpy(&sizeofString, tempBuffer, sizeof(int));
		tempBuffer += sizeof(int);


		//Create stream on heap: Keep stream alive:
		stringstream *rs = new stringstream();
		rs->write(tempBuffer, sizeofString);

		//Create archive on heap: Keep stream alive:
		boost::archive::text_iarchive * ia = new boost::archive::text_iarchive(*rs, boost::archive::no_header);
		(*ia) >> boostObject;

		//Assign the streams: 
		freeArchive = ia;
		freeStream = rs;
	}
