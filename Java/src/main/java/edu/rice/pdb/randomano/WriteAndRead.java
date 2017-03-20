package edu.rice.pdb.randomano;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.apache.log4j.Logger;

import edu.rice.dmodel.Customer;
import edu.rice.dmodel.Element;
import edu.rice.dmodel.IndexData;
import edu.rice.dmodel.LineItem;
import edu.rice.dmodel.Order;
import edu.rice.dmodel.Part;
import edu.rice.dmodel.RootData;
import edu.rice.pdb.serialization.Const;
import edu.rice.pdb.serialization.KryoSinglton;
import edu.rice.pdb.util.Utils;

public class WriteAndRead {

	static ByteBuffer bb = ByteBuffer.allocateDirect(Const.PAGESIZE);

	final static Logger logger = Logger.getLogger(WriteAndRead.class);

	public static void writeObjects(SerializationMethod method, SerializedData dataType, int numberOfObjects, String indexFile, String OutputFileName) throws FileNotFoundException, IOException {

		List<RootData> objectList = null;
		RootData m_p = null;
		// Generate different types of Data.
		switch (dataType) {
		case PART:
			m_p = new Part();
			objectList = m_p.generateObjects(numberOfObjects);
			break;
		case LINEITEM:
			m_p = new LineItem();
			objectList = m_p.generateObjects(numberOfObjects);
			break;
		case ORDER:
			m_p = new Order();
			objectList = m_p.generateObjects(numberOfObjects);
			break;
		case CUSTOMER:
			m_p = new Customer();
			objectList = m_p.generateObjects(numberOfObjects);
			break;
		case ELEMENT:
			m_p = new Element();
			objectList = m_p.generateObjects(numberOfObjects);
			break;
		}

		ArrayList<byte[]> byteObjects = new ArrayList<byte[]>();

		// Write them to file with different serialization methods.
		switch (method) {
		case JAVADEFAULT:
			for (RootData object : objectList) {
				RootData tmpPart = object;
				byteObjects.add(tmpPart.javaDefaultSerialization());
			}
			break;
		case JSON:
			for (RootData object : objectList) {
				RootData tmpPart = object;
				byteObjects.add(tmpPart.jsonSerialization());
			}
			break;
		case BSON:
			for (RootData object : objectList) {
				RootData tmpPart = object;
				byteObjects.add(tmpPart.bsonSerialization());
			}
			break;
		case PROTOCOL:

			for (RootData object : objectList) {
				RootData tmpPart = object;
				byteObjects.add(tmpPart.protocolBufferWrite());
			}

			break;

		case KRYO:

			for (RootData object : objectList) {
				RootData tmpPart = object;
				byteObjects.add(tmpPart.kryoSerialization(KryoSinglton.getInstance().getKryo()));
			}

			break;

		case BYTEBUFFER:

			for (RootData object : objectList) {
				RootData tmpPart = object;
				byteObjects.add(tmpPart.writeByteBuffer());
			}

			break;

		}

		Utils.writeObjectsWithIndex(byteObjects, 1, indexFile, OutputFileName);

		logger.debug("Index File " + dataType + " for " + method + " Written and closed ... ");
	}

