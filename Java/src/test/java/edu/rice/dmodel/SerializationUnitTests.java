package edu.rice.dmodel;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.json.Json;
import javax.json.JsonObject;
import javax.json.JsonWriter;
import javax.json.JsonWriterFactory;
import javax.json.stream.JsonGenerator;

import org.apache.log4j.PropertyConfigurator;
import org.codehaus.jackson.map.ObjectMapper;
import org.junit.Test;

import edu.rice.pdb.randomano.SerializationMethod;
import edu.rice.pdb.randomano.WriteAndRead;
import edu.rice.pdb.serialization.KryoSinglton;
import edu.rice.pdb.util.Utils;

public class SerializationUnitTests {



	public SerializationUnitTests() {
		PropertyConfigurator.configure("log4j.properties");

	}

	@Test
	public void partTest() {
		PropertyConfigurator.configure("log4j.properties");

		Part p = new Part();

		p.setBrand("BRAND");
		p.setName("NAME");
		p.setContainer("Container");
		p.setComment("Comment");
		p.setMfgr("MFGR");
		p.setType("Type");
		p.setSize(1);

		byte[] buf = p.jsonSerialization();

		Part p2 = new Part();
		p2 = p2.jsonDeserialization(buf);

		assertEquals(p.getName(), p2.getName());
	}

	@Test
	public void kryoElementTest() {

		Element myElement = new Element();
		long x=1;

		myElement.setMyInteger(x);
		myElement.setMyDouble(1.0);

		byte[] buf = myElement.kryoSerialization(KryoSinglton.getInstance().getKryo());

		// System.out.println(buf.length);

		Element newData = (Element) myElement.kryoDeserialization(buf, myElement.getClass(), KryoSinglton.getInstance().getKryo());

		// System.out.println(newData.getMyDouble());

		assertEquals(myElement.getMyDouble(), newData.getMyDouble());

	}

	@Test
	public void byteBufferPartTest() {

		Part myPart = new Part(1, "tmpName", "tmpMfgr", "tmpBrand", "tmpType", 1, "tmpContainer", 1.0, "tmpComment");

		byte[] buf = myPart.writeByteBuffer();

		Part newPart = (Part) myPart.readByteBuffer(buf);

		assertEquals(myPart.getPartID(), newPart.getPartID());

	}

	@Test
	public void byteBufferSupplierTest() {

		Supplier mySupplier = new Supplier(1, "name", "address", 1, "phone", 1.0, "comment");
		byte[] buf = mySupplier.writeByteBuffer();

		Supplier newSupplier = (Supplier) mySupplier.readByteBuffer(buf);

		assertEquals(mySupplier.getSupplierKey(), newSupplier.getSupplierKey());

	}

	@Test
	public void byteBufferLineItemTest() {

		Supplier mySupplier = new Supplier(1, "name", "address", 1, "phone", 1.0, "comment");
		Part myPart = new Part(1, "tmpName", "tmpMfgr", "tmpBrand", "tmpType", 1, "tmpContainer", 1.0, "tmpComment");

		int i = 1;

		LineItem myLineItem = new LineItem(i, mySupplier, myPart, i, i, i, i, i, "a" + i, "a" + i, "Data-" + i, "Data-" + i, "Data-" + i, "a" + i, "a" + i, "comment" + i);

		byte[] buf = myLineItem.writeByteBuffer();

		LineItem newLineItem = (LineItem) myLineItem.readByteBuffer(buf);

		assertEquals(myLineItem.getSupplier().getSupplierKey(), newLineItem.getSupplier().getSupplierKey());
		assertTrue(myLineItem.getDiscount() == newLineItem.getDiscount());
		assertEquals(myLineItem.getPart().getPartID(), newLineItem.getPart().getPartID());
	}

	@Test
	public void byteBufferOrderTest() {

		Supplier mySupplier = new Supplier(1, "name", "address", 1, "phone", 1.0, "comment");
		Part myPart = new Part(1, "tmpName", "tmpMfgr", "tmpBrand", "tmpType", 1, "tmpContainer", 1.0, "tmpComment");

		int i = 1;

		LineItem myLineItem = new LineItem(i, mySupplier, myPart, i, i, i, i, i, "a" + i, "a" + i, "Data-" + i, "Data-" + i, "Data-" + i, "a" + i, "a" + i, "comment" + i);

		ArrayList<LineItem> lineItemList = new ArrayList<LineItem>();
		lineItemList.add(myLineItem);
		lineItemList.add(myLineItem);

		Order myOrder = new Order(lineItemList, i, i, "1", i, "1" + i, "1", "clerk1", i, "comment1");

		byte[] buf = myOrder.writeByteBuffer();

		// System.out.println(buf.length);

		Order newOrder = (Order) myOrder.readByteBuffer(buf);

		assertEquals(myOrder.getLineItems().size(), newOrder.getLineItems().size());
		assertEquals(myOrder.getLineItems().get(0).getComment(), newOrder.getLineItems().get(0).getComment());

	}

