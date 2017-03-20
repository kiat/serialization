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
import java.util.Map;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

import javax.json.Json;
import javax.json.JsonObject;
import javax.json.JsonObjectBuilder;
import javax.json.JsonReader;
import javax.json.JsonWriter;
import javax.json.JsonWriterFactory;
import javax.json.stream.JsonGenerator;

import org.apache.log4j.Logger;

import com.fasterxml.jackson.core.JsonToken;
import com.google.protobuf.CodedInputStream;

import edu.rice.pdb.serialization.BsonSinglton;
//import edu.rice.pdb.serialization.ProtocolBufferSinglton;
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP;
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP.PartP;
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP.SupplierP;

public class LineItem extends Base implements RootData {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4559123225256716388L;

	static Logger logger = Logger.getLogger(LineItem.class);

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

	// private String name;
	private int orderKey;

	private Supplier supplier;
	private Part part;

	private int lineNumber;
	private double quantity;
	private double extendedPrice;
	private double discount;
	private double tax;
	private String returnFlag;
	private String lineStatus;

	private String shipDate;
	private String commitDate;
	private String receiptDate;

	private String shipinStruct;
	private String shipMode;
	private String comment;

	public LineItem() {
	}

	public LineItem(int orderKey, Supplier suppliers, Part parts, int lineNumber, double quantity, double extendedPrice, double discount, double tax, String returnFlag, String lineStatus, String shipDate,
			String commitDate, String receiptDate, String shipinStruct, String shipMode, String comment) {
		super();
		this.orderKey = orderKey;
		this.supplier = suppliers;
		this.part = parts;
		this.lineNumber = lineNumber;
		this.quantity = quantity;
		this.extendedPrice = extendedPrice;
		this.discount = discount;
		this.tax = tax;
		this.returnFlag = returnFlag;
		this.lineStatus = lineStatus;
		this.shipDate = shipDate;
		this.commitDate = commitDate;
		this.receiptDate = receiptDate;
		this.shipinStruct = shipinStruct;
		this.shipMode = shipMode;
		this.comment = comment;
	}

	@Override
	public ArrayList<RootData> generateObjects(int number) {
		ArrayList<RootData> objectList = new ArrayList<RootData>();

		for (int i = 0; i < number; i++) {

			RootData tmp = new LineItem(i, new Supplier().generateSupplier(), new Part().generatePart(), i, i, i, i, i, "a" + i, "a" + i, "Data-" + i, "Data-" + i, "Data-" + i, "a" + i, "a" + i, "comment" + i);
			objectList.add(tmp);
		}

		return objectList;
	}

	/**
	 * This is exactly the same implementation as in Class Part
	 */
	@Override
	public LineItem javaDefaultDeserialization(byte[] buf) {

		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		ObjectInputStream objectInputStream;

		LineItem p = null;

		try {
			objectInputStream = new ObjectInputStream(b);
			p = (LineItem) objectInputStream.readObject();

		} catch (IOException e) {
			logger.error("LineItem javaDefaultDeserialization IOException: Can NOT read object LineItem from byteArray", e);
		} catch (ClassNotFoundException e) {
			logger.error("LineItem javaDefaultDeserialization ClassNotFoundException:  Can NOT read object LineItem from byteArray", e);
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
		JsonObjectBuilder lineItemBuilder = Json.createObjectBuilder();

		// add all of the attributes of LineItem
		lineItemBuilder.add("orderkey", this.getOrderKey());
		lineItemBuilder.add("linenumber", this.getLineNumber());
		lineItemBuilder.add("quantity", this.getQuantity());
		lineItemBuilder.add("extendedprice", this.getExtendedPrice());
		lineItemBuilder.add("discount", this.getDiscount());
		lineItemBuilder.add("tax", this.getTax());
		lineItemBuilder.add("returngflag", this.getReturnFlag());
		lineItemBuilder.add("shipdate", this.getShipDate());
		lineItemBuilder.add("commitdate", this.getCommitDate());
		lineItemBuilder.add("shipinstruct", this.getShipinStruct());
		lineItemBuilder.add("shipmode", this.getShipMode());
		lineItemBuilder.add("comment", this.getComment());

		// Supplier and Part are two objects
		lineItemBuilder.add("supplier", this.getSupplier().jsonJsonObjectBuilder());
		lineItemBuilder.add("part", this.part.jsonJsonObjectBuilder());

		JsonObject lineitemJsonObject = lineItemBuilder.build();
		return lineitemJsonObject;
	}

	@Override
	public LineItem jsonDeserialization(byte[] buf) {
		LineItem myLineItem = new LineItem();

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

			myLineItem = readJSONLineItem(jsonObject);

			// we can close IO resource and JsonReader now
			jsonReader.close();

		} catch (Exception e) {
			logger.error("Can not read JSON object Part from byteArray", e);
		}

		return myLineItem;
	}

