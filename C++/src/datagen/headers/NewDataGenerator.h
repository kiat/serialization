#ifndef NEWDATAGEN_H
#define NEWDATAGEN_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <chrono>
#include <sstream>
#include <vector>
#include "FileHandler.h"
#include "RootData.h"

#include "Part.h"
#include "Supplier.h"
#include "LineItem.h"
#include "Order.h"
#include "Customer.h"

#include "PartBoost.h"
#include "SupplierBoost.h"
#include "LineItemBoost.h"
#include "OrderBoost.h"
#include "CustomerBoost.h"

#include "PartProto.h"
#include "SupplierProto.h"
#include "LineItemProto.h"
#include "OrderProto.h"
#include "CustomerProto.h"


using namespace std;

						

class NewDataGenerator : public RootData {


	public:
		//Default destructor:
		~NewDataGenerator() {
		}


		void setDefaults() {
			//Duplication Factor(OLD): 60, 2, 7
			//Read Experiments: Duplication Factor: 16000, 200, 200
			//Duplication Factor: 500, 20, 20
			this->partDuplicationFactor = 16000;
			this->lineItemDuplicationFactor = 200;
			this->customerDuplicationFactor = 200;

			//Total Serialization Types:
			this->totalSerializationTypes = 4;			
		}


		//General constructor used all along:
		NewDataGenerator(string scaleFactor, int serializationType, int objectType) {
			//We have datasets with 0.1, 1, ...
			this->scaleFactor = scaleFactor;

			//Set serialization type:
			this->serializationType = serializationType;

			//Initialize with default values:
			this->setDefaults();

			//Prepare data in appropriate files:
			this->prepareData(serializationType, objectType);
		}

	private:
		//Specifications:
		string scaleFactor;


		//How many copies of data?
		int partDuplicationFactor;
		int lineItemDuplicationFactor;
		int customerDuplicationFactor;

		//Total serialization types:
		int totalSerializationTypes;


		//Print logs or not:
		bool printLogs = true;

		int serializationType;


		// Will prepare HandCoded/InPlace Objects:
		void prepareCommonData(int objectType) {
			// All files to parse:
			string PartFile = "tables_scale_" + scaleFactor + "/part.tbl";
			string SupplierFile = "tables_scale_" + scaleFactor + "/supplier.tbl";
			string OrderFile = "tables_scale_" + scaleFactor + "/orders.tbl";
			string LineitemFile = "tables_scale_" + scaleFactor + "/lineitem.tbl";
			string CustomerFile = "tables_scale_" + scaleFactor + "/customer.tbl";

			//Common constructs:
			string line;
			string delimiter = "|";
			ifstream infile;


			// ####################################
			// ####################################
			// ########## #########
			// ########## Part #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "PartFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(PartFile.c_str());
			vector<Part *> partList;
			map <int, Part *> partMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}

				Part * tPart = new Part(atoi(tokens.at(0).c_str()), tokens.at(1), tokens.at(2), tokens.at(3), tokens.at(4), atoi(tokens.at(5).c_str()), tokens.at(6), atof(tokens.at(7).c_str()), tokens.at(8));
				partList.push_back(tPart);

