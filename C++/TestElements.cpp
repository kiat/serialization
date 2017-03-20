
#include <iostream>
#include <sstream>
#include "Element.h"
#include "ElementBoost.h"
#include "ElementProto.h"
#include "ElementIP.h"

#include "SparseVectors.h"

int main(int argc, char* argv[]) {

	int serializationType = atoi(argv[1]);
	cout << "serializationType:: " << serializationType << endl;

	int count = 1000;
	int dimension = 100000;

	SparseVectors * oldSV = new SparseVectors();
	oldSV->generateSparseVector(count, dimension, serializationType);


	SparseVectors * newSV = new SparseVectors();
	newSV->setUp(serializationType);

	int startIndex = 0;
	char * networkBuffer = new char [20 * 1024 * 1024];
	
	// Old:
	startIndex = oldSV->getNextNetworkPage(startIndex, networkBuffer);
	oldSV->printInfo();
	cout << "startIndex:: " << startIndex << endl;


	// New:
	newSV->parseNetworkPage(networkBuffer);
	newSV->printInfo();

}