	public LineItem readJSONLineItem(JsonObject jsonObject) {
		LineItem myLineItem = new LineItem();

		myLineItem.setOrderKey(jsonObject.getInt("orderkey"));
		myLineItem.setLineNumber(jsonObject.getInt("linenumber"));
		myLineItem.setQueantity(jsonObject.getInt("quantity"));
		myLineItem.setExtendedPrice(jsonObject.getInt("extendedprice"));
		myLineItem.setDiscount(jsonObject.getInt("discount"));
		myLineItem.setTax(jsonObject.getInt("tax"));
		myLineItem.setReturnFlag(jsonObject.getString("returngflag"));
		myLineItem.setShipDate(jsonObject.getString("shipdate"));
		myLineItem.setCommitDate(jsonObject.getString("commitdate"));
		myLineItem.setShipDate(jsonObject.getString("shipdate"));
		myLineItem.setShipinStruct(jsonObject.getString("shipinstruct"));
		myLineItem.setShipMode(jsonObject.getString("shipmode"));
		myLineItem.setComment(jsonObject.getString("comment"));

		// reading Supplier from JSON
		JsonObject m_supplierJsonObject = jsonObject.getJsonObject("supplier");
		myLineItem.setSupplier(new Supplier().readJSONSupplier(m_supplierJsonObject));

		// reading Part from JSON
		JsonObject m_partJsonObject = jsonObject.getJsonObject("part");
		myLineItem.setPart(new Part().readJSONSupplier(m_partJsonObject));

		return myLineItem;
	}

	@Override
	public byte[] protocolBufferWrite() {
		 LineItemP.Builder m_lineItem = LineItemP.newBuilder();
//		LineItemP.Builder m_lineItem = ProtocolBufferSinglton.getInstance().getLineItem();

		m_lineItem.setLineNumber(this.orderKey);
		m_lineItem.setLineNumber(this.lineNumber);
		m_lineItem.setQuantity(this.quantity);
		m_lineItem.setExtendedPrice(this.extendedPrice);
		m_lineItem.setDiscount(this.discount);
		m_lineItem.setTax(this.tax);
		m_lineItem.setReturnFlag(this.returnFlag);
		m_lineItem.setLineStatus(this.lineStatus);
		m_lineItem.setShipDate(this.shipDate);
		m_lineItem.setCommitDate(this.commitDate);
		m_lineItem.setReceiptDate(this.receiptDate);
		m_lineItem.setShipinStruct(this.shipinStruct);
		m_lineItem.setShipMode(this.shipMode);
		m_lineItem.setShipMode(this.shipMode);
		m_lineItem.setComment(this.comment);

		SupplierP.Builder m_supplier = SupplierP.newBuilder();
//		SupplierP.Builder m_supplier = ProtocolBufferSinglton.getInstance().getSupplier();

		m_supplier.setName(this.supplier.getName());
		m_supplier.setAddress(this.supplier.getAddress());
		m_supplier.setSupplierKey(this.supplier.getSupplierKey());
		m_supplier.setNationKey(this.supplier.getNationKey());
		m_supplier.setPhone(this.supplier.getPhone());
		m_supplier.setAccbal(this.supplier.getAccbal());
		m_supplier.setComment(this.supplier.getComment());

		m_lineItem.setSupplierP(m_supplier);

		PartP.Builder m_part = PartP.newBuilder();
//		PartP.Builder m_part = ProtocolBufferSinglton.getInstance().getPart();

		m_part.setName(this.part.getName());
		m_part.setType(this.part.getType());
		m_part.setSize(this.part.getSize());
		m_part.setBrand(this.part.getBrand());
		m_part.setMfgr(this.part.getMfgr());
		m_part.setPartID(this.part.getPartID());
		m_part.setRetailPrice(this.part.getRetailPrice());
		m_part.setContainer(this.part.getContainer());
		m_part.setComment(this.part.getComment());

		m_lineItem.setPartP(m_part);

		byte[] buf = null;

		ByteArrayOutputStream bos = new ByteArrayOutputStream();

		// Write the class room to disk.
		try {
			m_lineItem.build().writeTo(bos);
			// Get the bytes of the serialized JSON object
			buf = bos.toByteArray();
			bos.close();

		} catch (IOException e) {
			logger.error("Protocol Buffer Writer in Part ", e);
		}

		return buf;
	}

