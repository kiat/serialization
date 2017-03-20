#include <iostream>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <new>
#include "ElementProto.h"
#include "PartProto.h"
#include "SupplierProto.h"
#include "LineItemProto.h"
#include "OrderProto.h"
#include "CustomerProto.h"

using namespace std;

int main() {

	char * buffer;
	int countObjects = 5;

	//Test ElementProto Class:
	cout << "Testing \"ElementProto\" Class.\n";

	//Create a list of objects:
	vector<ElementProto *> elementObjectList = ElementProto::generateObjects(countObjects);
	
	int objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		ElementProto * oldElement = elementObjectList.at(i);

		buffer = oldElement->serialize(objectSize);
		//cout << objectSize << endl;


		ElementProto * newElement = new ElementProto();
		newElement->deserialize(buffer);
		//cout << newElement->toString() << endl;

		if (!(oldElement->toString().compare(newElement->toString())==0)) {
			throw invalid_argument("Element: Serialized and De-serialized contents are different.");
		}

		//Release Memory:
		delete[] buffer;

		//Delete new object:
		delete newElement;
		delete oldElement;
	}
	elementObjectList.clear();




	//Test PartProto Class:
	cout << "Testing \"PartProto\" Class.\n";

	//Create a list of objects:
	vector<PartProto *> partObjectList = PartProto::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		PartProto * oldPart = partObjectList.at(i);

		buffer = oldPart->serialize(objectSize);
		//cout << objectSize << endl;


		PartProto * newPart = new PartProto();
		newPart->deserialize(buffer);
		//cout << newPart->toString() << endl;

		if (!(oldPart->toString().compare(newPart->toString())==0)) {
			throw invalid_argument("Part: Serialized and De-serialized contents are different.");
		}

		//Release Memory:
		delete[] buffer;

		//Delete new object:
		delete newPart;
		delete oldPart;
	}
	partObjectList.clear();




	//Test SupplierProto Class:
	cout << "Testing \"SupplierProto\" Class.\n";

	//Create a list of objects:
	vector<SupplierProto *> supplierObjectList = SupplierProto::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		SupplierProto * oldSupplier = supplierObjectList.at(i);

		buffer = oldSupplier->serialize(objectSize);
		//cout << objectSize << endl;


		SupplierProto * newSupplier = new SupplierProto();
		newSupplier->deserialize(buffer);
		//cout << newSupplier->toString() << endl;

		if (!(oldSupplier->toString().compare(newSupplier->toString())==0)) {
			throw invalid_argument("Supplier: Serialized and De-serialized contents are different.");
		}

		//Release Memory:
		delete[] buffer;

		//Delete new object:
		delete newSupplier;
		delete oldSupplier;
	}
	supplierObjectList.clear();



	//Test LineItemProto Class:
	cout << "Testing \"LineItemProto\" Class.\n";

	//Create a list of objects:
	vector<LineItemProto *> lineItemObjectList = LineItemProto::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		LineItemProto * oldLineItem = lineItemObjectList.at(i);

		buffer = oldLineItem->serialize(objectSize);
		cout << objectSize << endl;


		LineItemProto * newLineItem = new LineItemProto();
		newLineItem->deserialize(buffer);
		//cout << newLineItem->toString() << endl;

		if (!(oldLineItem->toString().compare(newLineItem->toString())==0)) {
			throw invalid_argument("LineItem: Serialized and De-serialized contents are different.");
		}

		//Release Memory:
		delete[] buffer;

		//Delete new object:
		delete newLineItem;
		delete oldLineItem;
	}
	lineItemObjectList.clear();



	//Test OrderProto Class:
	cout << "Testing \"OrderProto\" Class.\n";

	//Create a list of objects:
	vector<OrderProto *> orderObjectList = OrderProto::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		OrderProto * oldOrder = orderObjectList.at(i);

		buffer = oldOrder->serialize(objectSize);
		//cout << objectSize << endl;


		OrderProto * newOrder = new OrderProto();
		newOrder->deserialize(buffer);
		//cout << newOrder->toString() << endl;

		if (!(oldOrder->toString().compare(newOrder->toString())==0)) {
			throw invalid_argument("Order: Serialized and De-serialized contents are different.");
		}

		//Release Memory:
		delete[] buffer;

		//Delete new object:
		delete newOrder;
		delete oldOrder;
	}
	orderObjectList.clear();


	//Test CustomerProto Class:
	cout << "Testing \"CustomerProto\" Class.\n";

	//Create a list of objects:
	vector<CustomerProto *> CustomerObjectList = CustomerProto::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		CustomerProto * oldCustomer = CustomerObjectList.at(i);

		buffer = oldCustomer->serialize(objectSize);
		cout << objectSize << endl;


		CustomerProto * newCustomer = new CustomerProto();
		newCustomer->deserialize(buffer);
		//cout << newCustomer->toString() << endl;

		if (!(oldCustomer->toString().compare(newCustomer->toString())==0)) {
			throw invalid_argument("Customer: Serialized and De-serialized contents are different.");
		}

		//Release Memory:
		delete[] buffer;

		//Delete new object:
		delete newCustomer;
		delete oldCustomer;
	}
	CustomerObjectList.clear();

}