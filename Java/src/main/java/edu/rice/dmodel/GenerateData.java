package edu.rice.dmodel;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import org.apache.log4j.PropertyConfigurator;

import edu.rice.pdb.read.SerializationMethod;
import edu.rice.pdb.serialization.KryoSinglton;
import edu.rice.pdb.util.Utils;


/**
 * 
 * @author Kia
 * This class reads the TPC-H table files and generate data in various serializations together with their index files. 
 * Scale 0.1 of TPC-H dbgen generates 20000 Parts, 600000 LineItem and 15000 Customers. 
 * We store multiply each of these data sets with different factors. 
 * Parts x100 i.e., 20000x 100 = 2000000 Parts that we store in file 
 * LineItems x20 i.e., 600000 x 20 = 12000000 LineItems
 * Customers x80 i.e., 15000x80  = 1200000  Customers
 */
public class GenerateData {

	static SerializationMethod method = null;
	
	public static void main(String[] args) throws FileNotFoundException, IOException {

		PropertyConfigurator.configure("log4j.properties");

		if (args.length < 2) {
			System.out.println("Run it with the following arguments");
			System.out.println(" an int for SerializationMethod 1-JavaDefault 2-JSON 3-BSON 4-ProtocolBuffer 5-ByteBuffer 6-KRYO ");
			System.out.println(" an int scale file - inputfile should be in a directory with name tables_scale_1 1 for scale 1");
			System.out.println(" an int for data type 1 for PART, 2 for LINEITEM, 3 for CUSTOMER");
			System.exit(0);
		} else {
			int serializationType = Integer.parseInt(args[0]);

			// Write them to file with different serialization methods.
			switch (serializationType) {
			case 1:
				method = SerializationMethod.JAVADEFAULT;

				System.out.println("Running with  SerializationMethod.JAVADEFAULT");

				break;
			case 2:
				method = SerializationMethod.JSON;
				System.out.println("Running with  SerializationMethod.JSON");

				break;

			case 3:
				method = SerializationMethod.BSON;
				System.out.println("Running with  SerializationMethod.BSON");

				break;
			case 4:

				method = SerializationMethod.PROTOCOL;
				System.out.println("Running with  SerializationMethod.PROTOCOL");

				break;

			case 5:
				method = SerializationMethod.KRYO;
				System.out.println("Running with  SerializationMethod.KRYO");
				break;

			case 6:
				method = SerializationMethod.BYTEBUFFER;
				System.out.println("Running with  SerializationMethod.ByteBuffer");
				break;

			}

		}

		String PartFile = "tables_scale_" + args[1] + "/part.tbl";
		String SupplierFile = "tables_scale_" + args[1] + "/supplier.tbl";
		String OrderFile = "tables_scale_" + args[1] + "/orders.tbl";
		String LineitemFile = "tables_scale_" + args[1] + "/lineitem.tbl";
		String CustomerFile = "tables_scale_" + args[1] + "/customer.tbl";

		HashMap<Integer, Part> partMap = new HashMap<Integer, Part>(6000000);
		HashMap<Integer, Supplier> supplierMap = new HashMap<Integer, Supplier>(100000);
		HashMap<Integer, ArrayList<LineItem>> lineItemMap = new HashMap<Integer, ArrayList<LineItem>>(60000000);
		HashMap<Integer, ArrayList<Order>> orderMap = new HashMap<Integer, ArrayList<Order>>(60000000);

		ArrayList<byte[]> myByteArray = new ArrayList<byte[]>();

		
		// ####################################
		// ####################################
		// 
		//   BIG SELECT CASE BASED ON DATA TYPE
		// 
		// ####################################
		// ####################################
		
		
		int dataType = Integer.parseInt(args[2]);


		// ####################################
		// ####################################
		// ########## #########
		// ########## Part #########
		// ########## #########
		// ####################################
		// ####################################

		// READING PARTS line by line

		// CREATE TABLE PART ( P_PARTKEY INTEGER NOT NULL,
		// P_NAME VARCHAR(55) NOT NULL,
		// P_MFGR CHAR(25) NOT NULL,
		// P_BRAND CHAR(10) NOT NULL,
		// P_TYPE VARCHAR(25) NOT NULL,
		// P_SIZE INTEGER NOT NULL,
		// P_CONTAINER CHAR(10) NOT NULL,
		// P_RETAILPRICE DECIMAL(15,2) NOT NULL,
		// P_COMMENT VARCHAR(23) NOT NULL );
		System.out.println("Reading Parts ...");



		try (BufferedReader br = new BufferedReader(new FileReader(PartFile))) {
			String line;
			while ((line = br.readLine()) != null) {
				String[] data = line.split("\\|");
				Part myPart = new Part(Integer.parseInt(data[0]), data[1], data[2], data[3], data[4], Integer.parseInt(data[5]), data[6], Double.parseDouble(data[7]), data[8]);

				// index based on counter
				partMap.put(Integer.parseInt(data[0]), myPart);

				switch (method) {
				case JAVADEFAULT:
					myByteArray.add(myPart.javaDefaultSerialization());
					break;
				case JSON:
					myByteArray.add(myPart.jsonSerialization());
					break;
				case BSON:
					myByteArray.add(myPart.bsonSerialization());
					break;
				case PROTOCOL:
					myByteArray.add(myPart.protocolBufferWrite());
					break;
				case KRYO:
					myByteArray.add(myPart.kryoSerialization(KryoSinglton.getInstance().getKryo()));
					break;
				case BYTEBUFFER:
					myByteArray.add(myPart.writeByteBuffer());
					break;
				}

			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}



		
		if(dataType==1){
     	// Writing to DISK
		System.out.println("Writing Parts to disk ...");
		// Changed to 16K X 20K objects to produce = 320M Parts 
		// for Networking experiments X 2K
//		Utils.writeObjectsWithIndex(myByteArray, 2000 , "inputdata/PART" + method + ".index", "inputdata/PART" + method + ".object");
		Utils.writeObjectsWithIndex(myByteArray, 20000 , "inputdata/PART" + method + ".index", "inputdata/PART" + method + ".object");
		System.exit(0);
		}
		
		// ####################################
		// ####################################
		// ########## #########
		// ########## Supplier #########
		// ########## #########
		// ####################################
		// ####################################

		// READING Suppliers line by line

		// CREATE TABLE [dbo].[SUPPLIER](
		// [S_SUPPKEY] [int] NOT NULL,
		// [S_NAME] [char](25) NOT NULL,
		// [S_ADDRESS] [varchar](40) NOT NULL,
		// [S_NATIONKEY] [int] NOT NULL,
		// [S_PHONE] [char](15) NOT NULL,
		// [S_ACCTBAL] [decimal](15, 2) NOT NULL,
		// [S_COMMENT] [varchar](101) NOT NULL
		// );

		System.out.println("Start reading Suppliers  ...");

		try (BufferedReader br = new BufferedReader(new FileReader(SupplierFile))) {
			String line;
			while ((line = br.readLine()) != null) {
				String[] data = line.split("\\|");
				Supplier mySupplier = new Supplier(Integer.parseInt(data[0]), data[1], data[2], Integer.parseInt(data[3]), data[4], Double.parseDouble(data[5]), data[6]);
				supplierMap.put(Integer.parseInt(data[0]), mySupplier);
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		// // Activate Garbage Collector
		// System.gc();

		// ####################################
		// ####################################
		// ########## #########
		// ########## LineItem #########
		// ########## #########
		// ####################################
		// ####################################

		// CREATE TABLE [dbo].[LINEITEM](
		// [L_ORDERKEY] [int] NOT NULL,
		// [L_PARTKEY] [int] NOT NULL,
		// [L_SUPPKEY] [int] NOT NULL,
		// [L_LINENUMBER] [int] NOT NULL,
		// [L_QUANTITY] [decimal](15, 2) NOT NULL,
		// [L_EXTENDEDPRICE] [decimal](15, 2) NOT NULL,
		// [L_DISCOUNT] [decimal](15, 2) NOT NULL,
		// [L_TAX] [decimal](15, 2) NOT NULL,
		// [L_RETURNFLAG] [char](1) NOT NULL,
		// [L_LINESTATUS] [char](1) NOT NULL,
		// [L_SHIPDATE] [date] NOT NULL,
		// [L_COMMITDATE] [date] NOT NULL,
		// [L_RECEIPTDATE] [date] NOT NULL,
		// [L_SHIPINSTRUCT] [char](25) NOT NULL,
		// [L_SHIPMODE] [char](10) NOT NULL,
		// [L_COMMENT] [varchar](44) NOT NULL
		// );
		System.out.println("Start reading LineItem  ...");

		// set counter back to zero
		BufferedReader brLineItems = new BufferedReader(new FileReader(LineitemFile));
		String lineLineItem;
		myByteArray = new ArrayList<byte[]>();

		while ((lineLineItem = brLineItems.readLine()) != null) {

			String[] lineItemData = lineLineItem.split("\\|");
			int orderKey = Integer.parseInt(lineItemData[0]);
			int partKey = Integer.parseInt(lineItemData[1]);
			int supplierKey = Integer.parseInt(lineItemData[2]);

			// get the Part
			Part partTmp = null;
			if (partMap.containsKey(partKey))
				partTmp = partMap.get(partKey);
			else
				System.err.println("There is no such Part");

			// get the Supplier
			Supplier supplierTmp = null;
			if (supplierMap.containsKey(supplierKey))
				supplierTmp = supplierMap.get(supplierKey);
			else
				System.err.println("There is no such Supplier");

			LineItem tmpLineItem = new LineItem(orderKey, supplierTmp, partTmp, Integer.parseInt(lineItemData[3]), Double.parseDouble(lineItemData[4]),
					Double.parseDouble(lineItemData[5]), Double.parseDouble(lineItemData[6]), Double.parseDouble(lineItemData[7]), lineItemData[8], lineItemData[9],
					lineItemData[10], lineItemData[11], lineItemData[12], lineItemData[13], lineItemData[14], lineItemData[15]);

			// Serialize for Writting to disk
			switch (method) {
			case JAVADEFAULT:
				myByteArray.add(tmpLineItem.javaDefaultSerialization());
				break;
			case JSON:
				myByteArray.add(tmpLineItem.jsonSerialization());
				break;
			case BSON:
				myByteArray.add(tmpLineItem.bsonSerialization());
				break;
			case PROTOCOL:
				myByteArray.add(tmpLineItem.protocolBufferWrite());
				break;
			case KRYO:
				myByteArray.add(tmpLineItem.kryoSerialization(KryoSinglton.getInstance().getKryo()));
				break;
			case BYTEBUFFER:
				myByteArray.add(tmpLineItem.writeByteBuffer());
				break;
			}

			// add to memory for building orders based on them .

			if (lineItemMap.containsKey(orderKey)) {
				ArrayList<LineItem> values = lineItemMap.get(orderKey);

				values.add(tmpLineItem); // add the new one
				// put back
				lineItemMap.put(orderKey, values);

			} else {
				ArrayList<LineItem> values = new ArrayList<LineItem>();
				values.add(tmpLineItem); // add the new one
				// add for the first time
				lineItemMap.put(orderKey, values);
			}

		}

		// LineItems loaded to memory
		System.out.println("LineItems loaded to memory");




		if(dataType==2){
		// Writing to DISK
		System.out.println("Writing LineItems to disk ...");
		// 600572 X 200 = 120114400 Lineitem 
		// For networking experiments X20 
//		Utils.writeObjectsWithIndex(myByteArray, 20, "inputdata/LINEITEM" + method + ".index", "inputdata/LINEITEM" + method + ".object");
		Utils.writeObjectsWithIndex(myByteArray, 200, "inputdata/LINEITEM" + method + ".index", "inputdata/LINEITEM" + method + ".object");
		System.out.println("Wrote LineItems to disk ...");
		System.exit(0);
		}
	

		// ####################################
		// ####################################
		// ########## #########
		// ########## Order #########
		// ########## #########
		// ####################################
		// ####################################

		// Reading Orders

		// CREATE TABLE ORDERS ( O_ORDERKEY INTEGER NOT NULL,
		// O_CUSTKEY INTEGER NOT NULL,
		// O_ORDERSTATUS CHAR(1) NOT NULL,
		// O_TOTALPRICE DECIMAL(15,2) NOT NULL,
		// O_ORDERDATE DATE NOT NULL,
		// O_ORDERPRIORITY CHAR(15) NOT NULL,
		// O_CLERK CHAR(15) NOT NULL,
		// O_SHIPPRIORITY INTEGER NOT NULL,
		// O_COMMENT VARCHAR(79) NOT NULL);

		System.out.println("Start reading Orders ...");

		BufferedReader brOrders = new BufferedReader(new FileReader(OrderFile));

		String lineOrder;

		while ((lineOrder = brOrders.readLine()) != null) {
			String[] orderData = lineOrder.split("\\|");
			int orderKey = Integer.parseInt(orderData[0]);
			int customerKey = Integer.parseInt(orderData[1]);

			Order myOrder = new Order(lineItemMap.get(orderKey), orderKey, Integer.parseInt(orderData[1]), orderData[2], Double.parseDouble(orderData[3]), orderData[4],
					orderData[5], orderData[6], Integer.parseInt(orderData[7]), orderData[8]);

			if (orderMap.containsKey(customerKey)) {
				ArrayList<Order> values = orderMap.get(customerKey);
				// add the new value
				values.add(myOrder);

				// put the list back
				orderMap.put(customerKey, values);
			} else {
				ArrayList<Order> values = new ArrayList<Order>();
				values.add(myOrder); // add the new one
				// add for the first time
				orderMap.put(customerKey, values);
			}

		}

		// Orders loaded to memory
		System.out.println("Orders loaded to memory");

		brLineItems.close();
		brOrders.close();
		

		// ####################################
		// ####################################
		// ########## #########
		// ########## Customers #########
		// ########## #########
		// ####################################
		// ####################################

		// Reading Orders
		// private List<Order> orders;
		// private int custkey;
		// private String name;
		// private String address;
		// private int nationkey;
		// private String phone;
		// private double accbal;
		// private String mktsegment;
		// private String comment;
		//

		System.out.println("Start reading Customers ...");

		myByteArray = new ArrayList<byte[]>();

		BufferedReader brCustomers = new BufferedReader(new FileReader(CustomerFile));
		String lineCustomer;

		while ((lineCustomer = brCustomers.readLine()) != null) {

			String[] customerData = lineCustomer.split("\\|");
			int customerKey = Integer.parseInt(customerData[0]);

			ArrayList<Order> values = new ArrayList<Order>();
			if (orderMap.containsKey(customerKey)) {
				// if the orderMap contains an other with this id add it to the
				// List
				values = orderMap.get(customerKey);
			}

			Customer myCustomer = new Customer(values, customerKey, customerData[1], customerData[2], Integer.parseInt(customerData[3]), customerData[4],
					Double.parseDouble(customerData[5]), customerData[6], customerData[7]);
			switch (method) {
			case JAVADEFAULT:
				myByteArray.add(myCustomer.javaDefaultSerialization());
				break;
			case JSON:
				myByteArray.add(myCustomer.jsonSerialization());
				break;
			case BSON:
				myByteArray.add(myCustomer.bsonSerialization());
				break;
			case PROTOCOL:
				myByteArray.add(myCustomer.protocolBufferWrite());
				break;
			case KRYO:
				myByteArray.add(myCustomer.kryoSerialization(KryoSinglton.getInstance().getKryo()));
				break;
			case BYTEBUFFER:
				myByteArray.add(myCustomer.writeByteBuffer());
				break;
			}

		}

		// Writing to DISK
		System.out.println("Writing Customers to disk ...");
		// 15000 X 200 = 3M
		// For Networking Experiments X100 
//		Utils.writeObjectsWithIndex(myByteArray, 100,  "inputdata/CUSTOMER" + method + ".index", "inputdata/CUSTOMER" + method + ".object");
		Utils.writeObjectsWithIndex(myByteArray, 200,  "inputdata/CUSTOMER" + method + ".index", "inputdata/CUSTOMER" + method + ".object");
		System.out.println("Wrote Customers to disk ...");
		brCustomers.close();		
	}
}
