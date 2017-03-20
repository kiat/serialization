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

public class Order extends Base implements RootData {

	// CREATE TABLE ORDERS ( O_ORDERKEY INTEGER NOT NULL,
	// O_CUSTKEY INTEGER NOT NULL,
	// O_ORDERSTATUS CHAR(1) NOT NULL,
	// O_TOTALPRICE DECIMAL(15,2) NOT NULL,
	// O_ORDERDATE DATE NOT NULL,
	// O_ORDERPRIORITY CHAR(15) NOT NULL,
	// O_CLERK CHAR(15) NOT NULL,
	// O_SHIPPRIORITY INTEGER NOT NULL,
	// O_COMMENT VARCHAR(79) NOT NULL);

	// private int orderkey;
	// private int custkey;

	private static final long serialVersionUID = 6828883074098155463L;

	private List<LineItem> lineItems;

	private int orderkey;
	private int custkey;
	private String orderstatus;
	private double totalprice;
	private String orderdate;
	private String orderpriority;
	private String clerk;
	private int shippriority;
	private String comment;

	public Order() {

	}

	public Order(List<LineItem> lineItems, int orderkey, int custkey, String orderstatus, double totalprice, String orderdate, String orderpriority, String clerk, int shippriority, String comment) {
		super();

		this.lineItems = lineItems;
		this.orderkey = orderkey;
		this.custkey = custkey;
		this.orderstatus = orderstatus;
		this.totalprice = totalprice;
		this.orderdate = orderdate;
		this.orderpriority = orderpriority;
		this.clerk = clerk;
		this.shippriority = shippriority;
		this.comment = comment;
	}

	public int getOrderkey() {
		return orderkey;
	}

	public void setOrderkey(int orderkey) {
		this.orderkey = orderkey;
	}

	public int getCustkey() {
		return custkey;
	}

	public void setCustkey(int custkey) {
		this.custkey = custkey;
	}

	public double getTotalprice() {
		return totalprice;
	}

	public void setTotalprice(double totalprice) {
		this.totalprice = totalprice;
	}

	public String getOrderstatus() {
		return orderstatus;
	}

	public void setOrderstatus(String orderstatus) {
		this.orderstatus = orderstatus;
	}

	public String getOrderdate() {
		return orderdate;
	}

	public void setOrderdate(String orderdate) {
		this.orderdate = orderdate;
	}

	public String getOrderpriority() {
		return orderpriority;
	}

	public void setOrderpriority(String orderpriority) {
		this.orderpriority = orderpriority;
	}

	public String getClerk() {
		return clerk;
	}

	public void setClerk(String clerk) {
		this.clerk = clerk;
	}

	public int getShippriority() {
		return shippriority;
	}

	public void setShippriority(int shippriority) {
		this.shippriority = shippriority;
	}

	public String getComment() {
		return comment;
	}

	public void setComment(String comment) {
		this.comment = comment;
	}

	@Override
	public ArrayList<RootData> generateObjects(int number) {
		ArrayList<RootData> tmpList = new ArrayList<RootData>();

		List<RootData> rootDataLineItems = new ArrayList<RootData>();
		List<LineItem> lineItems = new ArrayList<LineItem>();

		rootDataLineItems = new LineItem().generateObjects(10);
		for (RootData tmp : rootDataLineItems) {
			lineItems.add((LineItem) tmp);
		}

		for (int i = 0; i < number; i++) {
			// Order(int , int , char , int , String , char , char , int ,
			// String )
			String ichar = i + "";
			double totalprice = (double) (i);
			Order myOrder = new Order(lineItems, i, i, ichar, totalprice, "" + i, ichar, ichar, i, "" + i);

			tmpList.add(myOrder);
		}

		return tmpList;
	}

	public Order readJSONSupplier(JsonObject supplierJsonObject) {

		Order m_order = new Order();
		m_order.setOrderkey(supplierJsonObject.getInt("orderkey"));
		m_order.setCustkey(supplierJsonObject.getInt("custkey"));

		m_order.setOrderstatus(supplierJsonObject.getString("orderstatus"));

		m_order.setTotalprice(supplierJsonObject.getInt("totalprice"));
		m_order.setOrderdate(supplierJsonObject.getString("orderdate"));
		m_order.setOrderpriority(supplierJsonObject.getString("orderpriority"));
		m_order.setClerk(supplierJsonObject.getString("clerk"));
		m_order.setShippriority(supplierJsonObject.getInt("shippriority"));
		m_order.setComment(supplierJsonObject.getString("comment"));

		return m_order;
	}