	@Test
	public void jsonGZIPTest() {
		Supplier mySupplier = new Supplier(1, "name", "address", 1, "phone", 1.0, "comment");

		byte[] bufSupplier = mySupplier.jsonSerialization();

		Supplier newSupplier = (Supplier) mySupplier.jsonDeserialization(bufSupplier);

		assertEquals(mySupplier.getSupplierKey(), newSupplier.getSupplierKey());

	}

	@Test
	public void bsonTest() {
		// Supplier mySupplier = new Supplier(1, "name", "address", 1, "phone", 1.0, "comment");
		Part myPart = new Part(1, "tmpName", "tmpMfgr", "tmpBrand", "tmpType", 1, "tmpContainer", 1.0, "tmpComment");

		byte[] bufSupplier = myPart.bsonSerialization();

		Part newPart = (Part) myPart.bsonDeSerialization(bufSupplier, myPart.getClass());

		assertEquals(myPart.getPartID(), newPart.getPartID());
	}

	
	
	@Test
	public void protocolBufferSerialiyationTest() {
		Part myPart = new Part(1, "tmpName", "tmpMfgr", "tmpBrand", "tmpType", 1, "tmpContainer", 1.0, "tmpComment");
		System.out.println(myPart);

		try {

			byte[] byteObjects = myPart.protocolBufferWrite();

			ArrayList<byte[]> objectList = new ArrayList<byte[]>();
			for (int i = 0; i < 1000121; i++) {
				objectList.add(byteObjects);
			}

			Utils.writeObjectsWithIndex(objectList, 1,  "protocolBufferSerialiyationTest.index", "protocolBufferSerialiyationTest.object");

			// Part
			// Calculate the time only before and after reading objects from file read not sequentially
			ArrayList<RootData> results1 = WriteAndRead.readObjects(SerializationMethod.PROTOCOL, new Part(), 658, 658,  "protocolBufferSerialiyationTest.index", "protocolBufferSerialiyationTest.object", false, true);
			Part newPart = (Part) results1.get(1);
			
			
			System.out.println(newPart);
			
			assertEquals(myPart.getName(), newPart.getName());

			// Read sequentially
			ArrayList<RootData> results2 = WriteAndRead.readObjects(SerializationMethod.PROTOCOL, new Part(),658, 658, "protocolBufferSerialiyationTest.index", "protocolBufferSerialiyationTest.object", true, false);

			System.out.println(results2.size());
			Part newPart2 = (Part) results2.get(1);
			
			System.out.println(newPart2);
			
			assertEquals(myPart.getName(), newPart2.getName());
			
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}

	}
	
	
	@Test
	public void testIndexSerialization() {
		
		long[] indexPositions = new long[300];
		int[] indexLenghts = new int[20];
		
//		System.out.println(indexPositions.length);
		
		// 2147483647
		for (int i = 0; i < 300; i++) {
			indexPositions[i] = (long) i;
		}
		
		for (int i = 0; i < 20; i++) {
			indexLenghts[i]=i;
		}
		
		Utils.writeIndex(indexPositions, indexLenghts ,  "testIndex.index");
		
		
		
		IndexData indexNew=Utils.readIndex("testIndex.index", 0);
		
		
		System.out.println("indexNew.getLenghts().length  "+indexNew.getLenghts().length);
		
		int i=123;
		System.out.print(indexPositions[i]+  " should be equal to  ");
		System.out.println(indexNew.getStarts()[i]);

		
		assertEquals(indexPositions[i], indexNew.getStarts()[i]);
	
	}
	
	
	

	@Test
	public void jsonElementTest() {
		Element myElement = new Element((long)1, 1.0);

		byte[] buf = myElement.jsonSerialization();

		Element newSupplier = (Element) myElement.jsonDeserialization(buf);

		assertEquals(myElement.getMyInteger(), newSupplier.getMyInteger());

	}
	
	
	
	
	@Test
	public void jsonElementFileTest() {


		ArrayList<byte[]> objectList = new ArrayList<byte[]>();
		for (int i = 0; i < 100; i++) {
			Element myElement = new Element((long)1, 1.0);
			
			byte[] byteObjects = myElement.jsonSerialization();
			objectList.add(byteObjects);
		}


		try {

	
			Utils.writeObjectsWithIndex(objectList, 1, "testElement.index", "testElement.object");



			// Read sequentially
			ArrayList<RootData> results2 = WriteAndRead.readObjects(SerializationMethod.JSON, new Element(), 20, 20, "testElement.index", "testElement.object", true, true);

			Element newPart2 = (Element) results2.get(15);
			
			System.out.println( newPart2.getMyInteger());
			
			
			assertTrue(1==newPart2.getMyInteger());
			
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}

	}
	
	
	