	/**
	 * This method reads objects based on serialization and data type
	 * 
	 * @param method
	 * @param myDataType
	 * @param numberOfObjectsToRead
	 * @param numberOfObjectsInTotal
	 * @param indexFile
	 * @param fileinput
	 * @throws IOException
	 * @throws ClassNotFoundException
	 */
	public static ArrayList<RootData> readObjects(SerializationMethod method, RootData myDataType, int numberOfObjectsToRead, int indexSizeToRead, String indexFile, String fileinput, boolean seq, boolean randomStart)
			throws IOException, ClassNotFoundException {

		ArrayList<RootData> readListOfObject = new ArrayList<RootData>();
		logger.debug("Now reading objects back!");

		IndexData sizeIndex = Utils.readIndex(indexFile, 0);

		long[] indexStarts = sizeIndex.getStarts();
		int[] indexLenghts = sizeIndex.getLenghts();

//		System.out.println(indexStarts.length);
//		System.out.println(indexLenghts.length);
		
		long startTime = 0;
		double elapsedIOTime = 0;


		Path path = Paths.get(fileinput);
		FileChannel fileChannel = FileChannel.open(path);

		// NOW read the objects from memory
		// START OF Time calculation
		startTime = System.nanoTime();

		if (seq) {

			// Read back object sequentially from file with only one time read from file
			long seekPositionTmp = Long.MAX_VALUE;

			int lenght_each_object;

			for (int x = 0; x < numberOfObjectsToRead; x++) {
				long position_each_object = indexStarts[x];
				int pageID = (int) (position_each_object / Const.PAGESIZE);

				if (x != indexStarts.length - 1) {
					// if we do not read the last object in the file.  
					int pageIDNext = (int) (indexStarts[x + 1] / Const.PAGESIZE);
					
					if (pageID == pageIDNext) {
					// if this object and next object are on the same page then it is simple :)
						lenght_each_object = (int) (indexStarts[x + 1] - indexStarts[x]);
					} else {
					// if there are not, then we get the length of the last object on this page from the index file. 						
						lenght_each_object = indexLenghts[pageID];
					}
				} else {
					// if this is the last object then we get its length from the index file. 
					lenght_each_object = indexLenghts[pageID];
				}

				// get the correct page from the RAM - reduce page number from the page number of start page
				// we store them in an array list that start from 0
				long seekPosition = (long) (pageID) * Const.PAGESIZE;

				// Only if the object is on another page it loads that page.
				if (seekPosition != seekPositionTmp) {

					bb.clear();

					long tmpTime = System.nanoTime();

					// Disk I/O
					fileChannel.position(seekPosition);
					fileChannel.read(bb); // read object data from disk

					// Time Calculation
					elapsedIOTime += (System.nanoTime() - tmpTime) / 1000000000.0;

					seekPositionTmp = seekPosition;
				}

				int relativePosition = (int) (position_each_object - pageID * Const.PAGESIZE);

				byte[] buffData_each_obecjt = new byte[lenght_each_object];

				// Copy part of the byte buffer to another byte array
				bb.position(relativePosition);
				bb.get(buffData_each_obecjt, 0, lenght_each_object);

				// NOW read each object
				readListOfObject.add(Utils.readObjectWithSerialization(myDataType, buffData_each_obecjt, method, KryoSinglton.getInstance().getKryo()));
			}

		} else {
			// a buffer list for byte arrays of objects
			long tmpTime = 0;
			int lenght_each_object;
			// Reading back N Objects from file
			for (int i = 0; i < numberOfObjectsToRead; i++) {
				// randomly choose which objects to read

				int randomNumber = generateRandom(0, indexStarts.length - 1);
				long position_each_object = indexStarts[randomNumber];

				int pageID = (int) (position_each_object / Const.PAGESIZE);
				
				if (randomNumber != indexStarts.length - 1) {
					int pageIDNext = (int) (indexStarts[randomNumber + 1] / Const.PAGESIZE);
					if (pageID == pageIDNext) {
						lenght_each_object = (int) (indexStarts[randomNumber + 1] - indexStarts[randomNumber]);
					} else {
						lenght_each_object = indexLenghts[pageID];
					}
				} else {
					lenght_each_object = indexLenghts[pageID];
				}

				byte[] buffData_each_obecjt = new byte[lenght_each_object];

				// this can be very large
				long seekPosition = (long) (pageID) * Const.PAGESIZE;

				tmpTime = System.nanoTime();

				fileChannel.position(seekPosition); // seek to the object data position
				// Get a channel directly to this position.
				// read the page
				bb.clear();
				fileChannel.read(bb); // read object data from disk

				elapsedIOTime += (System.nanoTime() - tmpTime) / 1000000000.0;

				int relativePosition = (int) (position_each_object - pageID * Const.PAGESIZE);
				bb.position(relativePosition);
				bb.get(buffData_each_obecjt, 0, lenght_each_object);

				readListOfObject.add(Utils.readObjectWithSerialization(myDataType, buffData_each_obecjt, method, KryoSinglton.getInstance().getKryo()));
			}
		}

		fileChannel.close();

		// End of time calculation
		long endTime = System.nanoTime();
		double elapsedTotalTime = (endTime - startTime) / 1000000000.0;

		logger.info("[ReadTimeJAVA]#" + method + "#" + seq + "#" + myDataType.getClass().getSimpleName() + "#" + String.format("%.9f", elapsedIOTime) + "#" + String.format("%.9f", elapsedTotalTime));

		return readListOfObject;
	}