	public List<LineItem> getLineItems() {
		return lineItems;
	}

	public void setLineItems(List<LineItem> lineItems) {
		this.lineItems = lineItems;
	}

	@Override
	public int compareTo(RootData data) {
		Order m_Order = (Order) data;
		int result=1;
		
		if (m_Order.getOrderkey() == this.getOrderkey()) {
			result=0;
			for (int i = 0; i < m_Order.lineItems.size(); i++) {
					result=m_Order.lineItems.get(i).compareTo(this.lineItems.get(i));
			}
		}
		return result;
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
		orderJsonObjectBuilder.add("orderkey", this.getOrderkey());
		orderJsonObjectBuilder.add("custkey", this.getCustkey());
		orderJsonObjectBuilder.add("orderstatus", this.getOrderstatus());
		orderJsonObjectBuilder.add("totalprice", this.getTotalprice());
		orderJsonObjectBuilder.add("orderdate", this.getOrderdate());
		orderJsonObjectBuilder.add("orderpriority", this.getOrderpriority());
		orderJsonObjectBuilder.add("clerk", this.getClerk());
		orderJsonObjectBuilder.add("shippriority", this.getShippriority());
		orderJsonObjectBuilder.add("comment", this.getComment());

		// Order are a list
		JsonArrayBuilder jsonOrdersArray = Json.createArrayBuilder();

		for (LineItem myLineItem : this.lineItems) {

			jsonOrdersArray.add(myLineItem.jsonObjectBuilder());
		}
		// add the Parts as JSON array to the lineitemBuilder
		orderJsonObjectBuilder.add("lineItems", jsonOrdersArray);

		JsonObject lineitemJsonObject = orderJsonObjectBuilder.build();
		return lineitemJsonObject;
	}

	@Override
	public RootData jsonDeserialization(byte[] buf) {

		Order myOrder = new Order();

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

			myOrder = readJSONLineItem(jsonObject);

			// we can close IO resource and JsonReader now
			jsonReader.close();

		} catch (Exception e) {
			logger.error("Can not read JSON object Part from byteArray", e);
		}

