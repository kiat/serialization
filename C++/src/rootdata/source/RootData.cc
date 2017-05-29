#include <iostream>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include "RootData.h"
#include <boost/archive/text_iarchive.hpp>
using namespace std;

//Note: 
//1) All "new" has matching "delete". Ruling out possible memory leak.
//2) Do not use "strcpy", instead use "memcpy" with string.length().

	/*
	 * These are utility functions. Putting in RootData to avoid multiple inheritance. 
	 */

	// Copy a double into the buffer.
	char * RootData::copyDouble(char * buffer, double copyThis) {
		memcpy(buffer, &copyThis, sizeof(copyThis));
		buffer += sizeof(copyThis);
		return buffer;
	}	
	
	// Parse a double from buffer.
	double RootData::parseDouble(char * buffer) {
		double element;
		memcpy(&element, buffer, sizeof(element));	
		return element;
	}

	// Copy an integer into the buffer.
	char * RootData::copyInt(char * buffer, int copyThis) {
		memcpy(buffer, &copyThis, sizeof(copyThis));
		buffer += sizeof(copyThis);
		return buffer;
	}
	
	// Parse an integer from buffer.
	int RootData::parseInt(char * buffer) {
		int element;
		memcpy(&element, buffer, sizeof(element));	
		return element;
	}
	
	// Copy an long into the buffer.
	char * RootData::copyLong(char * buffer, long copyThis) {
		memcpy(buffer, &copyThis, sizeof(copyThis));
		buffer += sizeof(copyThis);
		return buffer;
	}
	
	// Parse an long from buffer.
	long RootData::parseLong(char * buffer) {
		long element;
		memcpy(&element, buffer, sizeof(element));	
		return element;
	}


	// Copy [size(string), string] into the buffer.
	char * RootData::copyString (char * buffer, string copyThis) {
		//Write "length" of the string.
		int sizeofString = copyThis.length();
		memcpy(buffer, &sizeofString, sizeof(sizeofString));
		buffer += sizeof(sizeofString);	
		//Write the content of the string.
		memcpy(buffer, copyThis.c_str(), copyThis.length());
		buffer += copyThis.length();
		return buffer;
	}
	
/*		
	//Parse a string from buffer.
	string RootData::parseString (char * buffer) {
		//Parse "length" of the string.
		int sizeofString;
		memcpy(&sizeofString, buffer, sizeof(sizeofString));
		buffer += sizeof(sizeofString);
		
		//Parse the content of the string.
		string s;
		s.assign(buffer, sizeofString);

		return s;
	}
*/

	//Avoid sending string as returned object:
	void RootData::parseString(char * buffer, string& s) {
		//Parse "length" of the string.
		int sizeofString;
		memcpy(&sizeofString, buffer, sizeof(sizeofString));
		buffer += sizeof(sizeofString);
		
		//Parse the content of the string.
		s.assign(buffer, sizeofString);
	}

	
	//Convert integer to string.
	string RootData::itos(int element) {
		ostringstream ss;
		ss << element;
		return ss.str();
	}
	
	//Convert double to string.
	string RootData::dtos (double element) {
		ostringstream ss;
		ss << element;
		return ss.str();		
	}
	
	//Unimplemented Functions:
	long RootData::serializeInPlace(char * fileStart) {
		throw invalid_argument("RootData::serializeInPlace(...)");
		return -1;
	}

	char * RootData::serialize(char* buffer, int &objectSize) {
		throw invalid_argument("RootData::serialize(...)");
		return NULL;
	}

	int RootData::deserialize(char * buffer) {
		throw invalid_argument("RootData::deserialize(...)");
		return -1;		
	}

	void RootData::deserializeBoost(char * buffer, RootData*) {
		throw invalid_argument("RootData* RootData::deserializeBoost(...)");
	}

	void RootData::freePointedObjects() {
		throw invalid_argument("RootData::freeMemory(...)");
	}	

	boost::archive::text_iarchive * RootData::getArchive() {
		throw invalid_argument("RootData::getArchive(...)");
		return NULL;
	}

	stringstream * RootData::getStream() {
		throw invalid_argument("RootData::getStream(...)");
		return NULL;		
	}