#ifndef ROOTDATA_H
#define ROOTDATA_H

#include <iostream>
#include <chrono>
#include <boost/archive/text_iarchive.hpp>
using namespace std;

// Declaration of the "base" class.
class RootData {
	public:
		//char * copyBuffer(char * buffer, char * copyThis, int size);
		char * copyInt(char * buffer, int copyThis);
		char * copyLong(char * buffer, long copyThis);	
		char * copyDouble(char * buffer, double copyThis);
		char * copyString (char * buffer, string copyThis); 	
		//string parseString (char * buffer);
		int parseInt(char * buffer);
		long parseLong(char * buffer);
		double parseDouble(char * buffer);
		
		//int to string
		static string itos(int element);
		//double to string
		static string dtos (double element);


		//Efficient parseString:
		void parseString(char * buffer, string& s);

		//Unimplemented Functions:
		long serializeInPlace(char * fileStart);
		char * serialize(char* buffer, int &objectSize);
		int deserialize(char * buffer);
		void deserializeBoost(char *, RootData*);
		void freePointedObjects();
		boost::archive::text_iarchive * getArchive(); 
		stringstream * getStream();
};


#endif