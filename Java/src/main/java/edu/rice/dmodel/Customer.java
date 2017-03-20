package edu.rice.dmodel;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonArrayBuilder;
import javax.json.JsonObject;
import javax.json.JsonObjectBuilder;
import javax.json.JsonReader;
import javax.json.JsonWriter;
import javax.json.JsonWriterFactory;
import javax.json.stream.JsonGenerator;

import com.google.protobuf.CodedInputStream;

//import edu.rice.pdb.serialization.ProtocolBufferSinglton;
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP;
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP;
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP;
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP.PartP;
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP.SupplierP;

public class Customer extends Base implements RootData {

	private static final long serialVersionUID = 8868016127075395583L;

	public Customer(List<Order> orders, int custkey, String name, String address, int nationkey, String phone, double accbal, String mktsegment, String comment) {
		super();
		this.orders = orders;
		this.custkey = custkey;
		this.name = name;
		this.address = address;
		this.nationkey = nationkey;
		this.phone = phone;
		this.accbal = accbal;
		this.mktsegment = mktsegment;
		this.comment = comment;
	}

	private List<Order> orders;
	private int custkey;
	private String name;
	private String address;
	private int nationkey;
	private String phone;
	private double accbal;
	private String mktsegment;
	private String comment;

	public Customer() {

	}

	public List<Order> getOrders() {
		return orders;
	}

	public void setOrders(List<Order> orders) {
		this.orders = orders;
	}

	public int getCustkey() {
		return custkey;
	}

