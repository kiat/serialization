package edu.rice.dmodel;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Random;
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

import edu.rice.pdb.read.WriteAndRead;
import edu.rice.pdb.serialization.BsonSinglton;
import edu.rice.pdb.serialization.proto.LongVectorProtos.LongVectorP;

public class Element extends Base implements RootData {
	static Logger logger = Logger.getLogger(Element.class);

	// this class is used to represent a long vector by generating multiple
	// objects as elements in a vector.
	private Long myInteger;
	private Double myDouble;

	public Long getMyInteger() {
		return myInteger;
	}

	public void setMyInteger(Long myInteger) {
		this.myInteger = myInteger;
	}

	public Double getMyDouble() {
		return myDouble;
	}

	public void setMyDouble(Double myDouble) {
		this.myDouble = myDouble;
	}

	private static final long serialVersionUID = -3357709441198915272L;

	/*
	 * 
	 */
	public Element() {
		super();
	}

	public Element(Long position, Double value) {
		super();
		this.myInteger = position;
		this.myDouble = value;
	}


	@Override
	public List<RootData> generateObjects(int number) {
		ArrayList<RootData> objectList = new ArrayList<RootData>();

		Random r = new Random();

		for (long i = 0; i < number; i++) {
			int randomnumnber = r.nextInt(number);
			double d = (double) randomnumnber;

			RootData tmp = new Element( i, (Double) d);
			objectList.add(tmp);
		}

		return objectList;
	}

	/**
	 * This method generates random sparse vectors
	 * @param size  size of the vector 
	 * @param fill  filling size of vector 
	 * @return
	 */
	public ArrayList<RootData> generateRandomObjects(int size, int fill) {
		ArrayList<RootData> objectList = new ArrayList<RootData>();

		HashSet<Long> myRandomIndexies=new HashSet<Long>(); 
		
		for (int i = 0; i < fill; i++) {
			new WriteAndRead();

			double randomValue = WriteAndRead.generateRandom(0, size);
		
			long randomPosition= WriteAndRead.generateRandom(0, size);
			
			while(myRandomIndexies.contains(randomPosition)){
				randomPosition = WriteAndRead.generateRandom(0, size);
			}

			myRandomIndexies.add(randomPosition);
			
			Element tmp = new Element(randomPosition, randomValue);
			objectList.add((RootData) tmp );
		}
		
		// sort before sending
		Collections.sort(objectList);
		
		
		return objectList;
	}
	
	
	
	
	@Override
	public Element javaDefaultDeserialization(byte[] buf) {
		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		ObjectInputStream objectInputStream;

		Element myLongVector = null;

		try {
			objectInputStream = new ObjectInputStream(b);
			myLongVector = (Element) objectInputStream.readObject();

		} catch (IOException | ClassNotFoundException e) {
			logger.error("Can read object Vector from byteArray", e);
		}
		return myLongVector;
	}
	
	
	@Override
	public byte[] jsonSerialization() {
		JsonObjectBuilder partBuilder = Json.createObjectBuilder();

		if (this.getMyInteger() != null)
			partBuilder.add("pos", this.getMyInteger());

		if (this.getMyDouble() != null)
			partBuilder.add("double", this.getMyDouble().toString());

		JsonObject partJsonObject = partBuilder.build();

		Map<String, Object> properties = new HashMap<String, Object>(1);
		properties.put(JsonGenerator.PRETTY_PRINTING, true);
		JsonWriterFactory writerFactory = Json.createWriterFactory(properties);
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		byte[] buf=null;
		try {
			GZIPOutputStream gzipOutputStream = new GZIPOutputStream(bos);
			JsonWriter jsonWriter = writerFactory.createWriter(gzipOutputStream);
			jsonWriter.writeObject(partJsonObject);
			jsonWriter.close();
			buf = bos.toByteArray();
			bos.close();

		} catch (IOException e) {
			logger.error(e);
		}

		// Get the bytes of the serialized JSON object
		return buf;
	}

	@Override
	public Element jsonDeserialization(byte[] buf) {
		Element myLongVector = new Element();

		ByteArrayInputStream b = new ByteArrayInputStream(buf);
		try {
			InputStream gzis = new GZIPInputStream(b);
			// create JsonReader object
			JsonReader jsonReader = Json.createReader(gzis);

			// get JsonObject from JsonReader
			JsonObject jsonObject = jsonReader.readObject();

			myLongVector.setMyInteger((long) jsonObject.getInt("pos"));

			// We need to parse to double

			Double tmp = Double.parseDouble(jsonObject.getString("double"));
			myLongVector.setMyDouble(tmp);
			
			// we can close IO resource and JsonReader now
			jsonReader.close();
			gzis.close();
		} catch (Exception e) {
			logger.error("Can not read JSON object ELement from byteArray", e);
		}
		return myLongVector;
	}

