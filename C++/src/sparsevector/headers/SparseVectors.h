#ifndef SPARSEVECTORS_H
#define SPARSEVECTORS_H


#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>


#include "Element.h"
#include "ElementProto.h"
#include "ElementIP.h"
#include "ElementBoost.h"

using namespace std;

class SparseVectors {
public:

	//Default Constructor:
	SparseVectors() {
	}
	~SparseVectors();

	void setUp(int type) {
		serializationType = type;
	}

	void generateSparseVector(int count, int dimensions, int serializationType);

	
	void aggergateSparseVector(SparseVectors * vectorOne, SparseVectors * vectorTwo, SparseVectors * vectorThree);

	int getNextNetworkPage(int startIndex, char * networkBuffer);

	void parseNetworkPage(char * networkBuffer);


	int getPosition(int i) {
		switch(serializationType) {
			case 0:
				return elementHandCodedObjects.at(i)->getPosition();
				break;
			case 1:
				return elementProtoObjects.at(i)->getPosition();
				break;
			case 2:
				return elementInPlaceObjects.at(i)->getPosition();
				break;
			case 3:
				return elementBoostObjects.at(i)->getPosition();
				break;
			default: 
				break;
		}
	}

	double getValue(int i) {
		switch(serializationType) {
			case 0:
				return elementHandCodedObjects.at(i)->getValue();
				break;
			case 1:
				return elementProtoObjects.at(i)->getValue();
				break;
			case 2:
				return elementInPlaceObjects.at(i)->getValue();
				break;
			case 3:
				return elementBoostObjects.at(i)->getValue();
				break;
			default: 
				break;
		}
	}


	void addElement(int position, double value) {
		Element * temp;
		ElementIP * tempIP;
		ElementProto * tempProto;
		ElementBoost * tempBoost;	
			
		switch(serializationType) {
			case 0:
				temp = new Element(position, value);
				elementHandCodedObjects.push_back(temp);
				break;
			case 1:
				tempProto = new ElementProto(position, value);
				elementProtoObjects.push_back(tempProto);
				break;
			case 2:
				tempIP = new ElementIP(position, value);
				elementInPlaceObjects.push_back(tempIP);
				break;

			case 3:
				tempBoost = new ElementBoost(position, value);
				elementBoostObjects.push_back(tempBoost);
				break;
			default: break;
		}
	}


	// void setSerializationType(int type) {
	// 	this->serializationType = type;
	// }

	int getSerializationType() {
		return serializationType;
	}

	int getSize() {
		switch(serializationType) {
			case 0:
				return elementHandCodedObjects.size();
				break;
			case 1:
				return elementProtoObjects.size();
				break;
			case 2:
				return elementInPlaceObjects.size();
				break;
			case 3:
				return elementBoostObjects.size();
				break;
			default: 
				break;
		}
	}


	void printInfo() {
		cout << "printInfo:: " << endl;
		cout << "serializationType:: " << serializationType << endl;
		
		for (int i = 0; i < elementHandCodedObjects.size(); i++) {
			cout << elementHandCodedObjects.at(i)->toString() << endl;
		}

		for (int i = 0; i < elementProtoObjects.size(); i++) {
			cout << elementProtoObjects.at(i)->toString() << endl;
		}

		for (int i = 0; i < elementInPlaceObjects.size(); i++) {
			cout << elementInPlaceObjects.at(i)->toString() << endl;
		}

		for (int i = 0; i < elementBoostObjects.size(); i++) {
			cout << elementBoostObjects.at(i)->toString() << endl;
		}		

	}


private:
	int serializationType = -1;
	int count;
	int dimensions;
	vector<Element *> elementHandCodedObjects;

	vector<ElementProto *> elementProtoObjects;

	char * inPlaceBuffer;
	vector<ElementIP *> elementInPlaceObjects;
	
	vector<ElementBoost *> elementBoostObjects;

	vector<char *> archiveInPlaceBuffer; 

};


#endif