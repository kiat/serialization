package edu.rice.pdb.sort;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.PriorityQueue;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import edu.rice.dmodel.Customer;
import edu.rice.dmodel.Element;
import edu.rice.dmodel.IndexData;
import edu.rice.dmodel.LineItem;
import edu.rice.dmodel.Part;
import edu.rice.dmodel.RootData;
import edu.rice.pdb.read.SerializationMethod;
import edu.rice.pdb.read.SerializedData;
import edu.rice.pdb.serial.garbagecollector.Garbage;
import edu.rice.pdb.serialization.Const;
import edu.rice.pdb.serialization.KryoSinglton;
import edu.rice.pdb.util.Utils;

/**
 * 
 * @author Kia
 *
 *         This class implements external sort with different serialization methods.
 */

public class ExternalSort {
	final static Logger logger = Logger.getLogger(ExternalSort.class);

	static int numberOfFiles;
	static int numberOfObjectsInEachFiles;


	/**
	 * @param args
	 * @throws IOException
	 * @throws ClassNotFoundException
	 */
	public static void main(String[] args) throws IOException, ClassNotFoundException {

		numberOfFiles = Integer.parseInt(args[0]);
		numberOfObjectsInEachFiles = Integer.parseInt(args[1]);
		int myDataType = Integer.parseInt(args[2]);
		int serializationType = Integer.parseInt(args[3]);

		System.out.println("Number of files: " + numberOfFiles);
		System.out.println("Number of Objects in each file: " + numberOfObjectsInEachFiles);

		PropertyConfigurator.configure("log4j.properties");

		// Before any experiments create a hot gc
		// Create a hot garbage collector
		Garbage.activateGarbageCollector();

		SerializationMethod method = null;
		SerializedData dataType = null;

		// Write them to file with different serialization methods.
		switch (myDataType) {
		case 1:
			dataType = SerializedData.ELEMENT;
			break;
		case 2:
			dataType = SerializedData.PART;
			break;
		case 3:
			dataType = SerializedData.LINEITEM;
			break;
		case 4:
			dataType = SerializedData.CUSTOMER;
			break;
		}

		System.out.println("Data Type: " + dataType);

		switch (serializationType) {
		case 1:
			method = SerializationMethod.JSON;
			break;
		case 2:
			method = SerializationMethod.JAVADEFAULT;
			break;
		case 3:
			method = SerializationMethod.BSON;
			break;
		case 4:
			method = SerializationMethod.PROTOCOL;
			break;
		case 5:
			method = SerializationMethod.KRYO;
			break;
		case 6:
			method = SerializationMethod.BYTEBUFFER;
			break;
		}

		System.out.println("Serialization Type: " + method);
		System.out.println("\n\n");

		// finally run the experiment
		runTheExternalSort(method, dataType);

	}

