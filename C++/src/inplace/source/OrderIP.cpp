#include "OrderIP.h"
using namespace std;


	//Default constructor:
	OrderIP::OrderIP() {
		//throw invalid_argument("OrderIP: Not default constructible.");
	}


	//Constructor with arguments:
	OrderIP::OrderIP(vector<LineItem*> lineItems, int numLineItemsSupplied, int orderKey, int custKey, string orderStatus, double totalPrice, string orderDate, string orderPriority, string clerk, int shipPriority, string comment) {
		this->setUp(lineItems, numLineItemsSupplied, orderKey, custKey, orderStatus, totalPrice, orderDate, orderPriority, clerk, shipPriority, comment);
	}


	//Constructor with arguments:
	void OrderIP::setUp(vector<LineItem*> lineItems, int numLineItemsSupplied, int orderKey, int custKey, string orderStatus, double totalPrice, string orderDate, string orderPriority, string clerk, int shipPriority, string comment) {
		this->lineItems = new LineItemIP [numLineItemsSupplied];
		for (int i = 0; i < numLineItemsSupplied; i++) {
			(this->lineItems)[i].setUp(lineItems.at(i)->getName(), lineItems.at(i)->getOrderKey(), lineItems.at(i)->getSupplier(), lineItems.at(i)->getPart(), lineItems.at(i)->getLineNumber(), lineItems.at(i)->getQuantity(), lineItems.at(i)->getExtendedPrice(), lineItems.at(i)->getDiscount(), lineItems.at(i)->getTax(), lineItems.at(i)->getReturnFlag(),
									lineItems.at(i)->getLineStatus(), lineItems.at(i)->getShipDate(), lineItems.at(i)->getCommitDate(), lineItems.at(i)->getReceiptDate(), lineItems.at(i)->getShipinStruct(), lineItems.at(i)->getShipMode(), lineItems.at(i)->getComment());

		}

		this->numLineItemsSupplied = numLineItemsSupplied; 
		this->orderKey = orderKey;
		this->custKey = custKey;
		this->orderStatus = malloc <char> (strlen (orderStatus.c_str ()) + 1); strcpy (this->orderStatus, orderStatus.c_str ());
		this->totalPrice = totalPrice;
		this->orderDate = malloc <char> (strlen (orderDate.c_str ()) + 1); strcpy (this->orderDate, orderDate.c_str ());
		this->orderPriority = malloc <char> (strlen (orderPriority.c_str ()) + 1); strcpy (this->orderPriority, orderPriority.c_str ());
		this->clerk = malloc <char> (strlen (clerk.c_str ()) + 1); strcpy (this->clerk, clerk.c_str ());
		this->shipPriority = shipPriority;
		this->comment = malloc <char> (strlen (comment.c_str ()) + 1); strcpy (this->comment, comment.c_str ());
	}


	//Getters Follow:
	int OrderIP::getLineItemsCount() {
		return numLineItemsSupplied;
	}

	LineItemIP * OrderIP::getLineItems(){
		return lineItems;
	}

	int OrderIP::getOrderKey() {
		return orderKey;
	}

	int OrderIP::getCustKey() {
		return custKey;
	}

	double OrderIP::getTotalPrice() {
		return totalPrice;
	}

	string OrderIP::getOrderStatus() {
		return string(orderStatus);
	}

	string OrderIP::getOrderDate() {
		return string(orderDate);
	}

	string OrderIP::getOrderPriority() {
		return string(orderPriority);
	}

	string OrderIP::getClerk() {
		return string(clerk);
	}

	int OrderIP::getShipPriority() {
		return shipPriority;
	}

	string OrderIP::getComment() {
		return string(comment);
	}

	//C++: Explicit call needed for printing content:
	string OrderIP::toString() {
		string stringS = to_string(this->getOrderKey()) + " - " + to_string(this->getCustKey())  + " - " + this->getOrderStatus() + " - " + to_string(this->getTotalPrice()) + " - " + this->getOrderDate()+ " - " + this->getOrderPriority()
		 + " - " + this->getClerk() + " - " + to_string(this->getShipPriority()) + " - " + this->getComment() ; 
		;
	
		//All lineitems:
		for (int i = 0; i < numLineItemsSupplied; i++) {
			stringS += " LineItem " + to_string(i) + " : " + lineItems[i].toString(); 
		}

		return stringS;
	}


	//Implement your own custom comparator:
	bool OrderIP::operator< (OrderIP& other) {

		if (orderKey == other.orderKey) {
			bool result = false;

			if (getLineItemsCount() == other.getLineItemsCount()) {
				for (int i = 0; i < getLineItemsCount(); i++) {
					result = lineItems[i] < (other.getLineItems()[i]);
					if (result) return true;
				}
			}

			return getLineItemsCount() < other.getLineItemsCount();		
		}

		return orderKey < other.orderKey;	
	}
