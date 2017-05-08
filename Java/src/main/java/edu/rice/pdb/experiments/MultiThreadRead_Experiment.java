package edu.rice.pdb.experiments;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Properties;
import java.util.concurrent.CountDownLatch;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import edu.rice.dmodel.Customer;
import edu.rice.dmodel.IndexData;
import edu.rice.dmodel.LineItem;
import edu.rice.dmodel.Part;
import edu.rice.dmodel.RootData;
import edu.rice.pdb.read.SerializationMethod;
import edu.rice.pdb.serial.garbagecollector.Garbage;
import edu.rice.pdb.serialization.Const;
import edu.rice.pdb.serialization.KryoSinglton;
import edu.rice.pdb.util.Utils;

public class MultiThreadRead_Experiment extends Thread {

	final static Logger logger = Logger.getLogger(MultiThreadRead_Experiment.class);

	SerializationMethod method;
	RootData myDataType;
	int numberOfObjectsToRead;
	int fromObjectNumber;
	String indexFile;
	String fileinput;
	IndexData sizeIndex;
	
	FileChannel fileChannel;
	public static CountDownLatch latch = new CountDownLatch(4);

//	ArrayList<ArrayList<RootData>> data = new ArrayList<ArrayList<RootData>>();

	private static int numberOfThreads = 4;