	@Test
	public void comapareLineItemTest() {

		Supplier mySupplier = new Supplier(1, "name", "address", 1, "phone", 1.0, "comment");
		Part myPart = new Part(1, "tmpName", "tmpMfgr", "tmpBrand", "tmpType", 1, "tmpContainer", 1.0, "tmpComment");

		int i = 1;

		LineItem myLineItem = new LineItem(i, mySupplier, myPart, i, i, i, i, i, "a" + i, "a" + i, "Data-" + i, "Data-" + i, "Data-" + i, "a" + i, "a" + i, "comment" + i);
		LineItem myLineItem1 = new LineItem(i, mySupplier, myPart, i, i, i, i, i, "a" + i, "a" + i, "Data-" + i, "Data-" + i, "Data-" + i, "a" + i, "a" + i, "comment" + i);

//		System.out.println("Here is the result "+myLineItem.compareTo(myLineItem1));
		
		assertEquals(myLineItem.compareTo(myLineItem1), 0);
	}
	
	
	
	@Test
	public void writeAndReadTestJSON() {

		try {
			ArrayList<byte[]> objectList = new ArrayList<byte[]>();
			ArrayList<Part> myObjects = new ArrayList<Part>();

			for (int i = 0; i < 10000; i++) {
				Part myPart = new Part(i, "tmpName"+i, "tmpMfgr", "tmpBrand", "tmpType", i, "tmpContainer", 1.0, "tmpComment");
				myObjects.add(myPart);
				
				byte[] byteObjects = myPart.jsonSerialization();

				objectList.add(byteObjects);
			}

			Utils.writeObjectsWithIndex(objectList, 1,  "jsonSerialiyationTest.index", "jsonSerialiyationTest.object");

//			// Part
//			// Calculate the time only before and after reading objects from file read not sequentially
			ArrayList<RootData> results1 = WriteAndRead.readObjects(SerializationMethod.JSON, new Part(), 3001, 4000, "jsonSerialiyationTest.index", "jsonSerialiyationTest.object", true, false);
			Part newPart = (Part) results1.get(0);
			
			
			System.out.println(newPart);
			assertEquals(myObjects.get(0).getName(), newPart.getName());
			
		} catch (Exception  e) {
			e.printStackTrace();
		}

	}
	
	
	@Test
	public void writeAndReadTestProtocolBuffer() {

		try {
			ArrayList<byte[]> objectList = new ArrayList<byte[]>();
			ArrayList<Part> myObjects = new ArrayList<Part>();

			for (int i = 0; i < 4000; i++) {
				Part myPart = new Part(i, "tmpName"+i, "tmpMfgr", "tmpBrand", "tmpType", i, "tmpContainer", 1.0, "tmpComment");
				myObjects.add(myPart);
				
				byte[] byteObjects = myPart.protocolBufferWrite();
//				byte[] byteObjects = myPart.jsonSerialization();

				objectList.add(byteObjects);
			}

			Utils.writeObjectsWithIndex(objectList, 1,  "protocolBufferSerialiyationTest.index", "protocolBufferSerialiyationTest.object");

//			// Part
//			// Calculate the time only before and after reading objects from file read not sequentially
			ArrayList<RootData> results1 = WriteAndRead.readObjects(SerializationMethod.PROTOCOL, new Part(), 1502, 3000, "protocolBufferSerialiyationTest.index", "protocolBufferSerialiyationTest.object", true, false);
			Part newPart = (Part) results1.get(0);
			
			
			System.out.println(newPart);
			assertEquals(myObjects.get(0).getName(), newPart.getName());
			
		} catch (Exception  e) {
			e.printStackTrace();
		}

	}
	
	

	@Test
	public void objectSizeTest() {
		// Supplier mySupplier = new Supplier(1, "name", "address", 1, "phone", 1.0, "comment");
		Part myPart = new Part(1, "t", "t", "t", "t", 1, "t", 1.0, "t");

		byte[] bufSupplier = myPart.bsonSerialization();

		System.out.println("Size of Object : " +bufSupplier.length);
		
//		Part newPart = (Part) myPart.bsonDeSerialization(bufSupplier, myPart.getClass());
//
//		assertEquals(myPart.getPartID(), newPart.getPartID());
	}