				//Populate the hash:
				partMap[atoi(tokens.at(0).c_str())] = tPart;
			}

			//Serialize content and store in respective files:
			if (objectType == 0)
			{
				int j = serializationType;  

				//Set FileHandler:
				FileHandler<Part> * partFileHandler = new FileHandler<Part>("partObjects" + itos(j) +".txt", j);
				// Write objects in file.
				partFileHandler->appendObjectsToFile(partList);

				//Duplicate data:
				for (int i = 0; i < partDuplicationFactor; i++) {
					partFileHandler->appendObjectsToFile(partList);
				}

				// Flush data in file.
				partFileHandler->flush();	

				partFileHandler->store();

				if (printLogs) {
					cout << "Total Part objects: " << partFileHandler->getObjectCount() << endl;
				}
				
				delete partFileHandler;


				//Pushing Inside:
				infile.close(); infile.clear();
				return;
			}	
			infile.close(); infile.clear();



			// ####################################
			// ####################################
			// ########## #########
			// ########## Supplier #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "SupplierFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(SupplierFile.c_str());
			vector<Supplier *> supplierList;
			map <int, Supplier *> supplierMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}

				Supplier * tSupplier = new Supplier(atoi(tokens.at(0).c_str()), tokens.at(1), tokens.at(2), atoi(tokens.at(3).c_str()), tokens.at(4), atof(tokens.at(5).c_str()), tokens.at(6));
				supplierList.push_back(tSupplier);


				//Populate the hash:
				supplierMap[atoi(tokens.at(0).c_str())] = tSupplier;				
			}

			infile.close(); infile.clear();

			


			// ####################################
			// ####################################
			// ########## #########
			// ########## LineItem #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "LineitemFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(LineitemFile.c_str());
			vector<LineItem *> lineItemList;
			map <int, vector<LineItem *> > lineItemMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}


				int orderKey = atoi(tokens.at(0).c_str());
				int partKey = atoi(tokens.at(1).c_str());
				int supplierKey = atoi(tokens.at(2).c_str());

				Part * tPart;
				Supplier * tSupplier;

				//Find the appropriate "Part"
				if (partMap.find(partKey) != partMap.end()) {
					tPart = partMap[partKey];
				}
				else {
					throw invalid_argument("There is no such Part.");
				}


				//Find the appropriate "Part"
				if (supplierMap.find(supplierKey) != supplierMap.end()) {
					tSupplier = supplierMap[supplierKey];
				}
				else {
					throw invalid_argument("There is no such Supplier.");
				}




				// The field "name" has been omitted by Kia in his code.
				LineItem * tLineItem = new LineItem(tokens.at(0), orderKey, tSupplier, tPart, atoi(tokens.at(3).c_str()), atof(tokens.at(4).c_str()), atof(tokens.at(5).c_str()), atof(tokens.at(6).c_str()), atof(tokens.at(7).c_str()), 
					tokens.at(8), tokens.at(9), tokens.at(10), tokens.at(11), tokens.at(12), tokens.at(13), tokens.at(14), tokens.at(15));
				lineItemList.push_back(tLineItem);


				//Populate the hash:
				if (lineItemMap.find(orderKey) != lineItemMap.end()) {
					lineItemMap[orderKey].push_back(tLineItem);
				}
				else {
					vector<LineItem *> tLineItemArray;
					tLineItemArray.push_back(tLineItem);
					lineItemMap[orderKey] = tLineItemArray;
				}
			
			}



			//Serialize content and store in respective files:
			if (objectType == 1)
			{
				int j = serializationType;

				//Set FileHandler:
				FileHandler<LineItem> * lineItemFileHandler = new FileHandler<LineItem>("lineItemObjects" + itos(j) + ".txt", j);
				// Write objects in file.
				lineItemFileHandler->appendObjectsToFile(lineItemList);

				//Duplicate data:
				for (int i = 0; i < lineItemDuplicationFactor; i++) {
					lineItemFileHandler->appendObjectsToFile(lineItemList);
				}


				// Flush data in file.
				lineItemFileHandler->flush();	

				lineItemFileHandler->store();	

				if (printLogs) {
					cout << "Total LineItem objects: " << lineItemFileHandler->getObjectCount() << endl;	
				}	

				delete lineItemFileHandler;

				//Pushing Inside:
				infile.close(); infile.clear();
				return;				
			}	
			infile.close(); infile.clear();


			// ####################################
			// ####################################
			//New code: You actually don't need these maps after this point: Reduce memory footprint:
			partMap.clear();
			supplierMap.clear();
			// ####################################
			// ####################################



			// ####################################
			// ####################################
			// ########## #########
			// ########## Order #########
			// ########## #########
			// ####################################
			// ####################################


			//Open "OrderFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(OrderFile.c_str());
			vector<Order *> orderList;
			map <int, vector<Order *> > orderMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}


				int orderKey =  atoi(tokens.at(0).c_str());
				int customerKey = atoi(tokens.at(1).c_str());

				//Sanity Check:
				if (lineItemMap.find(orderKey) == lineItemMap.end()) {
					throw invalid_argument("There is no such Order.");
				}



				Order * tOrder = new Order(lineItemMap[orderKey], orderKey, customerKey, tokens.at(2), 
					atof(tokens.at(3).c_str()), tokens.at(4), tokens.at(5), tokens.at(6), atoi(tokens.at(7).c_str()), tokens.at(8));
				orderList.push_back(tOrder);



				//Populate the hash:
				if (orderMap.find(customerKey) != orderMap.end()) {
					orderMap[customerKey].push_back(tOrder);
				}
				else {
					vector<Order *> tOrderArray;
					tOrderArray.push_back(tOrder);
					orderMap[customerKey] = tOrderArray;
				}
			
			}
	
			infile.close(); infile.clear();		

			

			// ####################################
			// ####################################
			//New code: You actually don't need these maps after this point: Reduce memory footprint:
			lineItemMap.clear();
			// ####################################
			// ####################################



			// ####################################
			// ####################################
			// ########## #########
			// ########## Customers #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "CustomerFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(CustomerFile.c_str());
			vector<Customer *> customerList;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}		
			

				int customerKey = atoi(tokens.at(0).c_str());


				//Sanity: Deal with Customers without orders.
				if (orderMap.find(customerKey) == orderMap.end()) {
					vector<Order *> tOrderArray;
					orderMap[customerKey] = tOrderArray;					
				}


				Customer * tCustomer = new Customer(orderMap[customerKey], customerKey, tokens.at(1), tokens.at(2), atoi(tokens.at(3).c_str()), tokens.at(4), 
					atof(tokens.at(5).c_str()), tokens.at(6), tokens.at(7));

				customerList.push_back(tCustomer);
			}



			// ####################################
			// ####################################
			//New code: You actually don't need these maps after this point: Reduce memory footprint:
			orderMap.clear();
			// ####################################
			// ####################################



			//Serialize content and store in respective files:
			if (objectType == 2)
			{
				int j = serializationType;

				//Set FileHandler:
				FileHandler<Customer> * customerFileHandler = new FileHandler<Customer>("customerObjects" + itos(j) + ".txt", j);
				// Write objects in file.
				customerFileHandler->appendObjectsToFile(customerList);

				//Duplicate data:
				for (int i = 0; i < customerDuplicationFactor; i++) {
					customerFileHandler->appendObjectsToFile(customerList);
				}

				// Flush data in file.
				customerFileHandler->flush();

				customerFileHandler->store();

				if (printLogs) {				
					cout << "Total Customer objects: " << customerFileHandler->getObjectCount() << endl;
				}

				delete customerFileHandler;

				//Pushing Inside:
				infile.close(); infile.clear();
				return;
			}	
			infile.close(); infile.clear();

			//Memory leaks: All objects created using "new" must be explicitly deleted.
			//The pointers are shared in such a weird way, I can't free the objects here.
			//The memory is released by program termination here.
		}


		void prepareHandCodedData(int objectType) {
			prepareCommonData(objectType);
		}

		void prepareInPlaceData(int objectType) {
			prepareCommonData(objectType);
		}


		// Will prepare Boost Objects:
		void prepareBoostData(int objectType) {
			// All files to parse:
			string PartFile = "tables_scale_" + scaleFactor + "/part.tbl";
			string SupplierFile = "tables_scale_" + scaleFactor + "/supplier.tbl";
			string OrderFile = "tables_scale_" + scaleFactor + "/orders.tbl";
			string LineitemFile = "tables_scale_" + scaleFactor + "/lineitem.tbl";
			string CustomerFile = "tables_scale_" + scaleFactor + "/customer.tbl";

			//Common constructs:
			string line;
			string delimiter = "|";
			ifstream infile;


			// ####################################
			// ####################################
			// ########## #########
			// ########## Part #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "PartFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(PartFile.c_str());
			vector<PartBoost *> partList;
			map <int, PartBoost *> partMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}

				PartBoost * tPart = new PartBoost(atoi(tokens.at(0).c_str()), tokens.at(1), tokens.at(2), tokens.at(3), tokens.at(4), atoi(tokens.at(5).c_str()), tokens.at(6), atof(tokens.at(7).c_str()), tokens.at(8));
				partList.push_back(tPart);

				//Populate the hash:
				partMap[atoi(tokens.at(0).c_str())] = tPart;
			}

			//Serialize content and store in respective files:
			if (objectType == 0)
			{
				int j = serializationType;

				//Set FileHandler:
				FileHandler<PartBoost> * partFileHandler = new FileHandler<PartBoost>("partObjects" + itos(j) +".txt", j);
				// Write objects in file.
				partFileHandler->appendObjectsToFile(partList);

				//Duplicate data:
				for (int i = 0; i < partDuplicationFactor; i++) {
					partFileHandler->appendObjectsToFile(partList);
				}

				// Flush data in file.
				partFileHandler->flush();	

				partFileHandler->store();

				if (printLogs) {
					cout << "Total Part objects: " << partFileHandler->getObjectCount() << endl;
				}
				
				delete partFileHandler;


				//Pushing Inside:
				infile.close(); infile.clear();
				return;
			}	
			infile.close(); infile.clear();


			// ####################################
			// ####################################
			// ########## #########
			// ########## Supplier #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "SupplierFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(SupplierFile.c_str());
			vector<SupplierBoost *> supplierList;
			map <int, SupplierBoost *> supplierMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}

				SupplierBoost * tSupplier = new SupplierBoost(atoi(tokens.at(0).c_str()), tokens.at(1), tokens.at(2), atoi(tokens.at(3).c_str()), tokens.at(4), atof(tokens.at(5).c_str()), tokens.at(6));
				supplierList.push_back(tSupplier);


				//Populate the hash:
				supplierMap[atoi(tokens.at(0).c_str())] = tSupplier;				
			}

			infile.close(); infile.clear();

			


			// ####################################
			// ####################################
			// ########## #########
			// ########## LineItem #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "LineitemFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(LineitemFile.c_str());
			vector<LineItemBoost *> lineItemList;
			map <int, vector<LineItemBoost *> > lineItemMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}


				int orderKey = atoi(tokens.at(0).c_str());
				int partKey = atoi(tokens.at(1).c_str());
				int supplierKey = atoi(tokens.at(2).c_str());

				PartBoost * tPart;
				SupplierBoost * tSupplier;

				//Find the appropriate "Part"
				if (partMap.find(partKey) != partMap.end()) {
					tPart = partMap[partKey];
				}
				else {
					throw invalid_argument("There is no such Part.");
				}


				//Find the appropriate "Part"
				if (supplierMap.find(supplierKey) != supplierMap.end()) {
					tSupplier = supplierMap[supplierKey];
				}
				else {
					throw invalid_argument("There is no such Supplier.");
				}




				// The field "name" has been omitted by Kia in his code.
				LineItemBoost * tLineItem = new LineItemBoost(tokens.at(0), orderKey, tSupplier, tPart, atoi(tokens.at(3).c_str()), atof(tokens.at(4).c_str()), atof(tokens.at(5).c_str()), atof(tokens.at(6).c_str()), atof(tokens.at(7).c_str()), 
					tokens.at(8), tokens.at(9), tokens.at(10), tokens.at(11), tokens.at(12), tokens.at(13), tokens.at(14), tokens.at(15));
				lineItemList.push_back(tLineItem);


				//Populate the hash:
				if (lineItemMap.find(orderKey) != lineItemMap.end()) {
					lineItemMap[orderKey].push_back(tLineItem);
				}
				else {
					vector<LineItemBoost *> tLineItemArray;
					tLineItemArray.push_back(tLineItem);
					lineItemMap[orderKey] = tLineItemArray;
				}
			
			}



			//Serialize content and store in respective files:
			if (objectType == 1)
			{
				int j = serializationType;

				//Set FileHandler:
				FileHandler<LineItemBoost> * lineItemFileHandler = new FileHandler<LineItemBoost>("lineItemObjects" + itos(j) + ".txt", j);
				// Write objects in file.
				lineItemFileHandler->appendObjectsToFile(lineItemList);

				//Duplicate data:
				for (int i = 0; i < lineItemDuplicationFactor; i++) {
					lineItemFileHandler->appendObjectsToFile(lineItemList);
				}


				// Flush data in file.
				lineItemFileHandler->flush();	

				lineItemFileHandler->store();	

				if (printLogs) {
					cout << "Total LineItem objects: " << lineItemFileHandler->getObjectCount() << endl;	
				}	

				delete lineItemFileHandler;

				//Pushing Inside:
				infile.close(); infile.clear();
				return;
			}	
			infile.close(); infile.clear();


			// ####################################
			// ####################################
			//New code: You actually don't need these maps after this point: Reduce memory footprint:
			partMap.clear();
			supplierMap.clear();
			// ####################################
			// ####################################



			// ####################################
			// ####################################
			// ########## #########
			// ########## Order #########
			// ########## #########
			// ####################################
			// ####################################


			//Open "OrderFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(OrderFile.c_str());
			vector<OrderBoost *> orderList;
			map <int, vector<OrderBoost *> > orderMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}


				int orderKey =  atoi(tokens.at(0).c_str());
				int customerKey = atoi(tokens.at(1).c_str());

				//Sanity Check:
				if (lineItemMap.find(orderKey) == lineItemMap.end()) {
					throw invalid_argument("There is no such Order.");
				}



				OrderBoost * tOrder = new OrderBoost(lineItemMap[orderKey], orderKey, customerKey, tokens.at(2), 
					atof(tokens.at(3).c_str()), tokens.at(4), tokens.at(5), tokens.at(6), atoi(tokens.at(7).c_str()), tokens.at(8));
				orderList.push_back(tOrder);



				//Populate the hash:
				if (orderMap.find(customerKey) != orderMap.end()) {
					orderMap[customerKey].push_back(tOrder);
				}
				else {
					vector<OrderBoost *> tOrderArray;
					tOrderArray.push_back(tOrder);
					orderMap[customerKey] = tOrderArray;
				}
			
			}
	
			infile.close(); infile.clear();		

			

			// ####################################
			// ####################################
			//New code: You actually don't need these maps after this point: Reduce memory footprint:
			lineItemMap.clear();
			// ####################################
			// ####################################



			// ####################################
			// ####################################
			// ########## #########
			// ########## Customers #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "CustomerFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(CustomerFile.c_str());
			vector<CustomerBoost *> customerList;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}		
			

				int customerKey = atoi(tokens.at(0).c_str());


				//Sanity: Deal with Customers without orders.
				if (orderMap.find(customerKey) == orderMap.end()) {
					vector<OrderBoost *> tOrderArray;
					orderMap[customerKey] = tOrderArray;					
				}


				CustomerBoost * tCustomer = new CustomerBoost(orderMap[customerKey], customerKey, tokens.at(1), tokens.at(2), atoi(tokens.at(3).c_str()), tokens.at(4), 
					atof(tokens.at(5).c_str()), tokens.at(6), tokens.at(7));

				customerList.push_back(tCustomer);
			}



			// ####################################
			// ####################################
			//New code: You actually don't need these maps after this point: Reduce memory footprint:
			orderMap.clear();
			// ####################################
			// ####################################



			//Serialize content and store in respective files:
			if (objectType == 2)
			{
				int j = serializationType;

				//Set FileHandler:
				FileHandler<CustomerBoost> * customerFileHandler = new FileHandler<CustomerBoost>("customerObjects" + itos(j) + ".txt", j);
				// Write objects in file.
				customerFileHandler->appendObjectsToFile(customerList);

				//Duplicate data:
				for (int i = 0; i < customerDuplicationFactor; i++) {
					customerFileHandler->appendObjectsToFile(customerList);
				}

				// Flush data in file.
				customerFileHandler->flush();

				customerFileHandler->store();

				if (printLogs) {				
					cout << "Total Customer objects: " << customerFileHandler->getObjectCount() << endl;
				}

				delete customerFileHandler;

				//Pushing Inside:
				infile.close(); infile.clear();
				return;				
			}	
			infile.close(); infile.clear();

			//Memory leaks: All objects created using "new" must be explicitly deleted.
			//The pointers are shared in such a weird way, I can't free the objects here.
			//The memory is released by program termination here.
		}

		// Will prepare ProtocolBuffer Objects:
		void prepareProtoData(int objectType) {
			// All files to parse:
			string PartFile = "tables_scale_" + scaleFactor + "/part.tbl";
			string SupplierFile = "tables_scale_" + scaleFactor + "/supplier.tbl";
			string OrderFile = "tables_scale_" + scaleFactor + "/orders.tbl";
			string LineitemFile = "tables_scale_" + scaleFactor + "/lineitem.tbl";
			string CustomerFile = "tables_scale_" + scaleFactor + "/customer.tbl";

			//Common constructs:
			string line;
			string delimiter = "|";
			ifstream infile;


			// ####################################
			// ####################################
			// ########## #########
			// ########## Part #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "PartFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(PartFile.c_str());
			vector<PartProto *> partList;
			map <int, PartProto *> partMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}

				PartProto * tPart = new PartProto(atoi(tokens.at(0).c_str()), tokens.at(1), tokens.at(2), tokens.at(3), tokens.at(4), atoi(tokens.at(5).c_str()), tokens.at(6), atof(tokens.at(7).c_str()), tokens.at(8));
				partList.push_back(tPart);

				//Populate the hash:
				partMap[atoi(tokens.at(0).c_str())] = tPart;
			}

			//Serialize content and store in respective files:
			if (objectType == 0)
			{
				int j = serializationType;

				//Set FileHandler:
				FileHandler<PartProto> * partFileHandler = new FileHandler<PartProto>("partObjects" + itos(j) +".txt", j);
				// Write objects in file.
				partFileHandler->appendObjectsToFile(partList);

				//Duplicate data:
				for (int i = 0; i < partDuplicationFactor; i++) {
					partFileHandler->appendObjectsToFile(partList);
				}

				// Flush data in file.
				partFileHandler->flush();	

				partFileHandler->store();

				if (printLogs) {
					cout << "Total Part objects: " << partFileHandler->getObjectCount() << endl;
				}
				
				delete partFileHandler;

				//Pushing Inside:
				infile.close(); infile.clear();
				return;
			}
			infile.close(); infile.clear();	


			// ####################################
			// ####################################
			// ########## #########
			// ########## Supplier #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "SupplierFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(SupplierFile.c_str());
			vector<SupplierProto *> supplierList;
			map <int, SupplierProto *> supplierMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}

				SupplierProto * tSupplier = new SupplierProto(atoi(tokens.at(0).c_str()), tokens.at(1), tokens.at(2), atoi(tokens.at(3).c_str()), tokens.at(4), atof(tokens.at(5).c_str()), tokens.at(6));
				supplierList.push_back(tSupplier);


				//Populate the hash:
				supplierMap[atoi(tokens.at(0).c_str())] = tSupplier;				
			}

			infile.close(); infile.clear();

			


			// ####################################
			// ####################################
			// ########## #########
			// ########## LineItem #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "LineitemFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(LineitemFile.c_str());
			vector<LineItemProto *> lineItemList;
			map <int, vector<LineItemProto *> > lineItemMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}


				int orderKey = atoi(tokens.at(0).c_str());
				int partKey = atoi(tokens.at(1).c_str());
				int supplierKey = atoi(tokens.at(2).c_str());

				PartProto * tPart;
				SupplierProto * tSupplier;

				//Find the appropriate "Part"
				if (partMap.find(partKey) != partMap.end()) {
					tPart = partMap[partKey];
				}
				else {
					throw invalid_argument("There is no such Part.");
				}


				//Find the appropriate "Part"
				if (supplierMap.find(supplierKey) != supplierMap.end()) {
					tSupplier = supplierMap[supplierKey];
				}
				else {
					throw invalid_argument("There is no such Supplier.");
				}




				// The field "name" has been omitted by Kia in his code.
				LineItemProto * tLineItem = new LineItemProto(tokens.at(0), orderKey, tSupplier, tPart, atoi(tokens.at(3).c_str()), atof(tokens.at(4).c_str()), atof(tokens.at(5).c_str()), atof(tokens.at(6).c_str()), atof(tokens.at(7).c_str()), 
					tokens.at(8), tokens.at(9), tokens.at(10), tokens.at(11), tokens.at(12), tokens.at(13), tokens.at(14), tokens.at(15));
				lineItemList.push_back(tLineItem);


				//Populate the hash:
				if (lineItemMap.find(orderKey) != lineItemMap.end()) {
					lineItemMap[orderKey].push_back(tLineItem);
				}
				else {
					vector<LineItemProto *> tLineItemArray;
					tLineItemArray.push_back(tLineItem);
					lineItemMap[orderKey] = tLineItemArray;
				}
			
			}



			//Serialize content and store in respective files:
			if (objectType == 1)
			{
				int j = serializationType;

				//Set FileHandler:
				FileHandler<LineItemProto> * lineItemFileHandler = new FileHandler<LineItemProto>("lineItemObjects" + itos(j) + ".txt", j);
				// Write objects in file.
				lineItemFileHandler->appendObjectsToFile(lineItemList);

				//Duplicate data:
				for (int i = 0; i < lineItemDuplicationFactor; i++) {
					lineItemFileHandler->appendObjectsToFile(lineItemList);
				}


				// Flush data in file.
				lineItemFileHandler->flush();	

				lineItemFileHandler->store();	

				if (printLogs) {
					cout << "Total LineItem objects: " << lineItemFileHandler->getObjectCount() << endl;	
				}	

				delete lineItemFileHandler;

				//Pushing Inside:
				infile.close(); infile.clear();
				return;				
			}		
			infile.close(); infile.clear();


			// ####################################
			// ####################################
			//New code: You actually don't need these maps after this point: Reduce memory footprint:
			partMap.clear();
			supplierMap.clear();
			// ####################################
			// ####################################



			// ####################################
			// ####################################
			// ########## #########
			// ########## Order #########
			// ########## #########
			// ####################################
			// ####################################


			//Open "OrderFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(OrderFile.c_str());
			vector<OrderProto *> orderList;
			map <int, vector<OrderProto *> > orderMap;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}


				int orderKey =  atoi(tokens.at(0).c_str());
				int customerKey = atoi(tokens.at(1).c_str());

				//Sanity Check:
				if (lineItemMap.find(orderKey) == lineItemMap.end()) {
					throw invalid_argument("There is no such Order.");
				}



				OrderProto * tOrder = new OrderProto(lineItemMap[orderKey], orderKey, customerKey, tokens.at(2), 
					atof(tokens.at(3).c_str()), tokens.at(4), tokens.at(5), tokens.at(6), atoi(tokens.at(7).c_str()), tokens.at(8));
				orderList.push_back(tOrder);



				//Populate the hash:
				if (orderMap.find(customerKey) != orderMap.end()) {
					orderMap[customerKey].push_back(tOrder);
				}
				else {
					vector<OrderProto *> tOrderArray;
					tOrderArray.push_back(tOrder);
					orderMap[customerKey] = tOrderArray;
				}
			
			}
	
			infile.close(); infile.clear();		

			

			// ####################################
			// ####################################
			//New code: You actually don't need these maps after this point: Reduce memory footprint:
			lineItemMap.clear();
			// ####################################
			// ####################################



			// ####################################
			// ####################################
			// ########## #########
			// ########## Customers #########
			// ########## #########
			// ####################################
			// ####################################

			//Open "CustomerFile": Iteratively (Read line, Parse line, Create Objects): 
			infile.open(CustomerFile.c_str());
			vector<CustomerProto *> customerList;
			while (getline(infile, line)) {
				stringstream lineStream (line);
				vector<string> tokens;
				string token;

				while(getline(lineStream, token, '|')) {
					tokens.push_back(token);
				}		
			

				int customerKey = atoi(tokens.at(0).c_str());


				//Sanity: Deal with Customers without orders.
				if (orderMap.find(customerKey) == orderMap.end()) {
					vector<OrderProto *> tOrderArray;
					orderMap[customerKey] = tOrderArray;					
				}


				CustomerProto * tCustomer = new CustomerProto(orderMap[customerKey], customerKey, tokens.at(1), tokens.at(2), atoi(tokens.at(3).c_str()), tokens.at(4), 
					atof(tokens.at(5).c_str()), tokens.at(6), tokens.at(7));

				customerList.push_back(tCustomer);
			}



			// ####################################
			// ####################################
			//New code: You actually don't need these maps after this point: Reduce memory footprint:
			orderMap.clear();
			// ####################################
			// ####################################



			//Serialize content and store in respective files:
			if (objectType == 2)
			{
				int j = serializationType;

				//Set FileHandler:
				FileHandler<CustomerProto> * customerFileHandler = new FileHandler<CustomerProto>("customerObjects" + itos(j) + ".txt", j);
				// Write objects in file.
				customerFileHandler->appendObjectsToFile(customerList);

				//Duplicate data:
				for (int i = 0; i < customerDuplicationFactor; i++) {
					customerFileHandler->appendObjectsToFile(customerList);
				}

				// Flush data in file.
				customerFileHandler->flush();

				customerFileHandler->store();

				if (printLogs) {				
					cout << "Total Customer objects: " << customerFileHandler->getObjectCount() << endl;
				}

				delete customerFileHandler;

				//Pushing Inside:
				infile.close(); infile.clear();
				return;
			}
			infile.close(); infile.clear();

			//Memory leaks: All objects created using "new" must be explicitly deleted.
			//The pointers are shared in such a weird way, I can't free the objects here.
			//The memory is released by program termination here.
		}



		void prepareData(int serializationType, int objectType) {
			switch(serializationType){
				case 0:
					prepareHandCodedData(objectType); 
					break;
				case 1:
					prepareProtoData(objectType); 
					break;
				case 2:
					prepareInPlaceData(objectType);
					break;
				case 3: 
					prepareBoostData(objectType);
					break;
				default: return;
			}
			
		}




};

#endif