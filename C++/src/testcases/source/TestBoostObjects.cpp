#include <iostream>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <new>
#include "ElementBoost.h"
#include "PartBoost.h"
#include "SupplierBoost.h"

using namespace std;

int main() {

	char * buffer;
	int countObjects = 20;

	//Test ElementBoost Class:
	cout << "Testing \"ElementBoost\" Class.\n";

	//Create a list of objects:
	vector<ElementBoost *> elementObjectList = ElementBoost::generateObjects(countObjects);
	
	int objectSize = 0;
	for (int i = 0; i < countObjects; i++) {
		ElementBoost * oldElement = elementObjectList.at(i);
		buffer = oldElement->serialize(objectSize);

		ElementBoost * newElement = new ElementBoost();
		ElementBoost * T = newElement->deserialize(buffer);


		if (!(oldElement->toString().compare(T->toString())==0)) {
			throw invalid_argument("Element: Serialized and De-serialized contents are different.");
		}


		//Release Memory:
		delete[] buffer;
		delete T;

		//Delete new object:
		delete newElement;
		delete oldElement;
	}
	elementObjectList.clear();



	//Test PartBoost Class:
	cout << "Testing \"PartBoost\" Class.\n";

	//Create a list of objects:
	vector<PartBoost *> partObjectList = PartBoost::generateObjects(countObjects);
	
	objectSize = 0;
	for (int i = 0; i < countObjects; i++) {
		PartBoost * oldPart = partObjectList.at(i);
		buffer = oldPart->serialize(objectSize);

		//cout << objectSize << endl;
		//cout << "OLD: " << oldPart->toString() << endl;

		PartBoost * newPart = new PartBoost();
		PartBoost * T = newPart->deserialize(buffer);

		//cout << "NEW: "  << T->toString() << endl;

		if (!(oldPart->toString().compare(T->toString())==0)) {
			throw invalid_argument("Part: Serialized and De-serialized contents are different.");
		}


		//Release Memory:
		delete[] buffer;
		delete T;

		//Delete new object:
		delete newPart;
		delete oldPart;
	}
	partObjectList.clear();


	//Test SupplierBoost Class:
	cout << "Testing \"SupplierBoost\" Class.\n";

	//Create a list of objects:
	vector<SupplierBoost *> supplierObjectList = SupplierBoost::generateObjects(countObjects);
	
	objectSize = 0;
	for (int i = 0; i < countObjects; i++) {
		SupplierBoost * oldSupplier = supplierObjectList.at(i);
		buffer = oldSupplier->serialize(objectSize);

		//cout << objectSize << endl;
		//cout << "OLD: " << oldSupplier->toString() << endl;

		SupplierBoost * newSupplier = new SupplierBoost();
		SupplierBoost * T = newSupplier->deserialize(buffer);

		//cout << "NEW: "  << T->toString() << endl;

		if (!(oldSupplier->toString().compare(T->toString())==0)) {
			throw invalid_argument("Supplier: Serialized and De-serialized contents are different.");
		}


		//Release Memory:
		delete[] buffer;
		delete T;

		//Delete new object:
		delete newSupplier;
		delete oldSupplier;
	}
	supplierObjectList.clear();

}