	@Override
	public RootData protocolBufferRead(byte[] buf) {
		LineItem m_LineItem = new LineItem();
		ByteArrayInputStream b = new ByteArrayInputStream(buf);

		LineItemP.Builder protocLineItem = LineItemP.newBuilder();
//		LineItemP.Builder protocLineItem = ProtocolBufferSinglton.getInstance().getLineItem();

		CodedInputStream stream = CodedInputStream.newInstance(b);

		// mergeFrom
		try {
			protocLineItem.mergeFrom(stream);
		} catch (IOException e) {
			logger.error(" Protocol Buffer Reader  in Part ", e);
		}

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

		return m_LineItem;
	}

	public int getOrderKey() {
		return orderKey;
	}

	public void setOrderKey(int orderKey) {
		this.orderKey = orderKey;
	}

	public Supplier getSupplier() {
		return supplier;
	}

	public void setSupplier(Supplier supplier) {
		this.supplier = supplier;
	}

	public int getLineNumber() {
		return lineNumber;
	}

	public void setLineNumber(int lineNumber) {
		this.lineNumber = lineNumber;
	}

	public double getQuantity() {
		return quantity;
	}

	public void setQueantity(double queantity) {
		this.quantity = queantity;
	}

	public double getExtendedPrice() {
		return extendedPrice;
	}

	public void setExtendedPrice(double extendedPrice) {
		this.extendedPrice = extendedPrice;
	}

	public double getDiscount() {
		return discount;
	}

	public void setDiscount(double discount) {
		this.discount = discount;
	}

	public double getTax() {
		return tax;
	}

	public void setTax(double tax) {
		this.tax = tax;
	}

	public String getReturnFlag() {
		return returnFlag;
	}

	public void setReturnFlag(String returnFlag) {
		this.returnFlag = returnFlag;
	}

	public String getLineStatus() {
		return lineStatus;
	}

	public void setLineStatus(String lineStatus) {
		this.lineStatus = lineStatus;
	}

	public String getShipDate() {
		return shipDate;
	}

	public void setShipDate(String shipDate) {
		this.shipDate = shipDate;
	}

	public String getCommitDate() {
		return commitDate;
	}

	public void setCommitDate(String commitDate) {
		this.commitDate = commitDate;
	}

	public String getReceiptDate() {
		return receiptDate;
	}

	public void setReceiptDate(String receiptDate) {
		this.receiptDate = receiptDate;
	}

	public String getShipinStruct() {
		return shipinStruct;
	}

	public void setShipinStruct(String shipinStruct) {
		this.shipinStruct = shipinStruct;
	}

	public String getShipMode() {
		return shipMode;
	}

	public void setShipMode(String shipMode) {
		this.shipMode = shipMode;
	}

	public String getComment() {
		return comment;
	}

	public void setComment(String comment) {
		this.comment = comment;
	}

