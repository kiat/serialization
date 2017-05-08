package edu.rice.pdb.util;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectOutputStream;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;

import com.esotericsoftware.kryo.Kryo;

import edu.rice.dmodel.Element;
import edu.rice.dmodel.IndexData;
import edu.rice.dmodel.RootData;
import edu.rice.pdb.read.SerializationMethod;
import edu.rice.pdb.serialization.Const;
import edu.rice.pdb.sort.WriteValues;

public class Utils {

	final static Logger logger = Logger.getLogger(Utils.class);

	/**
	 * delete old files for storage and indexing
	 */

	public static void deleteOldFiles(String[] files) {

		for (String fileName : files) {
			// Delete if tempFile exists
			File fileTemp1 = new File(fileName);
			if (fileTemp1.exists()) {
				fileTemp1.delete();
			}
		}

	}

	/*
	 * 
	 */
	public static WriteValues writeObjectsWithIndex(ArrayList<byte[]> objectList, int numberOfCopies, String indexFileName, String outPutFileName) {

		WriteValues results = new WriteValues();

		long ioTime = 0;
		long indexTime = 0;

		ArrayList<Integer> indexLengthsArrayList = new ArrayList<Integer>();
		long[] indexStarts = new long[objectList.size() * numberOfCopies];

		// open a random access file for writing objects to it.
		File fileinput = new File(outPutFileName);
		RandomAccessFile fis;
		try {
			fis = new RandomAccessFile(fileinput, "rw");
			long position = 0;
			int counter = 0;
			int pageCounter = 1;
			int tmpPageCounter = pageCounter;
			int numberOfObjects = 0;

			int objectIndexCounter = 0;
			// write several objects to file
			byte[] buffData = new byte[Const.PAGESIZE]; // this is our page

			// write multiple copies of the same object list if given.
			for (int x = 0; x < numberOfCopies; x++) {
				for (int j = 0; j < objectList.size(); j++) {
					// for (byte[] buf : objectList) {

					if (objectList.get(j) != null) {
						// write in pages

						// If we need to create a new page
						// we write the last object length to the lengths array
						if (position + objectList.get(j).length >= (long) pageCounter * Const.PAGESIZE) {
							position = ((long) pageCounter) * Const.PAGESIZE;

							// write to the file
							// -1 is because we start from 1
							// write the data down and get a new page
							long tmpIOTime = System.nanoTime();

							fis.seek(((long) pageCounter - 1) * Const.PAGESIZE);
							fis.write(buffData); // write the old page
							ioTime += (System.nanoTime() - tmpIOTime);

							buffData = new byte[Const.PAGESIZE]; // get a new page

							results.addNumberOfObjects(numberOfObjects);
							numberOfObjects = 0;
							pageCounter++;

						}

						if (tmpPageCounter != pageCounter) {
							// at the end of each page, we add the length of the last object to the file.
							indexLengthsArrayList.add(objectList.get(j - 1).length);
						}

						tmpPageCounter = pageCounter;

						// save the position and length of the serialized object
						indexStarts[objectIndexCounter] = (long) position;
						objectIndexCounter++;

						int relativePosition = (int) (position - (pageCounter - 1) * Const.PAGESIZE);
						System.arraycopy(objectList.get(j), 0, buffData, relativePosition, objectList.get(j).length);

						// increase the current position in file which will be within the page size.
						position += (long) objectList.get(j).length;

						// add number of objects written for this page
						numberOfObjects++;
					} else {
						System.err.println("writeObjectsWithIndex  Array buf is null ... " + counter);
						logger.error("writeObjectsWithIndex  Array buf is null ... " + counter);
					}

				}

			}

			// index the size of the last object.
			// if it is the last object that has to be written.
			// we write its length to the index file.
			indexLengthsArrayList.add(objectList.get(objectList.size() - 1).length);

			// write WriteValues, how many objects are on each page.
			results.addNumberOfObjects(numberOfObjects);

			// write the rest and close
			fis.seek(((long) pageCounter - 1) * Const.PAGESIZE);
			fis.write(buffData); // write the old page

			fis.close();

			// End of time calculation
			// endTime = System.nanoTime();

			logger.debug("ByteArray File Written and closed ... ");

			long startTimeindexTime = System.nanoTime();

			// convert the array list index to a normal array.
			int[] indexLenghts = new int[indexLengthsArrayList.size()];
			for (int i = 0; i < indexLenghts.length; i++) {
				indexLenghts[i] = indexLengthsArrayList.get(i);
			}

			// write the Index File down to the index file.
			writeIndex(indexStarts, indexLenghts, indexFileName);

			indexTime = System.nanoTime() - startTimeindexTime;

		} catch (FileNotFoundException e) {
			logger.error("writeObjectsWithIndex FileNotFoundException: ", e);
		} catch (IOException e) {
			logger.error("writeObjectsWithIndex IOException: ", e);
		}

		// add time values
		results.setIndexTime(indexTime);
		results.setIoTime(ioTime);

		return results;

	}

