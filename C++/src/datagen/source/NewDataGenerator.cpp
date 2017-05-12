#include "NewDataGenerator.h"
using namespace std;


int main(int argc, char* argv[]) {

	if (argc != 3) {
		cout << "Requires one argument: serializationType objectType" << endl; 
		cout << "Run as: ./a.out 3 2" << endl;
		return 0;
	}

	string scaleFactor = "0.1"; 
	int serializationType = atoi(argv[1]);
	int objectType = atoi(argv[2]);

	//Create DataGenerator:
	NewDataGenerator * nd = new NewDataGenerator(scaleFactor, serializationType, objectType);

	//Free Memory: 	
	delete nd;
	
	return 0;
}