	public String toString() {
		String myString = this.getOrderKey() + "  - " + this.getLineNumber() + "  - " + this.getQuantity() + "  - " + this.getExtendedPrice() + "  - " + this.getDiscount() + "  - " + this.getTax() + "  - "
				+ this.getReturnFlag() + "  - " + this.getShipDate() + "  - " + this.getCommitDate() + "  - " + this.getShipinStruct() + "  - " + this.getShipMode() + "  - " + this.getComment();
		myString = myString + "-supplier-" + this.getSupplier().toString();
		myString = myString + "-part-" + this.getPart().toString();

		return myString;

	}

	public Part getPart() {
		return part;
	}

	public void setPart(Part part) {
		this.part = part;
	}

	@Override
	public int compareTo(RootData o) {

		LineItem myLineItem = (LineItem) o;

		if (myLineItem.getLineNumber() < this.getLineNumber()) {
			return -1;
		} else if (myLineItem.getLineNumber() > this.getLineNumber()) {
			return 1;
		} else {
			// They have the same LineNumber
			if (myLineItem.getPart().getPartID() < this.getPart().getPartID()) {
				return -1;
			} else if (myLineItem.getPart().getPartID() > this.getPart().getPartID()) {
				return 1;
			} else {
				// they have the same partID
				if (myLineItem.getSupplier().getSupplierKey() < this.getSupplier().getSupplierKey()) {
					return -1;
				} else if (myLineItem.getSupplier().getSupplierKey() > this.getSupplier().getSupplierKey()) {
					return 1;
				} else {
					// and they have the same getSupplierKey
					return 0;
				}
			}
		}
	}

	@Override
	public byte[] writeByteBuffer() {

		//
		// private int orderKey; // 4 bytes
		// private int lineNumber; // 4 bytes
		// private int quantity; // 4 bytes
		// private int extendedPrice; //4 bytes
		// private int discount; // 4 bytes
		// private int tax; // 4 bytes

		// private String name;
		// private String returnFlag;
		// private String lineStatus;
		// private String shipDate;
		// private String commitDate;
		// private String receiptDate;
		// private String shipinStruct;
		// private String shipMode;
		// private String comment;

		// private Supplier supplier;
		// private Part part;
		//

		// Container
		byte[] returnFlagBytes = returnFlag.getBytes(Charset.forName("UTF-8"));
		byte[] lineStatusBytes = lineStatus.getBytes(Charset.forName("UTF-8"));
		byte[] shipDateBytes = shipDate.getBytes(Charset.forName("UTF-8"));
		byte[] commitDateBytes = commitDate.getBytes(Charset.forName("UTF-8"));
		byte[] receiptDateBytes = receiptDate.getBytes(Charset.forName("UTF-8"));
		byte[] shipinStructBytes = shipinStruct.getBytes(Charset.forName("UTF-8"));
		byte[] shipModeBytes = shipMode.getBytes(Charset.forName("UTF-8"));
		byte[] commentBytes = comment.getBytes(Charset.forName("UTF-8"));

		// Objects of supplier and part
		byte[] supplierBytes = supplier.writeByteBuffer();
		byte[] partBytes = part.writeByteBuffer();

		// allocate a buffer to write this object

		// 6 + 11 int x 4 =
		ByteBuffer byteBuffer = ByteBuffer.allocate(80 + returnFlagBytes.length + lineStatusBytes.length + shipDateBytes.length + commitDateBytes.length + receiptDateBytes.length + shipinStructBytes.length
				+ shipModeBytes.length + commentBytes.length + supplierBytes.length + partBytes.length);

		byteBuffer.putInt(orderKey); // 4 bytes
		byteBuffer.putInt(lineNumber); // 4 bytes
		byteBuffer.putDouble(quantity); // 8 bytes
		byteBuffer.putDouble(extendedPrice); // 8 bytes
		byteBuffer.putDouble(discount); // 8 bytes
		byteBuffer.putDouble(tax); // 8 bytes

		// returnFlagBytes
		byteBuffer.putInt(returnFlagBytes.length); // 4 bytes size of
													// "container string"
		byteBuffer.put(returnFlagBytes);

		// lineStatusBytes
		byteBuffer.putInt(lineStatusBytes.length); // 4 bytes size of
													// "container string"
		byteBuffer.put(lineStatusBytes);

		// shipDateBytes
		byteBuffer.putInt(shipDateBytes.length); // 4 bytes size of
													// "container string"
		byteBuffer.put(shipDateBytes);

		// commitDateBytes
		byteBuffer.putInt(commitDateBytes.length); // 4 bytes size of
													// "container string"
		byteBuffer.put(commitDateBytes);

		// receiptDateBytes
		byteBuffer.putInt(receiptDateBytes.length); // 4 bytes size of
													// "container string"
		byteBuffer.put(receiptDateBytes);

		// shipinStructBytes
		byteBuffer.putInt(shipinStructBytes.length); // 4 bytes size of
														// "container string"
		byteBuffer.put(shipinStructBytes);

		// commitDateBytes
		byteBuffer.putInt(shipModeBytes.length); // 4 bytes size of
													// "container string"
		byteBuffer.put(shipModeBytes);

		// commitDateBytes
		byteBuffer.putInt(commentBytes.length); // 4 bytes size of
												// "container string"
		byteBuffer.put(commentBytes);

		// supplier
		byteBuffer.putInt(supplierBytes.length); // 4 bytes size of
													// "container string"
		byteBuffer.put(supplierBytes);

		// part
		byteBuffer.putInt(partBytes.length); // 4 bytes size of
												// "container string"
		byteBuffer.put(partBytes);

		return byteBuffer.array();
	}

