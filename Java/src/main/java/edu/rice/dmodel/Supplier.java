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

import com.fasterxml.jackson.core.JsonToken;
import com.google.protobuf.CodedInputStream;

import edu.rice.pdb.serialization.BsonSinglton;
//import edu.rice.pdb.serialization.ProtocolBufferSinglton;
import edu.rice.pdb.serialization.proto.CustomerProtos.CustomerP.OrderP.LineItemP.SupplierP;

public class Supplier extends Base implements RootData  {

	/**
	 * This class represents a supplier as it is specified in TPC-H benchmark
	 * schema
	 */
	private static final long serialVersionUID = -2215947411317295533L;

	// CREATE TABLE [dbo].[SUPPLIER](
	// [S_SUPPKEY] [int] NOT NULL,
	// [S_NAME] [char](25) NOT NULL,
	// [S_ADDRESS] [varchar](40) NOT NULL,
	// [S_NATIONKEY] [int] NOT NULL,
	// [S_PHONE] [char](15) NOT NULL,
	// [S_ACCTBAL] [decimal](15, 2) NOT NULL,
	// [S_COMMENT] [varchar](101) NOT NULL
	// );

	private int supplierKey;
	private String name;
	private String address;
	private int nationKey;
	private String phone;
	private double accbal;
	private String comment;

	public Supplier() {

	}

	public Supplier(int supplierKey, String name, String address, int nationKey, String phone, double accbal, String comment) {
		super();
		this.supplierKey = supplierKey;
		this.name = name;
		this.address = address;
		this.nationKey = nationKey;
		this.phone = phone;
		this.accbal = accbal;
		this.comment = comment;
	}
	
	public Supplier generateSupplier() {
		int i = 0;
		return	new Supplier( i, "name" + i, "address" + i, i, "phone" + i, i, "comment"+ i);
		
	}

	public ArrayList<RootData> generateObjects(int number) {

		ArrayList<RootData> objectList = new ArrayList<RootData>();

		for (int i = 0; i < number; i++) {
			Supplier tmp = new Supplier( i, "name" + i, "address" + i, i, "phone" + i, i, "comment"+ i);
			objectList.add(tmp);
		}

		return objectList;
	}

	public JsonObjectBuilder jsonJsonObjectBuilder() {
		JsonObjectBuilder supplierBuilder = Json.createObjectBuilder();

		supplierBuilder.add("name", this.getName())
		               .add("supplierkey", this.getSupplierKey())
		               .add("address", this.getAddress())
		               .add("nationkey", this.getNationKey())
		               .add("phone", this.getPhone())
		               .add("accbal", this.getAccbal())
		               .add("comment", this.getComment());

		return supplierBuilder;
	}

	public Supplier readJSONSupplier(JsonObject supplierJsonObject) {

		Supplier m_supplier = new Supplier();
		m_supplier.setName(supplierJsonObject.getString("name"));
		m_supplier.setSupplierKey(supplierJsonObject.getInt("supplierkey"));
		m_supplier.setAddress(supplierJsonObject.getString("address"));
		m_supplier.setAccbal(supplierJsonObject.getInt("accbal"));
		m_supplier.setNationKey(supplierJsonObject.getInt("nationkey"));
		m_supplier.setComment(supplierJsonObject.getString("comment"));
		m_supplier.setPhone(supplierJsonObject.getString("phone"));

		return m_supplier;
	}

	public int getSupplierKey() {
		return supplierKey;
	}