	public void setCustkey(int custkey) {
		this.custkey = custkey;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public int getNationkey() {
		return nationkey;
	}

	public void setNationkey(int nationkey) {
		this.nationkey = nationkey;
	}

	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	public double getAccbal() {
		return accbal;
	}

	public void setAccbal(double accbal) {
		this.accbal = accbal;
	}

	public String getMktsegment() {
		return mktsegment;
	}

	public void setMktsegment(String mktsegment) {
		this.mktsegment = mktsegment;
	}

	public String getComment() {
		return comment;
	}

	public void setComment(String comment) {
		this.comment = comment;
	}

	@Override
	public int compareTo(RootData data) {
		
		Customer m_Customer = (Customer) data;
		
		if (m_Customer.getCustkey() == this.getCustkey()) {
			for (int i = 0; i < m_Customer.orders.size(); i++) {
				if(m_Customer.orders.get(i).compareTo(this.orders.get(i)) < 1 ){
					return -1;
				}
			}
		}
		return 1;
	}

	@Override
	public List<RootData> generateObjects(int number) {

		ArrayList<RootData> listOfCustomers = new ArrayList<RootData>();

		for (int i = 0; i < number; i++) {
			Customer myCustomer = new Customer();

			ArrayList<RootData> listOfRootData = new Order().generateObjects(10);
			ArrayList<Order> listOfOrders = new ArrayList<Order>();

			// Casting
			for (RootData tmp : listOfRootData) {
				listOfOrders.add((Order) tmp);
			}

			myCustomer.setOrders(listOfOrders);
			myCustomer.setCustkey(i);
			myCustomer.setName("name" + i);
			myCustomer.setAddress("Address" + i);
			myCustomer.setNationkey(i);
			myCustomer.setPhone("phone" + i);
			myCustomer.setAccbal(i);
			myCustomer.setMktsegment("mktsegment" + i);
			myCustomer.setComment("comment" + i);

			listOfCustomers.add(myCustomer);

		}

		return listOfCustomers;
	}

	@Override
	public RootData javaDefaultDeserialization(byte[] buf) {
		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		ObjectInputStream objectInputStream;

		Customer p = null;

		try {
			objectInputStream = new ObjectInputStream(b);
			p = (Customer) objectInputStream.readObject();

		} catch (IOException | ClassNotFoundException e) {
			logger.error("Can read object Part from byteArray", e);
		}
		return p;
	}

	@Override
	public byte[] jsonSerialization() {
		// Set the properties
		Map<String, Object> properties = new HashMap<String, Object>(1);
		properties.put(JsonGenerator.PRETTY_PRINTING, true);

		JsonWriterFactory writerFactory = Json.createWriterFactory(properties);

		ByteArrayOutputStream bos = new ByteArrayOutputStream();
        try {
			GZIPOutputStream gzipOutputStream = new GZIPOutputStream(bos);
			JsonWriter jsonWriter = writerFactory.createWriter(gzipOutputStream);
			jsonWriter.writeObject(this.jsonObjectBuilder());
			jsonWriter.close();
		} catch (IOException e) {
			logger.error(e);		
		}
		// Get the bytes of the serialized JSON object
		byte[] buf = bos.toByteArray();
		return buf;
	}

	public JsonObject jsonObjectBuilder() {
		JsonObjectBuilder orderJsonObjectBuilder = Json.createObjectBuilder();

		// add all of the attributes of LineItem
		orderJsonObjectBuilder.add("custkey", this.getCustkey());
		orderJsonObjectBuilder.add("name", this.getName());
		orderJsonObjectBuilder.add("address", this.getAddress());
		orderJsonObjectBuilder.add("nationkey", this.getNationkey());
		orderJsonObjectBuilder.add("phone", this.getPhone());
		orderJsonObjectBuilder.add("accbal", this.getAccbal());
		orderJsonObjectBuilder.add("mktsegment", this.getMktsegment());
		orderJsonObjectBuilder.add("comment", this.getComment());

		// Order are a list
		JsonArrayBuilder jsonOrdersArray = Json.createArrayBuilder();

		for (Order myOrder : this.orders) {

			jsonOrdersArray.add(myOrder.jsonObjectBuilder());
		}
		// add the Parts as JSON array to the lineitemBuilder
		orderJsonObjectBuilder.add("orders", jsonOrdersArray);

		JsonObject lineitemJsonObject = orderJsonObjectBuilder.build();
		return lineitemJsonObject;
	}

	public Customer readJSONCustomer(JsonObject jsonObject) {
		Customer myCustomer = new Customer();

		myCustomer.setCustkey(jsonObject.getInt("custkey"));
		myCustomer.setName(jsonObject.getString("name"));
		myCustomer.setAddress(jsonObject.getString("address"));
		myCustomer.setNationkey(jsonObject.getInt("nationkey"));
		myCustomer.setPhone(jsonObject.getString("phone"));
		myCustomer.setAccbal(jsonObject.getInt("accbal"));
		myCustomer.setMktsegment(jsonObject.getString("mktsegment"));
		myCustomer.setComment(jsonObject.getString("comment"));

		// reading LineItem arrays from json
		JsonArray jsonOrder = jsonObject.getJsonArray("orders");
		ArrayList<Order> orders = new ArrayList<Order>();
		Order morder = new Order();

		for (int i = 0; i < jsonOrder.size(); i++) {
			JsonObject m_LineItemJsonObject = jsonOrder.getJsonObject(i);
			orders.add(morder.readJSONLineItem(m_LineItemJsonObject));
		}
		myCustomer.setOrders(orders);

		return myCustomer;
	}

	@Override
	public RootData jsonDeserialization(byte[] buf) {
		Customer myCustomer = new Customer();

		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		
		try {
			InputStream gzis = new GZIPInputStream(b);
			// create JsonReader object
			JsonReader jsonReader = Json.createReader(gzis);

			// get JsonObject from JsonReader
			JsonObject jsonObject = jsonReader.readObject();
			// we can close IO resource and JsonReader now
			jsonReader.close();
			b.close();

			myCustomer = readJSONCustomer(jsonObject);

			// we can close IO resource and JsonReader now
			jsonReader.close();

		} catch (Exception e) {
			logger.error("Can not read JSON object Part from byteArray", e);
		}

		return myCustomer;
	}

	
	
	/*
	 * Write a serializes a customer object using google protocol buffer.
	 * 
	 * @see edu.rice.dmodel.RootData#protocolBufferWrite()
	 */
	@Override
	public byte[] protocolBufferWrite() {

		CustomerP.Builder customerP = CustomerP.newBuilder();

		customerP.setCustkey(this.custkey);
		customerP.setName(this.name);
		customerP.setAddress(this.address);
		customerP.setNationkey(this.nationkey);
		customerP.setPhone(this.phone);
		customerP.setAccbal(this.accbal);
		customerP.setMktsegment(this.mktsegment);
		customerP.setComment(this.comment);

		for (Order order : this.orders) {
			OrderP.Builder morder = OrderP.newBuilder();

			morder.setOrderStatus(order.getOrderstatus());
			morder.setTotalPrice(order.getTotalprice());
			morder.setOrderDate(order.getOrderdate());
			morder.setOrderPriority(order.getOrderpriority());
			morder.setClerk(order.getClerk());
			morder.setShipPeriority(order.getShippriority());
			morder.setComment(order.getComment());

			for (LineItem lineItem : order.getLineItems()) {

				LineItemP.Builder m_lineItem = LineItemP.newBuilder();

				m_lineItem.setLineNumber(lineItem.getOrderKey());
				m_lineItem.setLineNumber(lineItem.getLineNumber());
				m_lineItem.setQuantity(lineItem.getQuantity());
				m_lineItem.setExtendedPrice(lineItem.getExtendedPrice());
				m_lineItem.setDiscount(lineItem.getDiscount());
				m_lineItem.setTax(lineItem.getTax());
				m_lineItem.setReturnFlag(lineItem.getReturnFlag());
				m_lineItem.setLineStatus(lineItem.getLineStatus());
				m_lineItem.setShipDate(lineItem.getShipDate());
				m_lineItem.setCommitDate(lineItem.getCommitDate());
				m_lineItem.setReceiptDate(lineItem.getReceiptDate());
				m_lineItem.setShipinStruct(lineItem.getShipinStruct());
				m_lineItem.setShipMode(lineItem.getShipMode());
				m_lineItem.setShipMode(lineItem.getShipMode());
				m_lineItem.setComment(lineItem.getComment());

				SupplierP.Builder m_supplier = SupplierP.newBuilder();

				m_supplier.setName(lineItem.getSupplier().getName());
				m_supplier.setAddress(lineItem.getSupplier().getAddress());
				m_supplier.setSupplierKey(lineItem.getSupplier().getSupplierKey());
				m_supplier.setNationKey(lineItem.getSupplier().getNationKey());
				m_supplier.setPhone(lineItem.getSupplier().getPhone());
				m_supplier.setAccbal(lineItem.getSupplier().getAccbal());
				m_supplier.setComment(lineItem.getSupplier().getComment());

				m_lineItem.setSupplierP(m_supplier);

				PartP.Builder m_part = PartP.newBuilder();

				m_part.setName(lineItem.getPart().getName());
				m_part.setType(lineItem.getPart().getType());
				m_part.setSize(lineItem.getPart().getSize());
				m_part.setBrand(lineItem.getPart().getBrand());
				m_part.setMfgr(lineItem.getPart().getMfgr());
				m_part.setPartID(lineItem.getPart().getPartID());
				m_part.setRetailPrice(lineItem.getPart().getRetailPrice());
				m_part.setContainer(lineItem.getPart().getContainer());
				m_part.setComment(lineItem.getPart().getComment());

				m_lineItem.setPartP(m_part);

				// add the lineItem to the order
				morder.addLineItem(m_lineItem);

			}

			// Add the Supplier to the list of the lineItem
			customerP.addOrder(morder);

		}

		byte[] buf = null;

		ByteArrayOutputStream bos = new ByteArrayOutputStream();

		// Write the class room to disk.
		try {
			customerP.build().writeTo(bos);
			// Get the bytes of the serialized JSON object
			buf = bos.toByteArray();
			bos.close();

		} catch (IOException e) {
			logger.error("Protocol Buffer Writer in Part ", e);
		}

		return buf;

	}

	
	

	
	@Override
	public byte[] writeByteBuffer() {


		
//		private int custkey; // 4 bytes 
//		private int nationkey; // 4 bytes 
//		private double accbal;  // 8 bytes 
		 
//		private String name;
//		private String address;
//		private String phone;
//		private String mktsegment;
//		private String comment;

//		private List<Order> orders;


		
		// Container
		byte[] nameBytes = name.getBytes(Charset.forName("UTF-8"));
		byte[] addressBytes = address.getBytes(Charset.forName("UTF-8"));
		byte[] phoneBytes = phone.getBytes(Charset.forName("UTF-8"));
		byte[] mktsegmentBytes = mktsegment.getBytes(Charset.forName("UTF-8"));
		byte[] commentBytes = comment.getBytes(Charset.forName("UTF-8"));


		ArrayList<byte[]> customerBytes = new ArrayList<byte[]>();

		int lengthOfOrder = 0;

		for (Order tmpOrder : orders) {
			byte[] tmpOrderBytes = tmpOrder.writeByteBuffer();
			customerBytes.add(tmpOrderBytes);
			
			// 4 bytes is for an additional integer for write the size byteArray			
			lengthOfOrder = lengthOfOrder + tmpOrderBytes.length+4;
		}

		// allocate a buffer to write this object
		// an additional 4 bytes for the number of LineItem in the list
		ByteBuffer byteBuffer = ByteBuffer.allocate(32 + nameBytes.length + addressBytes.length + addressBytes.length + mktsegmentBytes.length +commentBytes.length+ commentBytes.length + lengthOfOrder);

		
		byteBuffer.putInt(custkey); // 4 bytes
		byteBuffer.putInt(nationkey); // 4 bytes
		byteBuffer.putDouble(accbal); // 8 bytes

		// name
		byteBuffer.putInt(nameBytes.length); // 4 bytes size of
		byteBuffer.put(nameBytes);

		// addressBytes
		byteBuffer.putInt(addressBytes.length); // 4 bytes size of
		byteBuffer.put(addressBytes);

		// phoneBytes
		byteBuffer.putInt(phoneBytes.length); // 4 bytes size of
		byteBuffer.put(phoneBytes);

		// mktsegmentBytes
		byteBuffer.putInt(mktsegmentBytes.length); // 4 bytes size of
		byteBuffer.put(mktsegmentBytes);
		
		
		// commentBytes
		byteBuffer.putInt(commentBytes.length); // 4 bytes size of
		byteBuffer.put(commentBytes);
		
		
		// first write how many items have to be read from the buffer
		// number of LineItems in the list
		byteBuffer.putInt(customerBytes.size());

		for (byte[] myOrderBytes : customerBytes) {
			// Add each lineItem byte array one bye one
			byteBuffer.putInt(myOrderBytes.length);
			byteBuffer.put(myOrderBytes);
		}

		return byteBuffer.array();
	}

	
	
	@Override
	public RootData protocolBufferRead(byte[] buf) {

		Customer mCustomer = new Customer();
		ByteArrayInputStream b = new ByteArrayInputStream(buf);

		CustomerP.Builder protocCustomer = CustomerP.newBuilder();
		CodedInputStream stream = CodedInputStream.newInstance(b);

		// mergeFrom
		try {
			protocCustomer.mergeFrom(stream);
		} catch (IOException e) {
			logger.error(" Protocol Buffer Reader  in Part ", e);
		}

		mCustomer.setCustkey(protocCustomer.getCustkey());
		mCustomer.setName(protocCustomer.getName());
		mCustomer.setAddress(protocCustomer.getAddress());
		mCustomer.setNationkey(protocCustomer.getNationkey());
		mCustomer.setPhone(protocCustomer.getPhone());
		mCustomer.setAccbal(protocCustomer.getAccbal());
		mCustomer.setMktsegment(protocCustomer.getMktsegment());
		mCustomer.setComment(protocCustomer.getComment());

		List<OrderP> p_orders = protocCustomer.getOrderList();
		ArrayList<Order> morders = new ArrayList<Order>();

		
		for (OrderP order : p_orders) {
			Order morder=new Order();
			
			morder.setOrderstatus(order.getOrderStatus());
			morder.setTotalprice(order.getTotalPrice());
			morder.setOrderdate(order.getOrderDate());
			morder.setOrderpriority(order.getOrderPriority());
			morder.setClerk(order.getClerk());
			morder.setShippriority(order.getShipPeriority());
			morder.setComment(order.getComment());
			
			List<LineItemP> listLineItemsP= order.getLineItemList();
			List<LineItem> mListOfLineItems= new ArrayList<LineItem>();
			
			
 			for (LineItemP protocLineItem : listLineItemsP) {
 				LineItem m_LineItem=new LineItem();
 				
 				m_LineItem.setLineNumber(protocLineItem.getLineNumber());
 				m_LineItem.setQueantity(protocLineItem.getQuantity());
 				m_LineItem.setExtendedPrice(protocLineItem.getExtendedPrice());
 				m_LineItem.setDiscount(protocLineItem.getDiscount());
 				m_LineItem.setTax(protocLineItem.getTax());
 				m_LineItem.setReturnFlag(protocLineItem.getReturnFlag());
 				m_LineItem.setLineStatus(protocLineItem.getLineStatus());
 				m_LineItem.setShipDate(protocLineItem.getShipDate());
 				m_LineItem.setCommitDate(protocLineItem.getCommitDate());
 				m_LineItem.setReceiptDate(protocLineItem.getReceiptDate());
 				m_LineItem.setShipinStruct(protocLineItem.getShipinStruct());
 				m_LineItem.setShipMode(protocLineItem.getShipMode());
 				m_LineItem.setComment(protocLineItem.getComment());
 				
 				// #####################################################
 				// #####################################################
 				// Reading Supplier
 				// #####################################################
 				// #####################################################

 				SupplierP p_suppliers = protocLineItem.getSupplierP();

 				Supplier sup = new Supplier();

 				sup.setName(p_suppliers.getName());
 				sup.setAddress(p_suppliers.getAddress());
 				sup.setName(p_suppliers.getName());
 				sup.setAccbal(p_suppliers.getAccbal());
 				sup.setComment(p_suppliers.getComment());
 				sup.setNationKey(p_suppliers.getNationKey());
 				sup.setPhone(p_suppliers.getPhone());
 				sup.setSupplierKey(p_suppliers.getSupplierKey());

 				// add the list of suppliers to LineItem
 				m_LineItem.setSupplier(sup);

 				// #####################################################
 				// #####################################################
 				// Reading Part
 				// #####################################################
 				// #####################################################

 				PartP my_partP = protocLineItem.getPartP();

 				Part prt = new Part();
 				prt.setPartID(my_partP.getPartID());
 				prt.setName(my_partP.getName());
 				prt.setMfgr(my_partP.getMfgr());
 				prt.setBrand(my_partP.getBrand());
 				prt.setType(my_partP.getType());
 				prt.setSize(my_partP.getSize());
 				prt.setContainer(my_partP.getContainer());
 				prt.setRetailPrice(my_partP.getRetailPrice());
 				prt.setComment(my_partP.getComment()); 
 				
 				
 				m_LineItem.setPart(prt);

 				
 				
 				
 				mListOfLineItems.add(m_LineItem);
 				
				
			}
			
		morder.setLineItems(mListOfLineItems);
			
			
			
			
			
			
			// add the single order to the list of orders
			morders.add(morder);
			
			
			
			
		}
		
		// set the list of Orders for each customer 
		mCustomer.setOrders(morders);
		
		
		
		return mCustomer;
	}

	
	
	
	@Override
	public RootData readByteBuffer(byte[] buf) {

		
//		private int custkey; // 4 bytes 
//		private int nationkey; // 4 bytes 
//		private double accbal;  // 8 bytes 
		 
//		private String name;
//		private String address;
//		private String phone;
//		private String mktsegment;
//		private String comment;

//		private List<Order> orders;

		// allocate a buffer for 12 bytes to write an integer and a double
		ByteBuffer byteBuffer = ByteBuffer.wrap(buf);

		int tmpCustkey = byteBuffer.getInt(); // 4 bytes
		int tmpNationkey = byteBuffer.getInt(); // 4 bytes
		Double tmpAccbal = byteBuffer.getDouble(); // 8 bytes
		
		int stringSize;

		// Get name
		stringSize = byteBuffer.getInt(); // 4 bytes = 20
		String tmpName = extractString(byteBuffer, stringSize);

		// Get address
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpAddress = extractString(byteBuffer, stringSize);

		// phone
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpPhone = extractString(byteBuffer, stringSize);

		// mktsegment
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpMktsegment = extractString(byteBuffer, stringSize);

		// comment
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpComment = extractString(byteBuffer, stringSize);

		int numberOfOrders = byteBuffer.getInt();
		ArrayList<Order> myListOfOrders = new ArrayList<Order>();
		
		// Iterate and read Orders one by one 
		for (int i = 0; i < numberOfOrders; i++) {

			// add LineItems one by one
			byte[] orderBytes = new byte[byteBuffer.getInt()];
			byteBuffer.get(orderBytes, 0, orderBytes.length);
			Order tmpOrder = (Order) new Order().readByteBuffer(orderBytes);
			myListOfOrders.add(tmpOrder);
		}

		return new Customer(myListOfOrders, tmpCustkey, tmpName, tmpAddress, tmpNationkey, tmpPhone, tmpAccbal, tmpMktsegment, tmpComment);

	}
	
	public String toString(){
		return this.getName();
	} 
	
	

}
