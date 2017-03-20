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
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP.PartP;

//CREATE TABLE PART  ( P_PARTKEY     INTEGER NOT NULL,
//        P_NAME        VARCHAR(55) NOT NULL,
//        P_MFGR        CHAR(25) NOT NULL,
//        P_BRAND       CHAR(10) NOT NULL,
//        P_TYPE        VARCHAR(25) NOT NULL,
//        P_SIZE        INTEGER NOT NULL,
//        P_CONTAINER   CHAR(10) NOT NULL,
//        P_RETAILPRICE DECIMAL(15,2) NOT NULL,
//        P_COMMENT     VARCHAR(23) NOT NULL );

public class Part extends Base implements RootData {
	static Logger logger = Logger.getLogger(Part.class);

	/**
	 * 
	 */
	private static final long serialVersionUID = -7063315724751188574L;

	private int partID;
	private String name;
	private String mfgr;
	private String brand;
	private String type;
	private int size;
	private String container;
	private double retailPrice;
	private String comment;

	public Part() {
	}

	public Part(int partID, String name, String mfgr, String brand, String type, int size, String container, double retailPrice, String comment) {
		this.partID = partID;
		this.name = name;
		this.mfgr = mfgr;
		this.brand = brand;
		this.type = type;
		this.size = size;
		this.container = container;
		this.retailPrice = retailPrice;
		this.comment = comment;

	}

	@Override
	public ArrayList<RootData> generateObjects(int number) {

		ArrayList<RootData> objectList = new ArrayList<RootData>();

		for (int i = 0; i < number; i++) {
			RootData tmp = new Part(i, "Name-" + i, "MFGR-" + i, "Brand-" + i, "Type-" + i, i, "Container-" + i, i, "Comment-" + i);
			objectList.add(tmp);
		}

		return objectList;
	}

	public Part generatePart() {
		int i = 0;
		return new Part(i, "Name-" + i, "MFGR-" + i, "Brand-" + i, "Type-" + i, i, "Container-" + i, i, "Comment-" + i);
	}

	public ArrayList<Part> generateParts(int number) {

		ArrayList<Part> objectList = new ArrayList<Part>();

		for (int i = 0; i < number; i++) {
			Part tmp = new Part(i, "Name-" + i, "MFGR-" + i, "Brand-" + i, "Type-" + i, i, "Container-" + i, i, "Comment-" + i);
			objectList.add(tmp);
		}

		return objectList;
	}

	public Part javaDefaultDeserialization(byte[] buf) {

		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		ObjectInputStream objectInputStream;

		Part p = null;

		try {
			objectInputStream = new ObjectInputStream(b);
			p = (Part) objectInputStream.readObject();

		} catch (IOException | ClassNotFoundException e) {
			logger.error("Can read object Part from byteArray", e);
		}
		return p;

	}

	public JsonObjectBuilder jsonJsonObjectBuilder() {

		JsonObjectBuilder partBuilder = Json.createObjectBuilder();

		partBuilder.add("partid", this.getPartID());
		partBuilder.add("name", this.getName());
		partBuilder.add("mfgr", this.getMfgr());
		partBuilder.add("brand", this.getBrand());
		partBuilder.add("type", this.getType());
		partBuilder.add("container", this.getContainer());
		partBuilder.add("retailprice", this.getRetailPrice());
		partBuilder.add("comment", this.getComment());

		return partBuilder;

	}

	@Override
	public byte[] jsonSerialization() {

		JsonObjectBuilder partBuilder = Json.createObjectBuilder();

		partBuilder.add("partid", this.getPartID());

		if (this.getName() != null)
			partBuilder.add("name", this.getName());

		if (this.getMfgr() != null)
			partBuilder.add("mfgr", this.getMfgr());

		if (this.getBrand() != null)
			partBuilder.add("brand", this.getBrand());

		if (this.getType() != null)
			partBuilder.add("type", this.getType());

		if (this.getContainer() != null)
			partBuilder.add("container", this.getContainer());

		// if(this.getRetailPrice()!=0)
		partBuilder.add("retailprice", this.getRetailPrice());

		if (this.getComment() != null)
			partBuilder.add("comment", this.getComment());

		JsonObject partJsonObject = partBuilder.build();

		Map<String, Object> properties = new HashMap<String, Object>(1);
		properties.put(JsonGenerator.PRETTY_PRINTING, true);
		JsonWriterFactory writerFactory = Json.createWriterFactory(properties);

		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		try {
			GZIPOutputStream gzipOutputStream = new GZIPOutputStream(bos);
			JsonWriter jsonWriter = writerFactory.createWriter(gzipOutputStream);
			jsonWriter.writeObject(partJsonObject);
			jsonWriter.close();
		} catch (IOException e) {
			logger.error(e);
		}

		// Get the bytes of the serialized JSON object
		byte[] buf = bos.toByteArray();
		return buf;

	}

