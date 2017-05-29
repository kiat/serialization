#include <iostream>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <new>
#include "Element.h"
#include "Part.h"
#include "Supplier.h"
#include "LineItem.h"
#include "Order.h"
#include "Customer.h"
using namespace std;

int main() {

	char * buffer;
	int countObjects = 5;

	//Test Element Class:
	cout << "Testing \"Element\" Class.\n";

	//Create a list of objects:
	vector<Element *> elementObjectList = Element::generateObjects(countObjects);
	
	int objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		Element * oldElement = elementObjectList.at(i);

		buffer = oldElement->serialize(objectSize);

		Element * newElement = new Element();
		newElement->deserialize(buffer);

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




	//Test Part Class:
	cout << "Testing \"Part\" Class.\n";

	//Create a list of objects:
	vector<Part *> partObjectList = Part::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		Part * oldPart = partObjectList.at(i);

		buffer = oldPart->serialize(objectSize);

		Part * newPart = new Part();
		newPart->deserialize(buffer);

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



	//Test Supplier Class:
	cout << "Testing \"Supplier\" Class.\n";

	//Create a list of objects:
	vector<Supplier *> supplierObjectList = Supplier::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		Supplier * oldSupplier = supplierObjectList.at(i);

		buffer = oldSupplier->serialize(objectSize);

		Supplier * newSupplier = new Supplier();
		newSupplier->deserialize(buffer);

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




	//Test LineItem Class:
	cout << "Testing \"LineItem\" Class.\n";

	//Create a list of objects:
	vector<LineItem *> lineItemObjectList = LineItem::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		LineItem * oldLineItem = lineItemObjectList.at(i);

		buffer = oldLineItem->serialize(objectSize);
		//cout << "OLD: " << oldLineItem->toString() << endl;

		LineItem * newLineItem = new LineItem();
		newLineItem->deserialize(buffer);

		//cout << "NEW: " << newLineItem->toString() << endl;

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




	//Test Order Class:
	cout << "Testing \"Order\" Class.\n";

	//Create a list of objects:
	vector<Order *> orderObjectList = Order::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		Order * oldOrder = orderObjectList.at(i);

		buffer = oldOrder->serialize(objectSize);
		//cout << "OLD: " << oldOrder->toString() << endl;

		Order * newOrder = new Order();
		newOrder->deserialize(buffer);

		//cout << "NEW: " << newOrder->toString() << endl;

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


	//Test Customer Class:
	cout << "Testing \"Customer\" Class.\n";

	//Create a list of objects:
	vector<Customer *> customerObjectList = Customer::generateObjects(countObjects);
	
	objectSize = 0;

	for (int i = 0; i < countObjects; i++) {
		Customer * oldCustomer = customerObjectList.at(i);

		buffer = oldCustomer->serialize(objectSize);
		//cout << objectSize << endl;
		//cout << "OLD: " << oldCustomer->toString() << endl;

		Customer * newCustomer = new Customer();
		newCustomer->deserialize(buffer);

		//cout << "NEW: " << newCustomer->toString() << endl;

		if (!(oldCustomer->toString().compare(newCustomer->toString())==0)) {
			throw invalid_argument("Customer: Serialized and De-serialized contents are different.");
		}

		//Release Memory:
		delete[] buffer;

		//Delete new object:
		delete newCustomer;
		delete oldCustomer;
	}
	customerObjectList.clear();

}