	/*
	 * 	
	 * 
	 */
	private static void runTheExternalSort(SerializationMethod serializationMethod, SerializedData dataType) throws IOException, FileNotFoundException, ClassNotFoundException {
		System.out.println("Start Reading part of the file into RAM");

		// Open The object file
		File itRaw = new File("inputdata/" + dataType + serializationMethod + ".object");
		RandomAccessFile fisRaw = new RandomAccessFile(itRaw, "r");

		ArrayList<WriteValues> writeValues = new ArrayList<WriteValues>();
		ByteBuffer bb = ByteBuffer.allocateDirect(Const.PAGESIZE);

		// Get a Runtime object
		Runtime runTime = Runtime.getRuntime();

		IndexData myIndex = Utils.readIndex("inputdata/" + dataType + serializationMethod + ".index", 0);
		System.out.println("Read Index file number into RAM");

		long[] my_indexStarts = myIndex.getStarts();
		int[] my_indexLenghts = myIndex.getLenghts();
		// activate the garbage collector
		runTime.gc();

		long ioTime = 0;
		long indexTime =0;
		
		long sortTime = 0;


		// start of time calculation
		long startTime = System.nanoTime();
		
		for (int i = 0; i < numberOfFiles; i++) {

			// activate the garbage collector
			runTime.gc();

			System.out.println("Reading for file number " + i);

			ArrayList<RootData> m_list_read_from_file = new ArrayList<RootData>();

			long seekPositionTmp = Long.MAX_VALUE;

			// for (PositionSize psize : sizeIndex) {
			for (int x = 0; x < numberOfObjectsInEachFiles; x++) {

				long position_each_object = my_indexStarts[x];

				if(position_each_object<0)
					System.out.println("position_each_object " + position_each_object);
				
				int pageID = (int) (position_each_object / Const.PAGESIZE);

				// int lenght_each_object = indexLenghts[x];
				int lenght_each_object;
				if (x != my_indexStarts.length - 1) {
					// if we do not read the last object in the file.
					int pageIDNext = (int) (my_indexStarts[x + 1] / Const.PAGESIZE);

					if (pageID == pageIDNext) {
						// if this object and next object are on the same page then it is simple :)
						lenght_each_object = (int) (my_indexStarts[x + 1] - my_indexStarts[x]);
					} else {
						// if there are not, then we get the length of the last object on this page from the index file.
						lenght_each_object = my_indexLenghts[pageID];
					}
				} else {
					// if this is the last object then we get its length from the index file.
					lenght_each_object = my_indexLenghts[pageID];
				}

				// get the correct page from the RAM - reduce page number from the page number of start page
				// we store them in an array list that start from 0
				long seekPosition = (long) (pageID) * Const.PAGESIZE;

				// Only if the object is on another page it loads that page.
				if (seekPosition != seekPositionTmp) {

					bb.clear();

					long tmpTime = System.nanoTime();

					// Disk I/O
					fisRaw.getChannel().position(seekPosition);
					fisRaw.getChannel().read(bb); // read object data from disk

					// Time Calculation
					ioTime += System.nanoTime() - tmpTime;

					seekPositionTmp = seekPosition;
				}

				int relativePosition = (int) (position_each_object - pageID * Const.PAGESIZE);

				byte[] buffData_each_obecjt = new byte[lenght_each_object];

				// Copy part of the byte buffer to another byte array
				bb.position(relativePosition);
				bb.get(buffData_each_obecjt, 0, lenght_each_object);

				RootData tmp = null;

				// NOW read each object
				switch (dataType) {
				case PART:
					tmp = (Part) Utils.readObjectWithSerialization(new Part(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
					break;
				case LINEITEM:
					tmp = (LineItem) Utils.readObjectWithSerialization(new LineItem(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
					break;
				case CUSTOMER:
					tmp = (Customer) Utils.readObjectWithSerialization(new Customer(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
					break;
				case ELEMENT:
					tmp = (Element) Utils.readObjectWithSerialization(new Element(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
					break;
				default:
					break;
				}

				if (tmp != null) {
					m_list_read_from_file.add(tmp);
					// System.out.println(i+" "+tmp);
				} else
					System.err.println("Can not read the object from buffer");

			}

			// Sort the data
			long tmpTime = System.nanoTime();
			Collections.sort(m_list_read_from_file);
			// Sort Time Calculation
			sortTime += System.nanoTime() - tmpTime;
			
			
			// write the sorted list back to files.
			WriteValues tmpResults = writeToFiles(i, m_list_read_from_file, "tmp/sorted-" + dataType + serializationMethod, serializationMethod);
			
			// 	add IO time for writing data into disk
			tmpResults.setIoTime(tmpResults.getIoTime() + ioTime); // add IO time

			// set index time. 
			tmpResults.setIndexTime(tmpResults.getIndexTime());

			writeValues.add(tmpResults);
		}
		
		

		// add IO time for writing files
		for (WriteValues writeValues2 : writeValues) {
			ioTime += writeValues2.getIoTime();
			indexTime += writeValues2.getIndexTime();
		}
		
		fisRaw.close();


		
		
		
		
		
		// activate the garbage collector
		runTime.gc();
		
		System.out.println("First stage done! ");

		// ///////////////////////////////////////////////////
		// //
		// External Sort - Second stage //
		// //
		// //////////////////////////////////////////////////

		System.out.println("Second Stage ... ");

		long endTime = System.nanoTime();
		double elapsedSeconds = ((endTime - startTime) - indexTime) / 1000000000.0;
		double elapsedIOSeconds = ioTime / 1000000000.0 ;
		double elapsedIndexTime = indexTime / 1000000000.0;
		double elapsedSortTime = sortTime / 1000000000.0;

		System.out.println("[ReadTimeJAVA]#ExternalSortFirstStep#" + dataType + "#" + serializationMethod + "#" + String.format("%.9f", elapsedIOSeconds) + "#" + String.format("%.9f", elapsedIndexTime)+"#" +   String.format("%.9f", elapsedSortTime)  + "#" +   String.format("%.9f", elapsedSeconds));
		
		
		
		// Reading parts of each file, sort and write them back to a single file.
		// First, we need to read all of the index files to be able to iterate over the objects.
		// ArrayList<ArrayList<PositionSize>> indexArray = new ArrayList<ArrayList<PositionSize>>();

		ArrayList<long[]> indexStarts = new ArrayList<long[]>();
		ArrayList<int[]> indexLenghts = new ArrayList<int[]>();

		// read all of the index files into RAM
		for (int i = 0; i < numberOfFiles; i++) {
			// Read each index file and insert them at the specific position.
			// 0 means read the entire index file.
			IndexData mynewIndex = Utils.readIndex("tmp/sorted-" + dataType + serializationMethod + i + ".index", 0);
			indexStarts.add(i, mynewIndex.getStarts());
			indexLenghts.add(i, mynewIndex.getLenghts());
		}

		System.out.println("Done with reading index files ... ");

		// open all of data files and keep them open to read partially.
		ArrayList<RandomAccessFile> fileArray = new ArrayList<RandomAccessFile>();

		for (int x = 0; x < numberOfFiles; x++) {
			// now read the objects from file one by one - reading
			// sequentially
			File it = new File("tmp/sorted-" + dataType + serializationMethod + x + ".object");
			fileArray.add(x, new RandomAccessFile(it, "r"));
		}

		int[] pageCounter = new int[numberOfFiles];
		// set all positions to the beginning of files - zero.
		for (int y = 0; y < numberOfFiles; y++) {
			pageCounter[y] = 0;
		}

		PriorityQueue<ObjectFileIndex> queue = new PriorityQueue<ObjectFileIndex>();

		int[] maxObjectNumber = new int[numberOfFiles];
		int[] minObjectNumber = new int[numberOfFiles];
		long[] fileObjectCounter = new long[numberOfFiles];

		ByteBuffer myBuffer = ByteBuffer.allocateDirect(Const.PAGESIZE); // this is our page

		// reading objects from the first pages and adding them to a priority queue
		for (int i = 0; i < numberOfFiles; i++) {
			maxObjectNumber[i] = writeValues.get(i).getNumberOfObjectsInEachPage().get(0);

			// min object number is used in the next stage to know how many objects are read before.
			minObjectNumber[i] = maxObjectNumber[i];

			fileObjectCounter[i] = maxObjectNumber[i];

			myBuffer.clear();

			// IO time calculation
			long tmpIO = System.nanoTime();
			// read the first page from each file
			fileArray.get(i).seek(0); // seek to the position in file.
			fileArray.get(i).getChannel().read(myBuffer); // read object data from disk.
			ioTime += System.nanoTime() - tmpIO;

			int pageID = 0;

			for (int j = 0; j < maxObjectNumber[i]; j++) {

				long position_each_object = indexStarts.get(i)[j];

				// int lenght_each_object = indexLenghts.get(i)[j];

				int lenght_each_object;

				if (j != maxObjectNumber[i] - 1) {
					lenght_each_object = (int) (indexStarts.get(i)[j + 1] - indexStarts.get(i)[j]);
				} else {
					// if this is the last object then we get its length from the index file.
					lenght_each_object = indexLenghts.get(i)[pageID];
				}

				int relativePosition = (int) (position_each_object - pageID * Const.PAGESIZE);

				byte[] buffData_each_obecjt = new byte[lenght_each_object];

				// Copy part of the byte buffer to another byte array
				myBuffer.position(relativePosition);
				myBuffer.get(buffData_each_obecjt, 0, lenght_each_object);

				RootData tmpObject = null;
				// NOW read each object
				switch (dataType) {
				case PART:
					tmpObject = Utils.readObjectWithSerialization(new Part(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
					break;
				case LINEITEM:
					tmpObject = Utils.readObjectWithSerialization(new LineItem(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
					break;
				case CUSTOMER:
					tmpObject = Utils.readObjectWithSerialization(new Customer(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
					break;
				case ELEMENT:
					tmpObject = Utils.readObjectWithSerialization(new Element(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
					break;
				default:
					break;
				}

				// add objects to the list if not null
				if (tmpObject != null) {
					ObjectFileIndex tmpData = new ObjectFileIndex(i, tmpObject);
					queue.add(tmpData);
				} else {
					System.err.println("Null Pointer, can not read object");
				}

			} // End of loop over objects of the first page.
		} // End of loop over files

		System.out.println("First page reading is done! ");

		ArrayList<byte[]> dataToWrite = new ArrayList<byte[]>();

		// activate the garbage collector
		runTime.gc();
		
		long pageCount=1;
		// reading next pages
		// ###################################################################

		long objectCounter = 0;

		// open final output file.
		FileOutputStream objectFileOutput = new FileOutputStream(new File("tmp/finalSortedFile-" + dataType), true);
		BufferedOutputStream bout = new BufferedOutputStream(objectFileOutput);
		ObjectOutputStream fileIndexStream = new ObjectOutputStream(bout);

		while (objectCounter < numberOfFiles * numberOfObjectsInEachFiles) {



			ObjectFileIndex tmpFileIndex = queue.remove();
			RootData data = tmpFileIndex.getValue();
			int fileNumber = tmpFileIndex.getFileIndex();

			// reduce the number of objects read from that file.
			fileObjectCounter[fileNumber] = fileObjectCounter[fileNumber] - 1;

			// If needed load more objects from files.
			// if zero load the next page from file and add objects.
			if (fileObjectCounter[fileNumber] == 0 && pageCounter[fileNumber] < writeValues.get(fileNumber).getNumberOfPages() - 1) {
				
				pageCount++;
				
				// add page counter
				pageCounter[fileNumber] = pageCounter[fileNumber] + 1;

				maxObjectNumber[fileNumber] = writeValues.get(fileNumber).getNumberOfObjectsInEachPage().get(pageCounter[fileNumber]);
				fileObjectCounter[fileNumber] = maxObjectNumber[fileNumber];

				myBuffer.clear();

				// IO time calculation
				long tmpIO = System.nanoTime();

				// read next pages from each file
				fileArray.get(fileNumber).seek((long) pageCounter[fileNumber] * Const.PAGESIZE); // seek to the position in file.
				fileArray.get(fileNumber).getChannel().read(myBuffer); // read object data from disk.

				ioTime += System.nanoTime() - tmpIO;

				for (int j = minObjectNumber[fileNumber]; j < minObjectNumber[fileNumber] + maxObjectNumber[fileNumber]; j++) {

					long position_each_object = indexStarts.get(fileNumber)[j];
					// int lenght_each_object = indexLenghts.get(fileNumber)[j];

					int lenght_each_object;

					if (j != minObjectNumber[fileNumber] + maxObjectNumber[fileNumber] - 1) {
						lenght_each_object = (int) (indexStarts.get(fileNumber)[j + 1] - indexStarts.get(fileNumber)[j]);
					} else {
						// if this is the last object then we get its length from the index file.
						lenght_each_object = indexLenghts.get(fileNumber)[pageCounter[fileNumber]];
					}

					int relativePosition = (int) (position_each_object - pageCounter[fileNumber] * Const.PAGESIZE);

					byte[] buffData_each_obecjt = new byte[lenght_each_object];

					// Copy part of the byte buffer to another byte array
					myBuffer.position(relativePosition);
					myBuffer.get(buffData_each_obecjt, 0, lenght_each_object);

					RootData tmpObject = null;
					// NOW read each object
					switch (dataType) {
					case PART:
						tmpObject = Utils.readObjectWithSerialization(new Part(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
						break;
					case LINEITEM:
						tmpObject = Utils.readObjectWithSerialization(new LineItem(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo() );
						break;
					case CUSTOMER:
						tmpObject = Utils.readObjectWithSerialization(new Customer(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
						break;
					case ELEMENT:
						tmpObject = Utils.readObjectWithSerialization(new Element(), buffData_each_obecjt, serializationMethod, KryoSinglton.getInstance().getKryo());
						break;
					default:
						break;
					}

					if (tmpObject != null) {
						ObjectFileIndex tmpData = new ObjectFileIndex(fileNumber, tmpObject);
						queue.add(tmpData);
					} else
						System.err.println("Null Pointer, can not read object");

				}

				minObjectNumber[fileNumber] = minObjectNumber[fileNumber] + maxObjectNumber[fileNumber];

			} // End of big IF for load next pages.

			// now deserialize and write to the final file
			// Do the serialization
			switch (serializationMethod) {
			case JAVADEFAULT:
				dataToWrite.add(data.javaDefaultSerialization());
				break;
			case JSON:
				dataToWrite.add(data.jsonSerialization());
				break;
			case BSON:
				dataToWrite.add(data.bsonSerialization());
				break;
			case PROTOCOL:
				dataToWrite.add(data.protocolBufferWrite());
				break;
			case KRYO:
				dataToWrite.add(data.kryoSerialization(KryoSinglton.getInstance().getKryo()));
				break;
			case BYTEBUFFER:
				dataToWrite.add(data.writeByteBuffer());
				break;
			default:
				break;
			}

			
			// Write only of we collected more than 10000 objects
			if (dataToWrite.size() > 10000) {
				ioTime += Utils.appendObjects(dataToWrite, fileIndexStream);
				dataToWrite = new ArrayList<byte[]>();
			}

			objectCounter++;
			
			if (objectCounter % numberOfObjectsInEachFiles == 0)
				System.out.println("Sorted " + objectCounter + " objects!");
		}

		// write the rest of data to the disk after the foor loop
		ioTime += Utils.appendObjects(dataToWrite, fileIndexStream);

		fileIndexStream.close();

		// End of time calculation
		endTime = System.nanoTime();

		elapsedSeconds = ((endTime - startTime) - indexTime) / 1000000000.0;

		elapsedIOSeconds = ioTime / 1000000000.0 ;

//		System.out.println(indexTime);
		elapsedIndexTime = indexTime / 1000000000.0;

		logger.info("[ReadTimeJAVA]#ExternalSort#" + dataType + "#" + serializationMethod + "#" + String.format("%.9f", elapsedIOSeconds) + "#"+ String.format("%.9f", elapsedIndexTime) + "#" + String.format("%.9f", elapsedSeconds) );
		System.out.println("Sorted " + objectCounter + " " + dataType + " objects with " + serializationMethod);

		
		System.out.println("Page Counts for the second stage " + pageCount);
		
		
		System.out.println("Done!");

		// Close object files.
		for (RandomAccessFile m_file : fileArray) {
			m_file.close();
		}

	}

	

	/**
	 * 
	 * @param i
	 * @param m_list_read_from_file
	 * @param name
	 * @param method
	 */
	static WriteValues writeToFiles(int i, List<RootData> m_list_read_from_file, String name, SerializationMethod method) {
		WriteValues writeValueResults = null;

		// Now write them back one by one
		ArrayList<byte[]> byteObjects = new ArrayList<byte[]>();

		for (RootData object : m_list_read_from_file) {
			RootData tmpPart = object;
			byte[] buff = null;

			switch (method) {
			case JAVADEFAULT:
				buff = tmpPart.javaDefaultSerialization();
				break;
			case JSON:
				buff = tmpPart.jsonSerialization();
				break;
			case BSON:
				buff = tmpPart.bsonSerialization();
				break;
			case PROTOCOL:
				buff = tmpPart.protocolBufferWrite();
				break;
			case KRYO:
				buff = tmpPart.kryoSerialization(KryoSinglton.getInstance().getKryo());
				break;
			case BYTEBUFFER:
				buff = tmpPart.writeByteBuffer();
				break;
			}

			byteObjects.add(buff);
		}

		// Writing back after sorting
		try {
			writeValueResults = Utils.writeObjectsWithIndex(byteObjects, 1, name + i + ".index", name + i + ".object");
		} catch (Exception e) {
			logger.error("Write and Sort Vectors, error in writing objects to files , size of object list: " + byteObjects.size(), e);
		}

		return writeValueResults;

	}

	/**
	 * 
	 * @param file
	 * @param position
	 * @param buffData
	 * @return
	 */
	static ByteBuffer readAPage(RandomAccessFile file, long position, ByteBuffer buffData) {

		try {
			file.seek(position); // seak to the position in file.
			file.getChannel().read(buffData); // read object data from disk.

		} catch (IOException e) {
			logger.error("readAPage: can not read a page from file; ", e);
		}
		return buffData;
	}

}