	/**
	 * Write date possibility append to file if file already exists.
	 * 
	 * @param objectList
	 * @param outPutFileName
	 * @throws FileNotFoundException
	 * @throws IOException
	 */
	public static synchronized double appendObjects(List<byte[]> objectList, ObjectOutputStream fileIndexStream) throws FileNotFoundException, IOException {
		long startTime = System.nanoTime();

		byte[] buffData = new byte[Const.PAGESIZE]; // this is our page

		int position = 0;
		int pageCounter = 1;

		for (int i = 0; i < objectList.size(); i++) {

			// If we need to create a new page
			if (position + objectList.get(i).length >= Const.PAGESIZE) {
				position = 0;
				buffData = new byte[Const.PAGESIZE]; // get a new page
				pageCounter++;
			}
			System.arraycopy(objectList.get(i), 0, buffData, position, objectList.get(i).length);
			position += objectList.get(i).length;
			fileIndexStream.write(pageCounter);
		}

		fileIndexStream.flush(); // note that we only flush to disk one time
									// which might be a problem for large data.
									// fileIndexStream.close();
		// End of time calculation

		return (System.nanoTime() - startTime);
	}

	// public static synchronized double appendObjects(List<byte[]> objectList, ObjectOutputStream fileIndexStream) throws FileNotFoundException, IOException {
	// long startTime = System.nanoTime();
	//
	// for (byte[] buf : objectList) {
	// fileIndexStream.write(buf);
	// }
	//
	// fileIndexStream.flush(); // note that we only flush to disk one time
	// // which might be a problem for large data.
	// // fileIndexStream.close();
	// // End of time calculation
	//
	// return (System.nanoTime() - startTime );
	// }

	/**
	 * Write date possibility append to file if file already exists.
	 * 
	 * @param objectList
	 * @param outPutFileName
	 * @throws FileNotFoundException
	 * @throws IOException
	 */
	public static synchronized double appendObject(byte[] object, String outPutFileName) throws FileNotFoundException, IOException {
		long startTime = System.nanoTime();

		FileOutputStream indexFileOutput = new FileOutputStream(new File(outPutFileName), true);
		ObjectOutputStream fileIndexStream = new ObjectOutputStream(indexFileOutput);

		fileIndexStream.write(object);

		fileIndexStream.flush(); // note that we only flush to disk one time
									// which might be a problem for large data.
		fileIndexStream.close();
		// End of time calculation
		long endTime = System.nanoTime();

		return (endTime - startTime);

	}

	/*
	 * This method reads the objects based on their serialization and prints them out
	 */
	public static RootData readObjectWithSerialization(RootData myData, byte[] buffData, SerializationMethod method, Kryo kryo) {
		RootData myDeserlizedObject = null;

		switch (method) {
		case JAVADEFAULT:
			myDeserlizedObject = myData.javaDefaultDeserialization(buffData);
			break;
		case JSON:
			myDeserlizedObject = myData.jsonDeserialization(buffData);
			break;
		case BSON:
			myDeserlizedObject = myData.bsonDeSerialization(buffData, myData.getClass());
			break;
		case PROTOCOL:
			myDeserlizedObject = myData.protocolBufferRead(buffData);
			break;
		case KRYO:
			myDeserlizedObject = myData.kryoDeserialization(buffData, myData.getClass(), kryo);
			break;
		case BYTEBUFFER:
			myDeserlizedObject = myData.readByteBuffer(buffData);
			break;
		}

		// Print the objects to logger
		return myDeserlizedObject;
	}

	/**
	 * Writes the index file into disk. it writes two arrays, the lengths are much smaller than starts, then we write first the lengths down to disk.
	 *
	 * @param index
	 * @param indexFileName
	 */