	public void setSupplierKey(int supplierKey) {
		this.supplierKey = supplierKey;
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

	public int getNationKey() {
		return nationKey;
	}

	public void setNationKey(int nationKey) {
		this.nationKey = nationKey;
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

	public String getComment() {
		return comment;
	}

	public void setComment(String comment) {
		this.comment = comment;
	}

	public String toString() {

		String s = this.supplierKey + " -" + this.name + " -" + this.address + " -" + this.nationKey + " -" + this.phone + " -"
				+ this.accbal + " -" + this.comment;
		return s;
	}
	
	
	 public byte[] writeByteBuffer(){

//			private int supplierKey; // 4 bytes 
//			private int nationKey;  // 4 bytes 
//			private double accbal;  // 8 bytes 
 
		 
//			private String name; 
//			private String address;
//			private String phone;
//			private String comment;

		 
		    byte[] nameBytes = name.getBytes(Charset.forName("UTF-8"));
			byte[] addressBytes = address.getBytes(Charset.forName("UTF-8"));
			byte[] phoneBytes = phone.getBytes(Charset.forName("UTF-8"));
			byte[] commentBytes = comment.getBytes(Charset.forName("UTF-8"));

			// allocate a buffer to write this object
			ByteBuffer byteBuffer = ByteBuffer.allocate(32 + nameBytes.length + addressBytes.length + phoneBytes.length + commentBytes.length);

			byteBuffer.putInt(supplierKey); // 4 bytes
			byteBuffer.putInt(nationKey); // 4 bytes
			byteBuffer.putDouble(accbal); // 8 bytes


			// name
			byteBuffer.putInt(nameBytes.length); // 4 bytes size of "container string"
			byteBuffer.put(nameBytes);

			// address
			byteBuffer.putInt(addressBytes.length); // 4 bytes size of "container string"
			byteBuffer.put(addressBytes);

			// phone
			byteBuffer.putInt(phoneBytes.length); // 4 bytes size of "container string"
			byteBuffer.put(phoneBytes);
		
			// comment
			byteBuffer.putInt(commentBytes.length); // 4 bytes size of "container string"
			byteBuffer.put(commentBytes);

			return byteBuffer.array();
	 }
	 

	 public Supplier readByteBuffer(byte[] buf){
		 
		 

//			private int supplierKey; // 4 bytes 
//			private int nationKey;  // 4 bytes 
//			private double accbal;  // 8 bytes 

		 
//			private String name; 
//			private String address;
//			private String phone;
//			private String comment;
		 
		    // allocate a buffer for 12 bytes to write an integer and a double
			ByteBuffer byteBuffer = ByteBuffer.wrap(buf);

			int tmpSupplierKey = byteBuffer.getInt(); // 4 bytes
			int tmpNationKey = byteBuffer.getInt(); // 4 bytes = 8
			Double tmpAccbal = byteBuffer.getDouble(); // 8 bytes = 16

			int stringSize;

			// Get name
			stringSize = byteBuffer.getInt(); // 4 bytes = 20
			String tmpName = extractString(byteBuffer, stringSize);

			// address
			stringSize = byteBuffer.getInt(); // 4 bytes = 24
			String tmpAddress = extractString(byteBuffer, stringSize);

			// phone
			stringSize = byteBuffer.getInt(); // 4 bytes = 24
			String tmpPhone = extractString(byteBuffer, stringSize);

			// comment
			stringSize = byteBuffer.getInt(); // 4 bytes = 24
			String tmpComment = extractString(byteBuffer, stringSize);
		 return new Supplier(tmpSupplierKey, tmpName,tmpAddress, tmpNationKey, tmpPhone, tmpAccbal, tmpComment) ;
	 }

	@Override
	public int compareTo(RootData o) {
		Supplier mySupplier=(Supplier) o;
		return Integer.compare(mySupplier.getSupplierKey(), this.getSupplierKey());	
	}


	@Override
	public byte[] jsonSerialization() {
		JsonObjectBuilder supplierBuilder = this.jsonJsonObjectBuilder();;

		JsonObject partJsonObject = supplierBuilder.build();

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

	@Override
	public RootData jsonDeserialization(byte[] buf) {
		Supplier mySupplier = new Supplier();
		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		try {
			InputStream gzis = new GZIPInputStream(b);
			
			// create JsonReader object
			JsonReader jsonReader = Json.createReader(gzis);

			// get JsonObject from JsonReader
			JsonObject jsonObject = jsonReader.readObject();

			mySupplier.setName(jsonObject.getString("name"));
			mySupplier.setSupplierKey(jsonObject.getInt("supplierkey"));
			mySupplier.setAddress(jsonObject.getString("address"));
			mySupplier.setAccbal(jsonObject.getInt("accbal"));
			mySupplier.setNationKey(jsonObject.getInt("nationkey"));
			mySupplier.setComment(jsonObject.getString("comment"));
			mySupplier.setPhone(jsonObject.getString("phone"));
			
			// we can close IO resource and JsonReader now
			jsonReader.close();
		} catch (Exception e) {
			logger.error("Can not read JSON object Supplier from byteArray", e);
		}
		return mySupplier;
	}

	@Override
	public byte[] protocolBufferWrite() {
		SupplierP.Builder mySupplier = SupplierP.newBuilder();
//		SupplierP.Builder mySupplier = ProtocolBufferSinglton.getInstance().getSupplier();

		mySupplier.setSupplierKey(this.supplierKey);
		mySupplier.setName(this.name);
		mySupplier.setAddress(this.address);
		mySupplier.setAccbal(this.accbal);
		mySupplier.setNationKey(this.nationKey );
		mySupplier.setComment(this.comment);
		mySupplier.setPhone(this.phone);

		byte[] buf = null;

		ByteArrayOutputStream bos = new ByteArrayOutputStream();

		// Write the class room to disk.
		try {
			mySupplier.build().writeTo(bos);
			// Get the bytes of the serialized JSON object
			buf = bos.toByteArray();
			bos.close();

		} catch (IOException e) {
			logger.error("Protocol Buffer Writer in Supplier ", e);
		}

		return buf;
	}

	@Override
	public RootData protocolBufferRead(byte[] buf) {
		Supplier mySupplier = new Supplier();
		ByteArrayInputStream b = new ByteArrayInputStream(buf);

		SupplierP.Builder protocSupplier = SupplierP.newBuilder();
		
//		SupplierP.Builder protocSupplier = ProtocolBufferSinglton.getInstance().getSupplier();

		CodedInputStream stream = CodedInputStream.newInstance(b);

		// mergeFrom
		try {
			protocSupplier.mergeFrom(stream);
		} catch (IOException e) {
			logger.error(" Protocol Buffer Reader  in Supplier ", e);
		}

		mySupplier.setName(protocSupplier.getName());
		mySupplier.setSupplierKey(protocSupplier.getSupplierKey());
		mySupplier.setAddress(protocSupplier.getAddress());
		mySupplier.setAccbal(protocSupplier.getAccbal());
		mySupplier.setNationKey(protocSupplier.getNationKey());
		mySupplier.setComment(protocSupplier.getComment());
		mySupplier.setPhone(protocSupplier.getPhone());

		return mySupplier;
	}

	@Override
	public RootData javaDefaultDeserialization(byte[] buf) {
		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		ObjectInputStream objectInputStream;

		Supplier p = null;

		try {
			objectInputStream = new ObjectInputStream(b);
			p = (Supplier) objectInputStream.readObject();

		} catch (IOException | ClassNotFoundException e) {
			logger.error("Can read object Supplier from byteArray", e);
		}
		return p;
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

			gen.writeFieldName("name");
			gen.writeNumber(this.getName());

			gen.writeFieldName("supplierkey");
			gen.writeNumber(this.getSupplierKey());

			gen.writeFieldName("address");
			gen.writeString(this.getAddress());

			gen.writeFieldName("accbal");
			gen.writeNumber(this.getAccbal());

			gen.writeFieldName("nationkey");
			gen.writeNumber(this.getNationKey());

			gen.writeFieldName("phone");
			gen.writeString(this.getPhone());

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

		Supplier tmp = new Supplier();

		try {
			com.fasterxml.jackson.core.JsonParser parser = BsonSinglton.getInstance().getFactory().createJsonParser(bais);

			parser.nextToken();

			while (parser.nextToken() != JsonToken.END_OBJECT) {
				String fieldname = parser.getCurrentName();
				parser.nextToken();

				switch (fieldname) {
				case "name":
					tmp.setName(parser.getText());
					break;
				case "supplierkey":
					tmp.setSupplierKey(parser.getIntValue());
					break;
				case "address":
					tmp.setAddress(parser.getText());
					break;
				case "accbal":
					tmp.setAccbal(parser.getDoubleValue());
					break;
				case "nationkey":
					tmp.setNationKey(parser.getIntValue());
					break;
				case "phone":
					tmp.setPhone(parser.getText());
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
