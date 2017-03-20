#include "DataGenerator.h"
#include "ExternalSorter.h"
using namespace std;


int main(int argc, char* argv[]) {

	//Process arguments first:
	if (argc != 6) {
		cout << "Requires four arguments: scaleFactor numObjects objectType serializationType fileSplits" << endl; 
		cout << "Run as: ./a.out 0.1 10 2 3 10" << endl;
		return 0;
	}


	//Scale Factor:
	string scaleFactor = string(argv[1]);

	//Number of objects to sort:
	int numObjects = atoi(argv[2]);

	//Object Type:
	int objectType = atoi(argv[3]);	//0 = Part; 1 = LineItem; 2 = Customer.

	//Serialization Type:
	int serializationType = atoi(argv[4]);	// 2 = InPlace, 3 = Boost etc.

	//Number of splits of the original file:
	int fileSplits = atoi(argv[5]);


	//cout << "language#method#seq#datatype#iotime#totaltime" << endl;

	//Read data from files: "false" retrieves stored data, "true" generates new data:
	DataGenerator * d = new DataGenerator(scaleFactor, false, serializationType, objectType);

	//SECTION: Code related to external sort begins subsequently:

	//Sort Parts:
	if (objectType == 0) {
		// Sort "Part" Objects:	
		vector<FileHandler<Part> * > partFileHandlerList = d->getPartFileHandlerList();

		int i = serializationType;

		//External Sorter:
		ExternalSorter<Part> * es = new ExternalSorter<Part>(partFileHandlerList.at(i));
		FileHandler<Part> * sfh = es->sortFile(numObjects, objectType, fileSplits);

		//Print details.
		cout << "[ReadtimeCPP]" << "#" << "ExternalSort" << "#" << "Part" << "#" <<  d->getSerializationName(i) << "#" << es->getTotalIOTime() << "#" << (es->getTotalTimeExternalSort() - es->getOverheadTime()) << endl;

		//Sanity Check:
		if(sfh->getObjectCount() != numObjects) {  //partFileHandlerList.at(i)->getObjectCount()
			throw invalid_argument("Part: Mismatch in object count.");
		}

		//Memory Leak:
		es->freePointedResources();
		delete es;

	}




	//Sort "LineItem" Objects:
	if (objectType == 1) {

		// Sort "LineItem" Objects:
		vector<FileHandler<LineItem> * > lineItemFileHandlerList = d->getLineItemFileHandlerList();

		int i = serializationType;

		//External Sorter:
		ExternalSorter<LineItem> * es = new ExternalSorter<LineItem>(lineItemFileHandlerList.at(i));
		FileHandler<LineItem> * sfh = es->sortFile(numObjects, objectType, fileSplits);

		//Print details.
		cout << "[ReadtimeCPP]" << "#" << "ExternalSort" << "#" << "LineItem" << "#" <<  d->getSerializationName(i) << "#" << es->getTotalIOTime() << "#" << (es->getTotalTimeExternalSort() - es->getOverheadTime()) << endl;

		//Sanity Check:
		if(sfh->getObjectCount() != numObjects) { 	//lineItemFileHandlerList.at(i)->getObjectCount()
			throw invalid_argument("LineItem: Mismatch in object count.");
		}

		//Memory Leak:
		es->freePointedResources();
		delete es;

	}




	// Sort "Customer" Objects:
	if (objectType == 2) {

		vector<FileHandler<Customer> * > customerFileHandlerList = d->getCustomerFileHandlerList();

		int i = serializationType;

		//External Sorter:
		ExternalSorter<Customer> * es = new ExternalSorter<Customer>(customerFileHandlerList.at(i));
		FileHandler<Customer> * sfh = es->sortFile(numObjects, objectType, fileSplits);

		//Print details.
		cout << "[ReadtimeCPP]" << "#" << "ExternalSort" << "#" << "Customer" << "#" <<  d->getSerializationName(i) << "#" << es->getTotalIOTime() << "#" << (es->getTotalTimeExternalSort() - es->getOverheadTime()) << endl;

		//Sanity Check:
		if(sfh->getObjectCount() != numObjects) { //customerFileHandlerList.at(i)->getObjectCount()
			throw invalid_argument("Customer: Mismatch in object count.");
		}

		//Memory Leak:
		es->freePointedResources();
		delete es;		
	}


	//Free data generator:
	delete d;


	return 0;
}