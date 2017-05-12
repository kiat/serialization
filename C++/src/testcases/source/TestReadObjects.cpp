#include "DataReader.h"
using namespace std;


int main(int argc, char* argv[]) {

	if (argc != 5) {
		cout << "Requires two arguments: numObjects serializationType objectType isSequential" << endl; 
		cout << "Run as: ./a.out 10 0 0 1" << endl;
		return 0;
	}

	string scaleFactor = "0.1";
	int numObjects = atoi(argv[1]);
	int serializationType = atoi(argv[2]);
	int objectType = atoi(argv[3]);
	int isSequential = atoi(argv[4]);

	//Basic Checks:
	if (objectType < 0 || objectType > 2) {
		throw invalid_argument("ObjectType: Invalid input.");
	}

	if (serializationType < 0 || serializationType > 3) {
		throw invalid_argument("SerializationType: Invalid input.");
	}

	if (isSequential < 0 || isSequential > 1) {
		throw invalid_argument("SequentialRead/RandomRead: Invalid input.");
	}	


	//Create DataGenerator:
	DataReader * d = new DataReader(scaleFactor, objectType, serializationType);

	if(isSequential) {
		d->readObjectsSequential(numObjects);
	}
	else{
		d->readObjectsRandom(numObjects);
	}

	//Free Memory: 
	delete d;

	return 0;
}