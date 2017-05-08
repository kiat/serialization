package edu.rice.pdb.sort;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import edu.rice.dmodel.Element;
import edu.rice.dmodel.RootData;
import edu.rice.pdb.read.SerializationMethod;
import edu.rice.pdb.read.SerializedData;
import edu.rice.pdb.serialization.KryoSinglton;
import edu.rice.pdb.util.Utils;

/**
 * 
 * @author Kia
 *
 *         This class implements external sort with different serialization methods.
 */

public class GenerateDataForExternalSort {
	final static Logger logger = Logger.getLogger(GenerateDataForExternalSort.class);

	static int numberOfFiles = 1;
	static int numberOfObjectsInEachFiles = 100000;
	static int numberOfCopies = 10;

	/**
	 * @param args
	 * @throws IOException
	 * @throws ClassNotFoundException
	 */
	public static void main(String[] args) throws IOException, ClassNotFoundException {
		PropertyConfigurator.configure("log4j.properties");

//		numberOfFiles=Integer.parseInt(args[0]);
//		numberOfObjectsInEachFiles=Integer.parseInt(args[1]);

		// Run for different serialization methods
		generateLongVector();
	}

	/**
	 * Generate the long vector data Element(int, double)
	 * 
	 * @param method
	 */
	public static void generateLongVector() {

		RootData tmpRootData = new Element();

		List<RootData> m_list = tmpRootData.generateObjects(numberOfObjectsInEachFiles * numberOfFiles);
		// SerializationMethod[] methodArray = { SerializationMethod.JSON};
		SerializationMethod[] methodArray = { SerializationMethod.JAVADEFAULT, SerializationMethod.JSON, SerializationMethod.BSON, SerializationMethod.PROTOCOL,
				SerializationMethod.KRYO, SerializationMethod.BYTEBUFFER };
		
//		SerializationMethod[] methodArray = { SerializationMethod.JAVADEFAULT, SerializationMethod.KRYO};
		for (SerializationMethod method : methodArray) {
			System.out.println("Generating data for Element with serialization: " + method);
			
			
			
			ArrayList<byte[]> byteObjects = new ArrayList<byte[]>();

			for (RootData object : m_list) {
				RootData tmpPart = object;
				switch (method) {
				case JAVADEFAULT:
					byteObjects.add(tmpPart.javaDefaultSerialization());
					break;
				case JSON:
					byteObjects.add(tmpPart.jsonSerialization());
					break;
				case BSON:
					byteObjects.add(tmpPart.bsonSerialization());
					break;
				case PROTOCOL:
					byteObjects.add(tmpPart.protocolBufferWrite());
					break;
				case KRYO:
					byteObjects.add(tmpPart.kryoSerialization(KryoSinglton.getInstance().getKryo()));
					break;
				case BYTEBUFFER:
					byteObjects.add(tmpPart.writeByteBuffer());
					break;
				}
			}

			// Writing back after sorting
			try {
				Utils.writeObjectsWithIndex(byteObjects, numberOfCopies, "inputdata/" + SerializedData.ELEMENT + method + ".index", "inputdata/" + SerializedData.ELEMENT + method + ".object");

			} catch (Exception e) {
				logger.error(e);
			}
			
//			writeToFiles(1, m_list, "inputdata/" + SerializedData.ELEMENT + method, method);
		}

	}

//	static void writeToFiles(int i, List<RootData> m_list_read_from_file, String name, SerializationMethod method) {
//		// Now write them back one by one
//		ArrayList<byte[]> byteObjects = new ArrayList<byte[]>();
//
//		for (RootData object : m_list_read_from_file) {
//			RootData tmpPart = object;
//			switch (method) {
//			case JAVADEFAULT:
//				byteObjects.add(tmpPart.javaDefaultSerialization());
//				break;
//			case JSON:
//				byteObjects.add(tmpPart.jsonSerialization());
//				break;
//			case BSON:
//				byteObjects.add(tmpPart.bsonSerialization());
//				break;
//			case PROTOCOL:
//				byteObjects.add(tmpPart.protocolBufferWrite());
//				break;
//			case KRYO:
//				byteObjects.add(tmpPart.kryoSerialization());
//				break;
//			case BYTEBUFFER:
//				byteObjects.add(tmpPart.writeByteBuffer());
//				break;
//			}
//		}
//
//		// Writing back after sorting
//		try {
//			Utils.writeObjectsWithIndex(byteObjects, numberOfCopies, name + i + ".index", name + i + ".object");
//
//		} catch (Exception e) {
//			logger.error(e);
//		}
//	}

	static int generateRandom(int min, int max) {
		Random rand = new Random();
		int randomNum = rand.nextInt((max - min) + 1) + min;
		return randomNum;
	}
}