	public static void writeIndex(long[] starts, int[] lengths, String indexFileName) {

		// int size = starts.length;

		int objectCount = 0;
		int lenghtCount = 0;
		long position = 0;

		// byteBuffer.putInt(size);
		// position = position + 4;
		BufferedOutputStream bos = null;
		FileOutputStream fos;
		ByteBuffer byteBufferLengths = ByteBuffer.allocate(4 + 4 + (lengths.length * 4));

		try {
			fos = new FileOutputStream(indexFileName);
			bos = new BufferedOutputStream(fos);
			byte[] data = null;

			// first write total number of Objects into file.
			byteBufferLengths.putInt(starts.length); // 4 bytes

			
			// first write the length to the file.
			byteBufferLengths.putInt(lengths.length); // 4 bytes
			// position = 4;

			// then the whole length array.
			while (lenghtCount < lengths.length) {
				byteBufferLengths.putInt(lengths[lenghtCount]); // 4 bytes
				lenghtCount++;

			}

			// write the page down to disk.
			data = byteBufferLengths.array();
			bos.write(data); // write the data
			// flush BufferedOutputStream
			bos.flush();

			ByteBuffer byteBuffer = ByteBuffer.allocate(Const.IndexPAGESIZE);
			byteBuffer.clear();
			byteBuffer.rewind();
			position = 0; // set the position back to zero.

			// now write the positions.
			while (objectCount < starts.length) {
				position = position + 8; // 8 long

				// check if the total position is large than the actual page.
				if (position >= Const.IndexPAGESIZE) {
					// write the page down to disk.
					data = byteBuffer.array();
					bos.write(data); // write the data
					// flush BufferedOutputStream
					bos.flush();
					byteBuffer.clear();
					byteBuffer.rewind();
					position = 0; // set the position back to zero.
				}

				byteBuffer.putLong(starts[objectCount]); // 8 bytes
				objectCount++;
			}

			// final array conversion and write
			data = byteBuffer.array();

			bos.write(data); // write the data
			// flush and close the BufferedOutputStream
			bos.flush();
			bos.close();

		} catch (IOException e) {
			logger.error(e);
		}
	}

	/**
	 * 
	 * @param indexFileName
	 * @return
	 */
	public static IndexData readIndex(String indexFileName, int maxObjectNumber) {

		int[] lenghts = null;
		long[] starts = null;

		long relativePosition = 0;

		try {
			Path path = Paths.get(indexFileName);
			FileChannel fileChannel = FileChannel.open(path);

			// fileChannel.position(0);
			
			
			ByteBuffer byteBufferSizeOfLengths = ByteBuffer.allocateDirect(4); 
			fileChannel.read(byteBufferSizeOfLengths);
			byteBufferSizeOfLengths.flip();
			
			int totalObjectNumbers = byteBufferSizeOfLengths.getInt(); // 4 bytes
			
			// we read index up to the max number
			if(maxObjectNumber<totalObjectNumbers && maxObjectNumber!=0)
				totalObjectNumbers =maxObjectNumber;
			
			
			ByteBuffer byteBufferSizeOfLengths1 = ByteBuffer.allocateDirect(4); 
			fileChannel.read(byteBufferSizeOfLengths1);
			byteBufferSizeOfLengths1.flip();

			int lengthsSize = byteBufferSizeOfLengths1.getInt(); // 4 bytes

			ByteBuffer byteBufferLenght = ByteBuffer.allocateDirect(lengthsSize * 4); // our page
			fileChannel.read(byteBufferLenght);
			byteBufferLenght.flip();

			lenghts = new int[lengthsSize];

			int lenghtsCount = 0;

			// We read the whole array of lengths
			while (lenghtsCount < lengthsSize) {
				relativePosition = relativePosition + 4; // add 12 bytes, 4 int and 8 long
				int lenght = byteBufferLenght.getInt(); // 4 bytes
				lenghts[lenghtsCount] = lenght;

				lenghtsCount++;
			}

			ByteBuffer byteBuffer = ByteBuffer.allocateDirect(Const.IndexPAGESIZE); // our page

			fileChannel.read(byteBuffer);
			byteBuffer.flip();

			relativePosition = 0;
			starts = new long[totalObjectNumbers];

			int startsCount = 0;
			// We read partially the positions up to the requested number
			while (startsCount < totalObjectNumbers) {

				relativePosition = relativePosition + 8; // 8 long
				// check if the total position is large than the actual page.
				if (relativePosition >= Const.IndexPAGESIZE) {

					relativePosition = 0;
					byteBuffer.clear();
					fileChannel.read(byteBuffer);
					byteBuffer.flip();
				}

				long place = byteBuffer.getLong(); // 8 bytes
				starts[startsCount] = place;

				startsCount++;
			}

			fileChannel.close();

		} catch (IOException e) {

			logger.error("readIndex  ", e);
		}

		return new IndexData(starts, lenghts);
	}

	/**
	 * Read bytes from a File into a byte[].
	 * 
	 * @param file
	 *            The File to read.
	 * @return A byte[] containing the contents of the File.
	 * @throws IOException
	 *             Thrown if the File is too long to read or couldn't be read fully.
	 */
	public static byte[] readBytesFromFile(File file) throws IOException {
		InputStream is = new FileInputStream(file);

		// Get the size of the file
		long length = file.length();

		// Create the byte array to hold the data
		byte[] bytes = new byte[(int) length];

		// Read in the bytes
		int offset = 0;
		int numRead = 0;
		while (offset < bytes.length && (numRead = is.read(bytes, offset, bytes.length - offset)) >= 0) {
			offset += numRead;
		}

		// Close the input stream and return bytes
		is.close();
		return bytes;
	}