		return myOrder;

	}

	public Order readJSONLineItem(JsonObject jsonObject) {
		Order myOrder = new Order();

		myOrder.setOrderkey(jsonObject.getInt("orderkey"));
		myOrder.setCustkey(jsonObject.getInt("custkey"));
		myOrder.setOrderstatus(jsonObject.getString("orderstatus"));
		myOrder.setTotalprice(jsonObject.getInt("totalprice"));
		myOrder.setOrderdate(jsonObject.getString("orderdate"));
		myOrder.setOrderpriority(jsonObject.getString("orderpriority"));
		myOrder.setClerk(jsonObject.getString("clerk"));
		myOrder.setShippriority(jsonObject.getInt("shippriority"));
		myOrder.setComment(jsonObject.getString("comment"));

		// reading LineItem arrays from json
		JsonArray jsonSupplier = jsonObject.getJsonArray("lineItems");
		ArrayList<LineItem> lineItems = new ArrayList<LineItem>();
		LineItem myLineItem = new LineItem();

		for (int i = 0; i < jsonSupplier.size(); i++) {
			JsonObject m_LineItemJsonObject = jsonSupplier.getJsonObject(i);
			lineItems.add(myLineItem.readJSONLineItem(m_LineItemJsonObject));
		}
		myOrder.setLineItems(lineItems);

		return myOrder;
	}

	// TODO - it is not need to implement protocol buffer for this class
	@Override
	public byte[] protocolBufferWrite() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public RootData protocolBufferRead(byte[] buf) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Order javaDefaultDeserialization(byte[] buf) {
		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		ObjectInputStream objectInputStream;

		Order p = null;

		try {
			objectInputStream = new ObjectInputStream(b);
			p = (Order) objectInputStream.readObject();

		} catch (IOException | ClassNotFoundException e) {
			logger.error("Can read object Part from byteArray", e);
		}
		return p;
	}

	@Override
	public byte[] writeByteBuffer() {

		// private List<LineItem> lineItems;
		//
		// private int orderkey; // 4 bytes
		// private int custkey; // 4 bytes
		// private int totalprice; // 4 bytes
		// private int shippriority; //4 bytes
		//
		// private String orderstatus;
		// private String orderdate;
		// private String orderpriority;
		// private String clerk;
		// private String comment;

		// Container
		byte[] containerBytes = orderstatus.getBytes(Charset.forName("UTF-8"));
		byte[] nameBytes = orderdate.getBytes(Charset.forName("UTF-8"));
		byte[] mfgrBytes = orderpriority.getBytes(Charset.forName("UTF-8"));
		byte[] brandBytes = clerk.getBytes(Charset.forName("UTF-8"));
		byte[] commentBytes = comment.getBytes(Charset.forName("UTF-8"));

		ArrayList<byte[]> lineItemBytes = new ArrayList<byte[]>();

		int lengthOfLineItems = 0;

		for (LineItem tmpLineItem : lineItems) {
			byte[] tmpLineItemBytes = tmpLineItem.writeByteBuffer();
			lineItemBytes.add(tmpLineItemBytes);

			// 4 bytes is for an additional integer for write the size byteArray
			lengthOfLineItems = lengthOfLineItems + tmpLineItemBytes.length + 4;
		}

		// allocate a buffer to write this object
		// an additional 4 bytes for the number of LineItem in the list
		ByteBuffer byteBuffer = ByteBuffer.allocate(44 + containerBytes.length + nameBytes.length + mfgrBytes.length + brandBytes.length + commentBytes.length + lengthOfLineItems);

		byteBuffer.putInt(orderkey); // 4 bytes
		byteBuffer.putInt(custkey); // 4 bytes
		byteBuffer.putDouble(totalprice); // 8 bytes
		byteBuffer.putInt(shippriority); // 4 bytes

		byteBuffer.putInt(containerBytes.length); // 4 bytes size of
		byteBuffer.put(containerBytes);

		// name
		byteBuffer.putInt(nameBytes.length); // 4 bytes size of
		byteBuffer.put(nameBytes);

		// mfgr
		byteBuffer.putInt(mfgrBytes.length); // 4 bytes size of
		byteBuffer.put(mfgrBytes);

		// brand
		byteBuffer.putInt(brandBytes.length); // 4 bytes size of
		byteBuffer.put(brandBytes);

		// comment
		byteBuffer.putInt(commentBytes.length); // 4 bytes size of
		byteBuffer.put(commentBytes);

		// first write how many items have to be read from the buffer
		// number of LineItems in the list
		byteBuffer.putInt(lineItemBytes.size());

		for (byte[] mylineItemBytes : lineItemBytes) {
			// Add each lineItem byte array one bye one
			byteBuffer.putInt(mylineItemBytes.length);
			byteBuffer.put(mylineItemBytes);
		}

		return byteBuffer.array();
	}

	@Override
	public RootData readByteBuffer(byte[] buf) {

		// private List<LineItem> lineItems;
		//
		// private int orderkey; // 4 bytes
		// private int custkey; // 4 bytes
		// private int totalprice; // 4 bytes
		// private int shippriority; //4 bytes
		//
		// private String orderstatus;
		// private String orderdate;
		// private String orderpriority;
		// private String clerk;
		// private String comment;

		// allocate a buffer for 12 bytes to write an integer and a double
		ByteBuffer byteBuffer = ByteBuffer.wrap(buf);

		int tmpOrderkey = byteBuffer.getInt(); // 4 bytes
		int tmpCustkey = byteBuffer.getInt(); // 4 bytes
		double tmpTotalprice = byteBuffer.getDouble(); // 8 bytes
		int tmpShippriority = byteBuffer.getInt(); // 4 bytes
		int stringSize;

		// Get orderstatus
		stringSize = byteBuffer.getInt(); // 4 bytes = 20
		String tmpOrderstatus = extractString(byteBuffer, stringSize);

		// Get orderdate
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpOrderdate = extractString(byteBuffer, stringSize);

		// orderpriority
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpOrderpriority = extractString(byteBuffer, stringSize);

		// clerk
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpClerk = extractString(byteBuffer, stringSize);

		// comment
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpComment = extractString(byteBuffer, stringSize);

		int numberOfLineItems = byteBuffer.getInt();
		ArrayList<LineItem> myListOfLineItems = new ArrayList<LineItem>();

		for (int i = 0; i < numberOfLineItems; i++) {

			// add LineItems one by one
			byte[] LineItemBytes = new byte[byteBuffer.getInt()];
			byteBuffer.get(LineItemBytes, 0, LineItemBytes.length);
			LineItem tmpLineItem = (LineItem) new LineItem().readByteBuffer(LineItemBytes);
			myListOfLineItems.add(tmpLineItem);
		}

		return new Order(myListOfLineItems, tmpOrderkey, tmpCustkey, tmpOrderstatus, tmpTotalprice, tmpOrderdate, tmpOrderpriority, tmpClerk, tmpShippriority, tmpComment);

	}

}