	public MultiThreadRead_Experiment(IndexData myIndex, SerializationMethod method, RootData myDataType, int numberOfObjectsToRead, int fromObjectNumber, String fileinput) {
		this.method = method;
		this.myDataType = myDataType;
		this.numberOfObjectsToRead = numberOfObjectsToRead;
		this.fromObjectNumber = fromObjectNumber;
		this.fileinput = fileinput;
		sizeIndex=myIndex;

		Path path = Paths.get(fileinput);
		try {
			this.fileChannel = FileChannel.open(path);
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	public static void main(String[] args) {

		Properties prop = new Properties();

		PropertyConfigurator.configure(prop);
		prop.setProperty("log4j.appender.WORKLOG.File", "log4j.properties");

		 PropertyConfigurator.configure("log4j.properties");

		// Before any experiments create a hot gc
		// Create a hot garbage collector
		 Garbage.activateGarbageCollector();
		 //
		 System.out.println("Activating Java garbage collector!");
		 System.gc();
		 System.out.println("Java garbage collector is made hot!");

		try {

			int numberOfObjectsToRead = Integer.parseInt(args[0]);
			int serial = Integer.parseInt(args[1]);
			int dataType = Integer.parseInt(args[2]);

			SerializationMethod method = null;

			RootData myDataType = null;

			String fileinput = "";
			String indexFile = "";

			switch (serial) {
			case 1:
				System.out.println("Running with  SerializationMethod.JAVADEFAULT");
				method = SerializationMethod.JAVADEFAULT;
				break;

			case 2:
				System.out.println("Running with  SerializationMethod.JSON");
				method = SerializationMethod.JSON;
				break;

			case 3:
				System.out.println("Running with  SerializationMethod.BSON");
				method = SerializationMethod.BSON;
				break;

			case 4:
				System.out.println("Running with  SerializationMethod.PROTOCOL");
				method = SerializationMethod.PROTOCOL;
				break;

			case 5:
				System.out.println("Running with  SerializationMethod.KRYO");
				method = SerializationMethod.KRYO;
				break;

			case 6:
				System.out.println("Running with  SerializationMethod.ByteBuffer");
				method = SerializationMethod.BYTEBUFFER;
				break;
			}

			// Write them to file with different serialization methods.
			switch (dataType) {
			case 1:
				myDataType = new Part();

				fileinput = "inputdata/PART" + method + ".object";
				indexFile = "inputdata/PART" + method + ".index";
				break;
			case 2:
				myDataType = new LineItem();

				fileinput = "inputdata/LINEITEM" + method + ".object";
				indexFile = "inputdata/LINEITEM" + method + ".index";

				break;

			case 3:
				myDataType = new Customer();

				fileinput = "inputdata/CUSTOMER" + method + ".object";
				indexFile = "inputdata/CUSTOMER" + method + ".index";
				break;

			}


			IndexData[] myIndexes=new IndexData[4];
			
			for (int i = 0; i < 4; i++) {
				
				// We read the index to the size of 6 times number of objects
				myIndexes[i] = Utils.readIndex(indexFile, 6*numberOfObjectsToRead);
			}
			
			


			long startTime = 0;
			// NOW read the objects from memory
			// START OF Time calculation
			startTime = System.nanoTime();

			
			
			for (int i = 0; i < 4; i++) {
				
			  new MultiThreadRead_Experiment(myIndexes[i], method, myDataType, numberOfObjectsToRead, i * numberOfObjectsToRead, fileinput).run();
			}
			
			latch.await(); // Wait for countdown

			
			// End of time calculation
			long endTime = System.nanoTime();
			double elapsedTotalTime = (endTime - startTime) / 1000000000.0;

			logger.info("[ReadTimeJAVAMultiThreadToTal]#" + method + "#" + numberOfObjectsToRead * numberOfThreads + "#" + myDataType.getClass().getSimpleName() + "#" + String.format("%.9f", elapsedTotalTime));
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	public void run() {

		try {
			readObjects(method, myDataType, numberOfObjectsToRead, fromObjectNumber);

		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}

	}

	/**
	 * This method reads objects based on serialization and data type
	 * 
	 */
	public void  readObjects(SerializationMethod method, RootData myDataType, int numberOfObjectsToRead, int fromObjectNumber) throws IOException,
			ClassNotFoundException {
		ByteBuffer bb = ByteBuffer.allocateDirect(Const.PAGESIZE);

		System.out.println("Reading " + numberOfObjectsToRead + " Objects starting from " + fromObjectNumber);

		// we collect data in this arrayList but we do not return it. 
		ArrayList<RootData> readListOfObject = new ArrayList<RootData>();
		
		
		logger.debug("Now reading objects back!");

		long[] indexStarts = sizeIndex.getStarts();
		int[] indexLenghts = sizeIndex.getLenghts();

		long startTime = 0;
		double elapsedIOTime = 0;

		// NOW read the objects from memory
		// START OF Time calculation
		startTime = System.nanoTime();

		// Read back object sequentially from file with only one time read from
		// file
		long seekPositionTmp = Long.MAX_VALUE;

		int lenght_each_object;

		for (int x = fromObjectNumber; x < numberOfObjectsToRead + fromObjectNumber; x++) {
			long position_each_object = indexStarts[x];
			int pageID = (int) (position_each_object / Const.PAGESIZE);

			if (x != indexStarts.length - 1) {
				// if we do not read the last object in the file.
				int pageIDNext = (int) (indexStarts[x + 1] / Const.PAGESIZE);

				if (pageID == pageIDNext) {
					// if this object and next object are on the same page then
					// it is simple :)
					lenght_each_object = (int) (indexStarts[x + 1] - indexStarts[x]);
				} else {
					// if there are not, then we get the length of the last
					// object on this page from the index file.
					lenght_each_object = indexLenghts[pageID];
				}
			} else {
				// if this is the last object then we get its length from the
				// index file.
				lenght_each_object = indexLenghts[pageID];
			}

			// get the correct page from the RAM - reduce page number from the
			// page number of start page
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

		MultiThreadRead_Experiment.latch.countDown();
		fileChannel.close();

		// End of time calculation
		long endTime = System.nanoTime();
		double elapsedTotalTime = (endTime - startTime) / 1000000000.0;

		logger.info("[ReadTimeJAVAMultiThread]#" + method + "#" + myDataType.getClass().getSimpleName() + "#" + String.format("%.9f", elapsedIOTime) + "#"+ String.format("%.9f", elapsedTotalTime));
		

		
	}

}