	@Override
	public RootData readByteBuffer(byte[] buf) {

		// allocate a buffer for 12 bytes to write an integer and a double
		ByteBuffer byteBuffer = ByteBuffer.wrap(buf);

		//
		// private int orderKey; // 4 bytes
		// private int lineNumber; // 4 bytes
		// private int quantity; // 4 bytes
		// private int extendedPrice; //4 bytes
		// private int discount; // 4 bytes
		// private int tax; // 4 bytes

		// private String name;
		// private String returnFlag;
		// private String lineStatus;
		// private String shipDate;
		// private String commitDate;
		// private String receiptDate;
		// private String shipinStruct;
		// private String shipMode;
		// private String comment;

		// private Supplier supplier;
		// private Part part;
		//

		int tmpOrderKey = byteBuffer.getInt(); // 4 bytes
		int tmpLineNumber = byteBuffer.getInt(); // 4 bytes = 8
		double tmpQuantity = byteBuffer.getDouble(); // 8 bytes
		double tmpExtendedPrice = byteBuffer.getDouble(); // 8
		double tmpDiscount = byteBuffer.getDouble(); // 8 bytes
		double tmpTax = byteBuffer.getDouble(); // 8

		// reading the strings

		int stringSize;

		// mfgr
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpReturnFlag = extractString(byteBuffer, stringSize);

		// brand
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpLineStatus = extractString(byteBuffer, stringSize);

		// ShipDat
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpShipDate = extractString(byteBuffer, stringSize);

		// commitDate
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpCommitDate = extractString(byteBuffer, stringSize);

		// ReceiptDate
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpReceiptDate = extractString(byteBuffer, stringSize);

		// shipinStruct
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpShipinStruct = extractString(byteBuffer, stringSize);

		// shipMode
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpShipMode = extractString(byteBuffer, stringSize);

		// comment
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpComment = extractString(byteBuffer, stringSize);

		// now reading Supplier and Part

		// Supplier
		byte[] supplierBytes = new byte[byteBuffer.getInt()];
		byteBuffer.get(supplierBytes, 0, supplierBytes.length);
		Supplier tmpSupplier = new Supplier().readByteBuffer(supplierBytes);

		// Part
		byte[] partBytes = new byte[byteBuffer.getInt()];
		byteBuffer.get(partBytes, 0, partBytes.length);
		Part tmpPart = (Part) new Part().readByteBuffer(partBytes);

		return new LineItem(tmpOrderKey, tmpSupplier, tmpPart, tmpLineNumber, tmpQuantity, tmpExtendedPrice, tmpDiscount, tmpTax, tmpReturnFlag, tmpLineStatus, tmpShipDate, tmpCommitDate, tmpReceiptDate,
				tmpShipinStruct, tmpShipMode, tmpComment);

	}