	public Part readJSONSupplier(JsonObject jsonObject) {

		Part myPart = new Part();

		myPart.setPartID(jsonObject.getInt("partid"));
		myPart.setName(jsonObject.getString("name"));
		myPart.setMfgr(jsonObject.getString("mfgr"));
		myPart.setBrand(jsonObject.getString("brand"));
		myPart.setType(jsonObject.getString("type"));
		myPart.setContainer(jsonObject.getString("container"));
		myPart.setRetailPrice(jsonObject.getInt("retailprice"));
		myPart.setComment(jsonObject.getString("comment"));

		return myPart;
	}

	public Part jsonDeserialization(byte[] buf) {

		Part myPart = new Part();
		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		try {
			InputStream gzis = new GZIPInputStream(b);
			// create JsonReader object
			JsonReader jsonReader = Json.createReader(gzis);

			// get JsonObject from JsonReader
			JsonObject jsonObject = jsonReader.readObject();

			myPart.setPartID(jsonObject.getInt("partid"));
			myPart.setName(jsonObject.getString("name"));
			myPart.setMfgr(jsonObject.getString("mfgr"));
			myPart.setBrand(jsonObject.getString("brand"));
			myPart.setType(jsonObject.getString("type"));
			myPart.setContainer(jsonObject.getString("container"));
			myPart.setRetailPrice(jsonObject.getInt("retailprice"));
			myPart.setComment(jsonObject.getString("comment"));

			// we can close IO resource and JsonReader now
			jsonReader.close();
		} catch (Exception e) {
			logger.error("Can not read JSON object Part from byteArray", e);
		}
		return myPart;
	}