	@Override
	public byte[] protocolBufferWrite() {
		LongVectorP.Builder m_longVector = LongVectorP.newBuilder();

		m_longVector.setPosition(this.myInteger);
		m_longVector.setValue(this.myDouble);

		byte[] buf = null;

		ByteArrayOutputStream bos = new ByteArrayOutputStream();

		// Write the class room to disk.
		try {
			m_longVector.build().writeTo(bos);
			// Get the bytes of the serialized JSON object
			buf = bos.toByteArray();
			bos.close();

		} catch (IOException e) {
			logger.error("Protocol Buffer Writer in LongVector ", e);
		}

		return buf;
	}

	@Override
	public Element protocolBufferRead(byte[] buf) {
		Element m_part = new Element();
		ByteArrayInputStream b = new ByteArrayInputStream(buf);

		LongVectorP.Builder longVectorProto = LongVectorP.newBuilder();
		CodedInputStream stream = CodedInputStream.newInstance(b);

		// mergeFrom
		try {
			longVectorProto.mergeFrom(stream);
		} catch (IOException e) {
			logger.error(" Protocol Buffer Reader in Element ", e);
		}
		m_part.setMyInteger(longVectorProto.getPosition());
		m_part.setMyDouble(longVectorProto.getValue());

		return m_part;
	}

	public String toString() {
		return "(" + this.myInteger + "," + this.myDouble + ")";
	}

	@Override
	public int compareTo(RootData myvector) {
		Element myElement = (Element) myvector;
	
		return Long.compare(myElement.getMyInteger(), this.myInteger);
	}

	/**
	 * Binary Serialization of this object using ByteBuffer
	 * 
	 * @return
	 */
	public byte[] writeByteBuffer() {

		// allocate a buffer for 12 bytes to write an integer and a double
		ByteBuffer byteBuffer = ByteBuffer.allocate(16);

		byteBuffer.putLong(myInteger); // 8 bytes
		byteBuffer.putDouble(myDouble); // 8 bytes

		return byteBuffer.array();
	}

	/**
	 * Binary De-Serialization of this object using ByteBuffer
	 * 
	 * @param buf
	 * @return
	 */
	public RootData readByteBuffer(byte[] buf) {

		// allocate a buffer for 12 bytes to write an integer and a double
		ByteBuffer byteBuffer = ByteBuffer.wrap(buf);

		long tmpInt = byteBuffer.getLong(); // 8 bytes
		Double tmpDouble = byteBuffer.getDouble(); // 8 bytes

		// Get the bytes of the serialized JSON object
		return new Element(tmpInt, tmpDouble);
	}

	@Override
	public byte[] bsonSerialization() {

		// serialize data
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		try {
			com.fasterxml.jackson.core.JsonGenerator gen = BsonSinglton.getInstance().getFactory().createJsonGenerator(baos);

			gen.writeStartObject();
			gen.writeFieldName("Integer");
			gen.writeNumber(this.getMyInteger());

			gen.writeFieldName("Double");
			gen.writeNumber(this.getMyDouble());

			gen.close();

		} catch (IOException e) {

			logger.error("Element bsonSerialization ", e);
		}
		return baos.toByteArray();
	}

	@Override
	public RootData bsonDeSerialization(byte[] buf, Class<?> myclass) {

		ByteArrayInputStream bais = new ByteArrayInputStream(buf);

		Element tmpElement = new Element();

		try {
			com.fasterxml.jackson.core.JsonParser parser = BsonSinglton.getInstance().getFactory().createJsonParser(bais);

			parser.nextToken();

			while (parser.nextToken() != JsonToken.END_OBJECT) {
				String fieldname = parser.getCurrentName();
				parser.nextToken();

				switch (fieldname) {
				case "Integer":
					tmpElement.setMyInteger(parser.getLongValue());
					break;
				case "Double":
					tmpElement.setMyDouble(parser.getDoubleValue());
					break;
				}

			}

		} catch (IOException e) {
			logger.error("Element bsonSerialization ", e);

		}

		return tmpElement;
	}
	
	
	
	

}