	/**
	 * 
	 * @return
	 */
	@Override
	public byte[] bsonSerialization() {

		// serialize data
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		try {
			com.fasterxml.jackson.core.JsonGenerator gen = BsonSinglton.getInstance().getFactory().createJsonGenerator(baos);

			gen.writeStartObject();

			gen.writeFieldName("orderkey");
			gen.writeNumber(this.getOrderKey());

			gen.writeFieldName("linenumber");
			gen.writeNumber(this.getLineNumber());

			gen.writeFieldName("quantity");
			gen.writeNumber(this.getQuantity());

			gen.writeFieldName("extendedprice");
			gen.writeNumber(this.getExtendedPrice());

			gen.writeFieldName("discount");
			gen.writeNumber(this.getDiscount());

			gen.writeFieldName("tax");
			gen.writeNumber(this.getTax());

			gen.writeFieldName("returngflag");
			gen.writeNumber(this.getReturnFlag());

			gen.writeFieldName("shipdate");
			gen.writeString(this.getShipDate());

			gen.writeFieldName("shipinstruct");
			gen.writeString(this.getShipinStruct());

			gen.writeFieldName("shipmode");
			gen.writeString(this.getShipMode());

			gen.writeFieldName("comment");
			gen.writeString(this.getComment());

			gen.writeFieldName("part");
			gen.writeBinary(this.getPart().bsonSerialization());

			gen.writeFieldName("supplier");
			gen.writeBinary(this.getSupplier().bsonSerialization());

			gen.close();

		} catch (IOException e) {
			logger.error("Part bsonSerialization ", e);
		}
		return baos.toByteArray();
	}

	/**
	 * 
	 * @param buf
	 * @param myclass
	 * @return
	 */
	@Override
	public RootData bsonDeSerialization(byte[] buf, Class<?> myclass) {

		ByteArrayInputStream bais = new ByteArrayInputStream(buf);

		LineItem tmp = new LineItem();

		try {
			com.fasterxml.jackson.core.JsonParser parser = BsonSinglton.getInstance().getFactory().createJsonParser(bais);
			parser.nextToken();

			while (parser.nextToken() != JsonToken.END_OBJECT) {
				String fieldname = parser.getCurrentName();
				parser.nextToken();

				switch (fieldname) {
				case "orderkey":
					tmp.setOrderKey(parser.getIntValue());
					break;
				case "linenumber":
					tmp.setLineNumber(parser.getIntValue());
					break;
				case "quantity":
					tmp.setQueantity(parser.getDoubleValue());
					break;
				case "extendedprice":
					tmp.setExtendedPrice(parser.getDoubleValue());
					break;
				case "discount":
					tmp.setDiscount(parser.getDoubleValue());
					break;
				case "tax":
					tmp.setTax(parser.getDoubleValue());
					break;
				case "returngflag":
					tmp.setReturnFlag(parser.getText());
					break;
				case "shipdate":
					tmp.setShipDate(parser.getText());
					break;
				case "shipinstruct":
					tmp.setShipinStruct(parser.getText());
					break;
				case "shipmode":
					tmp.setShipMode(parser.getText());
					break;
				case "comment":
					tmp.setComment(parser.getText());
					break;
				case "part":
					tmp.setPart((Part) new Part().bsonDeSerialization(parser.getBinaryValue(), this.getClass()));
					break;
				case "supplier":
					tmp.setSupplier((Supplier) new Supplier().bsonDeSerialization(parser.getBinaryValue(), this.getClass()));
					break;

				}

			}

		} catch (IOException e) {
			logger.error("Element bsonSerialization ", e);

		}

		return tmp;
	}

}