	/**
	 * This method does the protocolBuffer Serialization for class Part
	 */
	@Override
	public byte[] protocolBufferWrite() {

    	PartP.Builder m_part = PartP.newBuilder();
		
//		PartP.Builder m_part=ProtocolBufferSinglton.getInstance().getPart();
		
		m_part.setPartID(this.partID);
		m_part.setName(this.name);
		m_part.setBrand(this.brand);
		m_part.setComment(this.comment);
		m_part.setContainer(this.container);
		m_part.setMfgr(this.mfgr);
		m_part.setRetailPrice(this.retailPrice);
		m_part.setSize(this.size);
		m_part.setType(this.type);

		byte[] buf = null;

		ByteArrayOutputStream bos = new ByteArrayOutputStream();

		// Write the class room to disk.
		try {
			m_part.build().writeTo(bos);
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
		Part m_part = new Part();
		ByteArrayInputStream b = new ByteArrayInputStream(buf);

		PartP.Builder protocPart = PartP.newBuilder();
//		PartP.Builder protocPart=ProtocolBufferSinglton.getInstance().getPart();

		CodedInputStream stream = CodedInputStream.newInstance(b);

		// mergeFrom
		try {
			protocPart.mergeFrom(stream);
		} catch (IOException e) {
			logger.error(" Protocol Buffer Reader  in Part ", e);
		}

		m_part.setName(protocPart.getName());
		m_part.setBrand(protocPart.getBrand());
		m_part.setComment(protocPart.getComment());
		m_part.setContainer(protocPart.getContainer());
		m_part.setMfgr(protocPart.getMfgr());
		m_part.setRetailPrice(protocPart.getRetailPrice());
		m_part.setSize(protocPart.getSize());
		m_part.setType(protocPart.getType());

		return m_part;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getBrand() {
		return brand;
	}

	public void setBrand(String brand) {
		this.brand = brand;
	}

	public String getMfgr() {
		return mfgr;
	}

	public void setMfgr(String mfgr) {
		this.mfgr = mfgr;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public int getSize() {
		return size;
	}

	public void setSize(int size) {
		this.size = size;
	}

	public String getContainer() {
		return container;
	}

	public void setContainer(String container) {
		this.container = container;
	}

	public double getRetailPrice() {
		return retailPrice;
	}

	public void setRetailPrice(double retailPrice) {
		this.retailPrice = retailPrice;
	}

	public String getComment() {
		return comment;
	}

	public void setComment(String comment) {
		this.comment = comment;
	}

	public String toString() {
		String stringS = this.getPartID() + " - "  + this.getName() + " - " + this.getBrand() + " - " + this.getComment() + " - " + this.getContainer() + " - " + this.getMfgr() + " - " + this.getRetailPrice() + " - " + this.getSize();
		return stringS;
	}

	public int getPartID() {
		return partID;
	}

	public void setPartID(int partID) {
		this.partID = partID;
	}

	@Override
	public int compareTo(RootData o) {
		Part myOrder = (Part) o;
		return Integer.compare(myOrder.getPartID(), this.getPartID());
	}

	@Override
	public byte[] writeByteBuffer() {

		// private int partID; // 4 bytes
		// private int size; // 4 bytes
		// private double retailPrice; // 4 bytes

		// private String container; // 8 bytes
		// private String name; // 8 bytes
		// private String mfgr; // 8 bytes
		// private String brand; // 8 bytes
		// private String type; // 8 bytes
		// private String comment; // 8 bytes

		// Container
		byte[] containerBytes = container.getBytes(Charset.forName("UTF-8"));
		byte[] nameBytes = name.getBytes(Charset.forName("UTF-8"));
		byte[] mfgrBytes = mfgr.getBytes(Charset.forName("UTF-8"));
		byte[] brandBytes = brand.getBytes(Charset.forName("UTF-8"));
		byte[] typeBytes = type.getBytes(Charset.forName("UTF-8"));
		byte[] commentBytes = comment.getBytes(Charset.forName("UTF-8"));

		// allocate a buffer to write this object
		ByteBuffer byteBuffer = ByteBuffer.allocate(40 + containerBytes.length + nameBytes.length + mfgrBytes.length + brandBytes.length + typeBytes.length + commentBytes.length);

		byteBuffer.putInt(partID); // 4 bytes
		byteBuffer.putInt(size); // 4 bytes
		byteBuffer.putDouble(retailPrice); // 8 bytes

		byteBuffer.putInt(containerBytes.length); // 4 bytes size of "container string"
		byteBuffer.put(containerBytes);

		// name
		byteBuffer.putInt(nameBytes.length); // 4 bytes size of "container string"
		byteBuffer.put(nameBytes);

		// mfgr
		byteBuffer.putInt(mfgrBytes.length); // 4 bytes size of "container string"
		byteBuffer.put(mfgrBytes);

		// brand
		byteBuffer.putInt(brandBytes.length); // 4 bytes size of "container string"
		byteBuffer.put(brandBytes);

		// type
		byteBuffer.putInt(typeBytes.length); // 4 bytes size of "container string"
		byteBuffer.put(typeBytes);

		// comment
		byteBuffer.putInt(commentBytes.length); // 4 bytes size of "container string"
		byteBuffer.put(commentBytes);

		return byteBuffer.array();
	}

	@Override
	public RootData readByteBuffer(byte[] buf) {

		// allocate a buffer for 12 bytes to write an integer and a double
		ByteBuffer byteBuffer = ByteBuffer.wrap(buf);

		int tmpPartID = byteBuffer.getInt(); // 4 bytes
		int tmpSize = byteBuffer.getInt(); // 4 bytes = 8
		Double tmpRetailPrice = byteBuffer.getDouble(); // 8 bytes = 16

		int stringSize;

		// Get Container
		stringSize = byteBuffer.getInt(); // 4 bytes = 20
		String tmpContainer = extractString(byteBuffer, stringSize);

		// Get name
		stringSize = byteBuffer.getInt(); // 4 bytes
		String tmpName = extractString(byteBuffer, stringSize);

		// mfgr
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpMfgr = extractString(byteBuffer, stringSize);

		// brand
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpBrand = extractString(byteBuffer, stringSize);

		// type
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpType = extractString(byteBuffer, stringSize);

		// comment
		stringSize = byteBuffer.getInt(); // 4 bytes = 24
		String tmpComment = extractString(byteBuffer, stringSize);

		return new Part(tmpPartID, tmpName, tmpMfgr, tmpBrand, tmpType, tmpSize, tmpContainer, tmpRetailPrice, tmpComment);

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

			gen.writeFieldName("partid");
			gen.writeNumber(this.getPartID());

			gen.writeFieldName("name");
			gen.writeString(this.getName());

			gen.writeFieldName("mfgr");
			gen.writeString(this.getMfgr());

			gen.writeFieldName("brand");
			gen.writeString(this.getBrand());

			gen.writeFieldName("type");
			gen.writeString(this.getType());

			gen.writeFieldName("container");
			gen.writeString(this.getContainer());

			gen.writeFieldName("retailprice");
			gen.writeNumber(this.getRetailPrice());

			gen.writeFieldName("comment");
			gen.writeString(this.getComment());

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

		Part tmp = new Part();

		try {
			com.fasterxml.jackson.core.JsonParser parser = BsonSinglton.getInstance().getFactory().createJsonParser(bais);

			parser.nextToken();

			while (parser.nextToken() != JsonToken.END_OBJECT) {
				String fieldname = parser.getCurrentName();
				parser.nextToken();

				switch (fieldname) {
				case "partid":
					tmp.setPartID(parser.getIntValue());
					break;
				case "name":
					tmp.setName(parser.getText());
					break;
				case "mfgr":
					tmp.setMfgr(parser.getText());
					break;
				case "brand":
					tmp.setBrand(parser.getText());
					break;
				case "type":
					tmp.setType(parser.getText());
					break;
				case "container":
					tmp.setContainer(parser.getText());
					break;
				case "retailprice":
					tmp.setRetailPrice(parser.getDoubleValue());
					break;
				case "comment":
					tmp.setComment(parser.getText());
					break;
				}

			}

		} catch (IOException e) {
			logger.error("Element bsonSerialization ", e);

		}

		return tmp;
	}

}
