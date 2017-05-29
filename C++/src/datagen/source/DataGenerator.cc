#include "DataGenerator.h"
using namespace std;


int main(int argc, char* argv[]) {

	if (argc != 2) {
		cout << "Requires one argument: serializationType" << endl; 
		cout << "Run as: ./a.out 3" << endl;
		return 0;
	}

	string scaleFactor = "0.1"; 
	int serializationType = atoi(argv[1]);

	//Create DataGenerator:
	DataGenerator * d = new DataGenerator(scaleFactor, serializationType);

	//Free Memory: 	
	delete d;
	
	return 0;
}