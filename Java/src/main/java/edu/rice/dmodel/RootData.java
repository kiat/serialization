package edu.rice.dmodel;

import java.io.Serializable;
import java.util.List;

import com.esotericsoftware.kryo.Kryo;

/**
 * 
 * @author Kia Teymourian
 * 
 *
 */
public interface RootData extends Serializable, Comparable<RootData> {

	/**
	 * This method generates n number of object of this class.
	 * @param number how many number of objects should be created 
	 * @return a list of RootData objects 
	 */
	List<RootData> generateObjects(int number);

	/**
	 * This method serialize to its java default serialization format. 
	 * @return a byte array of java default serialization
	 */
	byte[] javaDefaultSerialization();

	RootData javaDefaultDeserialization(byte[] buf);

	byte[] jsonSerialization();

	RootData jsonDeserialization(byte[] buf);

	byte[] protocolBufferWrite();

	RootData protocolBufferRead(byte[] buf);

	byte[] kryoSerialization(Kryo kryo);

	RootData kryoDeserialization(byte[] buf, Class<?> myclass,Kryo kryo);

	byte[] writeByteBuffer();

	RootData readByteBuffer(byte[] buffData);

	byte[] bsonSerialization();

	RootData bsonDeSerialization(byte[] buffData, Class<?> myclass);
	
	String toString();


}
