#include "LineItemIP.h"
using namespace std;

	//Default constructor:
	LineItemIP::LineItemIP() {
		//throw invalid_argument("LineItemIP: Not default constructible.");
	}

	//Copy constructor:
	LineItemIP::LineItemIP(const LineItemIP &obj) {
		throw invalid_argument("LineItemIP: Not copy constructible.");
	}


	//Constructor with arguments:
	LineItemIP::LineItemIP(string name, int orderKey, Supplier *supplier, Part *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
	string lineStatus, string shipDate, string commitDate, string receiptDate, string shipinStruct, string shipMode, string comment){
		this->setUp(name, orderKey, supplier, part, lineNumber, quantity, extendedPrice, discount, tax, returnFlag, lineStatus, shipDate, commitDate, receiptDate, shipinStruct, shipMode, comment);
    }

	void LineItemIP::setUp(string name, int orderKey, Supplier *supplier, Part *part, int lineNumber, double quantity, double extendedPrice, double discount, double tax, string returnFlag,
	string lineStatus, string shipDate, string commitDate, string receiptDate, string shipinStruct, string shipMode, string comment){
		this->name = malloc <char> (strlen (name.c_str ()) + 1); strcpy (this->name, name.c_str ());
		this->orderKey = orderKey;
		this->supplier = new SupplierIP(supplier->getSupplierKey(), supplier->getName(), supplier->getAddress(), supplier->getNationKey(), supplier->getPhone(), supplier->getAccbal(), supplier->getComment());
		this->part = new PartIP(part->getKey(), part->getName(), part->getMfgr(), part->getBrand(), part->getType(), part->getSize(), part->getContainer(), part->getRetailPrice(), part->getComment());
		this->lineNumber = lineNumber;
		this->quantity = quantity;
		this->extendedPrice = extendedPrice;
		this->discount = discount;
		this->tax = tax;
		this->returnFlag = malloc <char> (strlen (returnFlag.c_str ()) + 1); strcpy (this->returnFlag, returnFlag.c_str ());
		this->lineStatus = malloc <char> (strlen (lineStatus.c_str ()) + 1); strcpy (this->lineStatus, lineStatus.c_str ());
		this->shipDate = malloc <char> (strlen (shipDate.c_str ()) + 1);  strcpy (this->shipDate, shipDate.c_str ());
		this->commitDate = malloc <char> (strlen (commitDate.c_str ()) + 1); strcpy (this->commitDate, commitDate.c_str ());
		this->receiptDate = malloc <char> (strlen (receiptDate.c_str ()) + 1); strcpy (this->receiptDate, receiptDate.c_str ());
		this->shipinStruct = malloc <char> (strlen (shipinStruct.c_str ()) + 1); strcpy (this->shipinStruct, shipinStruct.c_str ());
		this->shipMode = malloc <char> (strlen (shipMode.c_str ()) + 1); strcpy (this->shipMode, shipMode.c_str ());
		this->comment = malloc <char> (strlen (comment.c_str ()) + 1); strcpy (this->comment, comment.c_str ());
    }

	//Getters and Setters Follow:
	string LineItemIP::getName() {
		return string(name);
	}

	int LineItemIP::getOrderKey() {
		return orderKey;
	}

	int LineItemIP::getLineNumber() {
		return lineNumber;
	}

 	double LineItemIP::getQuantity() {
		return quantity;
	}

	double LineItemIP::getExtendedPrice() {
		return extendedPrice;
	}

	double LineItemIP::getDiscount() {
		return discount;
	}

	double LineItemIP::getTax() {
		return tax;
	}

	string LineItemIP::getReturnFlag() {
		return string(returnFlag);
	}

	string LineItemIP::getLineStatus() {
		return string(lineStatus);
	}

	string LineItemIP::getShipDate() {
		return string(shipDate);
	}

	string LineItemIP::getCommitDate() {
		return string(commitDate);
	}

	string LineItemIP::getReceiptDate() {
		return string(receiptDate);
	}

	string LineItemIP::getShipinStruct() {
		return string(shipinStruct);
	}

	string LineItemIP::getShipMode() {
		return string(shipMode);
	}

	string LineItemIP::getComment() {
		return string(comment);
	}

	PartIP* LineItemIP::getPart() {
		return part;
	}

	SupplierIP* LineItemIP::getSupplier() {
		return supplier;
	}

	//C++: Explicit call needed for printing content:
	string LineItemIP::toString() {
		string stringS = this->getName() + " - " + to_string(this->getOrderKey())  + " - " + to_string(this->getLineNumber()) + " - " + to_string(this->getQuantity()) + " - " + to_string(this->getExtendedPrice())
		 + " - " + to_string(this->getDiscount())  + " - " + to_string(this->getTax()) + " - " + this->getReturnFlag() + " - " + this->getShipDate() + " - " + this->getShipinStruct() + " - " + this->getShipMode() 
		 + " - " + this->getComment() + " - " + this->getLineStatus() + " - " + this->getCommitDate() + " - " + this->getReceiptDate()  +  " - " + "-supplier-" + supplier[0].toString() + "-part-" + part[0].toString(); 
		;

		return stringS;
	}


	//Implement your own custom comparator:
	bool LineItemIP::operator< (LineItemIP& other) {
		//Duplicate detection:
		if (lineNumber == other.lineNumber) {
			// They have the same "lineNumber"
			if (part->getKey() < other.getPart()->getKey()) {
				return true;
			} 
			else if (part->getKey() > other.getPart()->getKey()) {
				return false;
			} 
			else {
				// They have the same "partKey"
				if (supplier->getSupplierKey() < other.getSupplier()->getSupplierKey()) {
					return true;
				} 
				else if (supplier->getSupplierKey() > other.getSupplier()->getSupplierKey()) {
					return false;
				} 
				else {
					// They have the same "partKey" and "supplierKey"
					return false;
				}
			}
		}

		return lineNumber < other.lineNumber;	
	}