	/**
	 * reads objects as ByteArray to send them over the network as they are in Byte
	 * 
	 * @param numberOfObjects
	 * @param indexFile
	 * @param fileinput
	 * @return
	 * @throws IOException
	 * @throws ClassNotFoundException
	 */
	public static DataAndIndex readByteArrayObjects(int numberOfObjects, String indexFile, String fileinput) throws IOException, ClassNotFoundException {
		DataAndIndex mydataAndIndex = null;
		ArrayList<byte[]> myDataAsByteArray = new ArrayList<byte[]>();
		ArrayList<Integer> myDataIndex = new ArrayList<Integer>();

		long startTime = 0;
		double elapsedIOTime = 0;
		
		// NOW read the objects from memory
		// START OF Time calculation
		startTime = System.nanoTime();

		
		
		IndexData sizeIndex = Utils.readIndex(indexFile, 0);

		long[] indexStarts = sizeIndex.getStarts();
		int[] indexLenghts = sizeIndex.getLenghts();



		Path path = Paths.get(fileinput);
		FileChannel fileChannel = FileChannel.open(path);

		// NOW read the objects from memory
		// START OF Time calculation
		startTime = System.nanoTime();

			// Read back object sequentially from file with only one time read from file
			long seekPositionTmp = Long.MAX_VALUE;

			// for (PositionSize psize : listPositionSize) {
			int lenght_each_object;

			for (int x = 0; x < numberOfObjects; x++) {
				long position_each_object = indexStarts[x];
				int pageID = (int) (position_each_object / Const.PAGESIZE);

				if (x != indexStarts.length - 1) {
					// if we do not read the last object in the file.  
					int pageIDNext = (int) (indexStarts[x + 1] / Const.PAGESIZE);
					
					if (pageID == pageIDNext) {
					// if this object and next object are on the same page then it is simple :)
						lenght_each_object = (int) (indexStarts[x + 1] - indexStarts[x]);
					} else {
					// if there are not, then we get the length of the last object on this page from the index file. 						
						lenght_each_object = indexLenghts[pageID];
					}
				} else {
					// if this is the last object then we get its length from the index file. 
					lenght_each_object = indexLenghts[pageID];
				}

				// get the correct page from the RAM - reduce page number from the page number of start page
				// we store them in an array list that start from 0
				long seekPosition = (long) (pageID) * Const.PAGESIZE;

				// Only if the object is on another page it loads that page.
				if (seekPosition != seekPositionTmp) {

					bb.clear();

					long tmpTime = System.nanoTime();

					// Disk I/O
					fileChannel.position(seekPosition);
					fileChannel.read(bb); // read object data from disk

					// Time Calculation
					elapsedIOTime += (System.nanoTime() - tmpTime) / 1000000000.0;

					seekPositionTmp = seekPosition;
				}

				int relativePosition = (int) (position_each_object - pageID * Const.PAGESIZE);

				byte[] buffData_each_obecjt = new byte[lenght_each_object];

				// Copy part of the byte buffer to another byte array
				bb.position(relativePosition);
				bb.get(buffData_each_obecjt, 0, lenght_each_object);

				// NOW read each object
//				readListOfObject.add(Utils.readObjectWithSerialization(myDataType, buffData_each_obecjt, method));
				
			
				myDataIndex.add(lenght_each_object);
				myDataAsByteArray.add(buffData_each_obecjt);
			}

		
		
		
		
		
			// make the data and index object to return
			mydataAndIndex = new DataAndIndex(myDataAsByteArray, myDataIndex);

		
		
		
		
		
		// End of time calculation
		long endTime = System.nanoTime();
		double elapsedTotalTime = (endTime - startTime) / 1000000000.0;

		logger.info("[ReadTimeJAVA]#" + String.format("%.9f", elapsedIOTime) + "#" + String.format("%.9f", elapsedTotalTime));

		return mydataAndIndex;
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
//	/**
//	 * reads objects as ByteArray to send them over the network as they are in Byte
//	 * 
//	 * @param numberOfObjects
//	 * @param indexFile
//	 * @param fileinput
//	 * @return
//	 * @throws IOException
//	 * @throws ClassNotFoundException
//	 */
//	public static DataAndIndex readByteArrayObjects(int numberOfObjects, String indexFile, String fileinput) throws IOException, ClassNotFoundException {
//		DataAndIndex mydataAndIndex = null;
//
//		// ArrayList<RootData> readListOfObject = new ArrayList<RootData>();
//		ArrayList<byte[]> myDataAsByteArray = new ArrayList<byte[]>();
//		ArrayList<Integer> myDataIndex = new ArrayList<Integer>();
//
//		System.out.println("Now reading objects back!");
//		
//		// @SuppressWarnings("unchecked")
//		IndexData myIndex = Utils.readIndex(indexFile, numberOfObjects);
//
//		long[] indexStarts = myIndex.getStarts();
//		int[] indexLenghts = myIndex.getLenghts();
//
//		System.out.println("Index Size is " +  indexStarts.length);
//		
//		
//		File it = new File(fileinput);
//
//		long startTime = 0;
//		double elapsedIOTime = 0;
//		RandomAccessFile fis = new RandomAccessFile(it, "r");
//		FileChannel inChannel = fis.getChannel();
//
//		// NOW read the objects from memory
//		// START OF Time calculation
//		startTime = System.nanoTime();
//
//		// Read back object sequentially from file with only one time read from file
//
//
//		List<ByteBuffer> bufferCache = new ArrayList<ByteBuffer>();
//
//		long startPostionOfStartOfNObject = indexStarts[0];
//		int startPageNumber = (int) (startPostionOfStartOfNObject / Const.PAGESIZE);
//
//		long startPostionOfLastNObject = indexStarts[numberOfObjects - 1];
//		int maxPageNumber = (int) (startPostionOfLastNObject / Const.PAGESIZE);
//
//		long tmpTime = System.nanoTime();
//
//		for (int i = startPageNumber; i < maxPageNumber + 1; i++) {
//			ByteBuffer bb = ByteBuffer.allocateDirect(Const.PAGESIZE);
//			// this can be very large
//			long seekPosition = (long) (i) * Const.PAGESIZE;
//
//			fis.seek(seekPosition); // seek to the object data position
//			inChannel.read(bb); // read object data from disk
//
//			bufferCache.add(bb);
//		}
//
//		// End of time calculation
//		long endTimeReadingFromDisk = System.nanoTime();
//		elapsedIOTime = (endTimeReadingFromDisk - tmpTime) / 1000000000.0;
//
//		// for (PositionSize psize : listPositionSize) {
//		for (int x = 0; x < indexStarts.length; x++) {
//
//			long position_each_object = indexStarts[x];
//			int lenght_each_object = indexLenghts[x];
//
//			int pageID = (int) (position_each_object / Const.PAGESIZE);
//
//			// get the correct page from the RAM - reduce page number from the page number of start page
//			// we store them in an array list that start from 0
//
//			ByteBuffer buffData = bufferCache.get(pageID - startPageNumber);
//
//			int relativePosition = (int) (position_each_object - pageID * Const.PAGESIZE);
//
//			byte[] buffData_each_obecjt = new byte[lenght_each_object];
//
//			// Copy part of the byte buffer to another byte array
//
//			buffData.position(relativePosition);
//			buffData.get(buffData_each_obecjt, 0, lenght_each_object);
//
//			// NOW read each object
//			// Utils.readObjectWithSerialization(myDataType, buffData_each_obecjt, method);
//
//			myDataIndex.add(lenght_each_object);
//			myDataAsByteArray.add(buffData_each_obecjt);
//
//			// NO DESERIALIZATION Send it as is a byte array
//			// readListOfObject.add(Utils.readObjectWithSerialization(myDataType, buffData_each_obecjt, method));
//		}
//
//		// make the data and index object to return
//		mydataAndIndex = new DataAndIndex(myDataAsByteArray, myDataIndex);
//
//		fis.close();
//
//		// End of time calculation
//		long endTime = System.nanoTime();
//		double elapsedTotalTime = (endTime - startTime) / 1000000000.0;
//
//		logger.info("[ReadTimeJAVA]#" + String.format("%.9f", elapsedIOTime) + "#" + String.format("%.9f", elapsedTotalTime));
//
//		return mydataAndIndex;
//	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	public static int generateRandom(int min, int max) {
		Random rand = new Random();
		int randomNum = rand.nextInt((max - min) + 1) + min;
		return randomNum;
	}

}