	@Test
	public void vectorAggregationTest() {

		ArrayList<RootData> mydata1 = new Element().generateRandomObjects(20, 5);
		ArrayList<RootData> mydata2 = new Element().generateRandomObjects(20, 5);
		ArrayList<RootData> mydata3 = new Element().generateRandomObjects(20, 5);
		
		
		
		
		System.out.println("MyData1");
		for (RootData rootData : mydata1) {
			Element e=(Element) rootData; 
			System.out.println("(" + e.getMyInteger() +" , " +e.getMyDouble() +")");
		}
	
		System.out.println("MyData2");
		for (RootData rootData : mydata2) {
			Element e=(Element) rootData; 
			System.out.println("(" + e.getMyInteger() +" , " +e.getMyDouble() +")");
		}
	
		
		System.out.println("MyData3");
		for (RootData rootData : mydata3) {
			Element e=(Element) rootData; 
			System.out.println("(" + e.getMyInteger() +" , " +e.getMyDouble() +")");
		}
		
		ArrayList<RootData> aggegated=Utils.aggergateSparseVector(mydata1, mydata2, mydata3);
		
		System.out.println("aggegated");
		for (RootData rootData : aggegated) {
			Element e=(Element) rootData; 
			System.out.println("(" + e.getMyInteger() +" , " +e.getMyDouble() +")");
		}
		System.out.println("aggegated size: "+aggegated.size());
		
		
	}
	

	@Test
	public void vectorAggregationTest2() {
		
		System.out.println("Created Vector 1. ");
		ArrayList<RootData> mydata1 = new Element().generateRandomObjects(100000000, 10000000);
		
		System.out.println("Created Vector 2. ");
		ArrayList<RootData> mydata2 = new Element().generateRandomObjects(100000000, 10000000);
		
		System.out.println("Created Vector 3. ");
		ArrayList<RootData> mydata3 = new Element().generateRandomObjects(100000000, 10000000);
		
		long startTime = System.nanoTime();
		
		System.out.println("Starting Vector Aggregation...");

		
		ArrayList<RootData> aggegated=Utils.aggergateSparseVector(mydata1, mydata2, mydata3);
		
		
		long endTime = System.nanoTime();

		double elapsedTotalTime = (endTime - startTime) / 1000000000.0;

		System.out.println("Aggregation Time in Sec is : "+String.format("%.9f", elapsedTotalTime));

		System.out.println("aggegated size: "+aggegated.size());
		
		
		
		
		
	}
	
	
	
	@Test
	public void CustomerJsonTest() {

		Supplier mySupplier = new Supplier(1, "name", "address", 1, "phone", 1.0, "comment");
		Part myPart = new Part(1, "tmpName", "tmpMfgr", "tmpBrand", "tmpType", 1, "tmpContainer", 1.0, "tmpComment");

		int i = 1;

		LineItem myLineItem = new LineItem(i, mySupplier, myPart, i, i, i, i, i, "a" + i, "a" + i, "Data-" + i, "Data-" + i, "Data-" + i, "a" + i, "a" + i, "comment" + i);
		LineItem myLineItem1 = new LineItem(i, mySupplier, myPart, i, i, i, i, i, "a" + i, "a" + i, "Data-" + i, "Data-" + i, "Data-" + i, "a" + i, "a" + i, "comment" + i);

		List<LineItem> myLineItemsList = new ArrayList<LineItem>(); 
		myLineItemsList.add(myLineItem);
		myLineItemsList.add(myLineItem1);		
		
		Order myOrder =new Order(myLineItemsList, 1, 1, "orderstatus",  1.0 , "orderdate", "orderpriority", "clerk", 1, "comment");
		Order myOrder1 =new Order(myLineItemsList, 1, 2, "orderstatus",  1.0 , "orderdate", "orderpriority", "clerk", 1, "comment");
		
		List<Order> myOrderList = new ArrayList<Order>(); 
		
		myOrderList.add(myOrder);
		myOrderList.add(myOrder1);
		
		Customer myCustomer = new Customer(myOrderList, 1, "name", "address", 1, "phone", 1.0 , "mktsegment", "comment");
		  
		JsonObject  myJsonObject = myCustomer.jsonObjectBuilder();
		

        Map<String, Object> properties = new HashMap<>(1);
        properties.put(JsonGenerator.PRETTY_PRINTING, true);
        StringWriter sw = new StringWriter();
        
        JsonWriterFactory writerFactory = Json.createWriterFactory(properties);
        JsonWriter jsonWriter = writerFactory.createWriter(sw);

        jsonWriter.writeObject(myJsonObject);
        jsonWriter.close();
        
        
		
		System.out.println(sw.toString());
		
		
		
	}


}