	/**
	 * Writes the specified byte[] to the specified File path.
	 * 
	 * @param theFile
	 *            File Object representing the path to write to.
	 * @param bytes
	 *            The byte[] of data to write to the File.
	 * @throws IOException
	 *             Thrown if there is problem creating or writing the File.
	 */
	public static void writeBytesToFile(File theFile, byte[] bytes) {
		BufferedOutputStream bos = null;

		FileOutputStream fos;
		try {
			fos = new FileOutputStream(theFile);

			bos = new BufferedOutputStream(fos);
			bos.write(bytes);
			// flush and close the BufferedOutputStream
			bos.flush();
			bos.close();

		} catch (IOException e) {
			logger.error(e);
		}

	}

	public static ArrayList<RootData> aggergateSparseVector(ArrayList<RootData> list1, ArrayList<RootData> list2, ArrayList<RootData> list3) {
		System.out.println("Starting Vector aggergation!");
		
		ArrayList<RootData> aggregatedData = new ArrayList<RootData>();
		
		Element element1, element2, element3;
		Element tmp= new Element(Long.MIN_VALUE, Double.MIN_VALUE);

		int indexList1=0, indexList2=0, indexList3=0;

//		while (!list1.isEmpty() || !list2.isEmpty() || !list3.isEmpty()) {

		while (indexList1 < list1.size() || indexList2 < list2.size() || indexList3 < list3.size() ) {
		
			if (indexList1 < list1.size()){
				element1 = (Element) list1.get(indexList1);
			}
			else
				element1 = tmp;

			if (indexList2 < list2.size())
			{
				element2 = (Element) list2.get(indexList2);
			}
			else
				element2 = tmp;

			if (indexList3 < list3.size())
			{
				element3 = (Element) list3.get(indexList3);
			}
			else
				element3 = tmp;
			
			
			
			if (element1.getMyInteger() > element2.getMyInteger() && element1.getMyInteger() > element3.getMyInteger()) {
				aggregatedData.add(element1);
				indexList1++;

			} else if (element1.getMyInteger() < element2.getMyInteger() && element2.getMyInteger() > element3.getMyInteger()) {
				aggregatedData.add(element2);
				indexList2++;

			} else if (element2.getMyInteger() < element3.getMyInteger() && element1.getMyInteger() < element3.getMyInteger()) {
				aggregatedData.add(element3);
				indexList3++;

			} else if (element1.getMyInteger() < element2.getMyInteger() && element2.getMyInteger() .equals(element3.getMyInteger())) {
				
				Double tmpDouble = element2.getMyDouble() + element3.getMyDouble();
				Element newTmp = new Element(element2.getMyInteger(), tmpDouble);
				
				aggregatedData.add(newTmp);
				indexList2++;
				indexList3++;
				
		   		
			} else if (element1.getMyInteger().equals(element2.getMyInteger()) && element1.getMyInteger() > element3.getMyInteger()) {
				
				Double tmpDouble = element1.getMyDouble() + element2.getMyDouble();
				Element newTmp = new Element(element1.getMyInteger(), tmpDouble);
				
				aggregatedData.add(newTmp);
				indexList1++;
				indexList2++;

			} else if (element1.getMyInteger() .equals(element3.getMyInteger()) && element1.getMyInteger() > element2.getMyInteger()) {
				
				Double tmpDouble = element1.getMyDouble() + element3.getMyDouble();
				Element newTmp = new Element(element1.getMyInteger(), tmpDouble);
				
				aggregatedData.add(newTmp);
				indexList1++;
				indexList3++;

			} else if (element2.getMyInteger().equals(element3.getMyInteger()) && element2.getMyInteger() > element1.getMyInteger()) {
				
				Double tmpDouble = element3.getMyDouble() + element2.getMyDouble();
				Element newTmp = new Element(element2.getMyInteger(), tmpDouble);
				
				aggregatedData.add(newTmp);

				indexList2++;
				indexList3++;

			} else if (element1.getMyInteger().equals(element2.getMyInteger()) && element1.getMyInteger().equals(element3.getMyInteger())) {
				Double tmpDouble = element1.getMyDouble() + element2.getMyDouble() + element3.getMyDouble();
				Element newTmp = new Element(element1.getMyInteger(), tmpDouble);
				
				aggregatedData.add(newTmp);
				indexList1++;
				indexList2++;
				indexList3++;
	
			}
		}
		
		System.out.println("Size of Aggegated Vector is:" + aggregatedData.size());
		
		return aggregatedData;
	}

}
