package edu.rice.pdb.experiments;

import java.io.FileNotFoundException;
import java.io.IOException;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import edu.rice.dmodel.Customer;
import edu.rice.dmodel.LineItem;
import edu.rice.dmodel.Part;
import edu.rice.pdb.read.SerializationMethod;
import edu.rice.pdb.read.WriteAndRead;
import edu.rice.pdb.serial.garbagecollector.Garbage;

/**
 * 
 * @author Kia
 * This class runs the experiments to read N object from disk by reading objects in a sequence or in randomaccess. 
 */

public class Experiment_1_4_AccessDataFromLocalDisk {

	static int numberOfObjectsTobeReadFromDisk;

	// 1) Randomly access N objects from across a local disk
	// 4) Access N objects sequentially from a local disk

	final static Logger logger = Logger.getLogger(Experiment_1_4_AccessDataFromLocalDisk.class);

	public static void main(String[] args) throws IOException, ClassNotFoundException {

		
		
		if (args.length < 4)
			System.out.println("provide an int for number of objects to be read from disk.");
		
		numberOfObjectsTobeReadFromDisk = Integer.parseInt(args[0]);
		int dataType = Integer.parseInt(args[1]);
		boolean seq  = Boolean.parseBoolean(args[2]);
		int serial = Integer.parseInt(args[3]);

		PropertyConfigurator.configure("log4j.properties");
		

		
		// Before any experiments create a hot gc
		// Create a hot garbage collector
		Garbage.activateGarbageCollector();

		System.out.println("Activating Java garbage collector!");
		
		System.gc();
		
		System.out.println("Java garbage collector is made hot!");

		System.out.println("Starting with Experiments...");


		switch (serial) {
		case 1:
			System.out.println("Running with  SerializationMethod.JAVADEFAULT");
			runExperiments(SerializationMethod.JAVADEFAULT,dataType,  seq);
			break;
		case 2:
			System.out.println("Running with  SerializationMethod.JSON");
			runExperiments(SerializationMethod.JSON,dataType,  seq);
			break;
		case 3:
			System.out.println("Running with  SerializationMethod.BSON");
			runExperiments(SerializationMethod.BSON,dataType,  seq);
			break;
		case 4:
			System.out.println("Running with  SerializationMethod.PROTOCOL");
			runExperiments(SerializationMethod.PROTOCOL,dataType,  seq);
			break;
		case 5:
			System.out.println("Running with  SerializationMethod.KRYO");
			runExperiments(SerializationMethod.KRYO,dataType,  seq);
			break;
		case 6:
			System.out.println("Running with  SerializationMethod.ByteBuffer");
			runExperiments(SerializationMethod.BYTEBUFFER,dataType,  seq);
			break;
			}
			
			
	}



	public static void runExperiments(SerializationMethod method, 	int dataType,  boolean seq) throws FileNotFoundException, IOException, ClassNotFoundException {
		
		System.out.println("Number of objects to be read from disk is: "+numberOfObjectsTobeReadFromDisk);

		
		// Write them to file with different serialization methods.
		switch (dataType) {
		case 1:
			// Part
			// Calculate the time only before and after reading objects from file read not sequentially
			WriteAndRead.readObjects(method, new Part(), numberOfObjectsTobeReadFromDisk,  numberOfObjectsTobeReadFromDisk, "inputdata/PART" + method + ".index", "inputdata/PART" + method + ".object", seq, true);

			break;
		case 2:
			
			// LineItem
			WriteAndRead.readObjects(method, new LineItem(), numberOfObjectsTobeReadFromDisk, numberOfObjectsTobeReadFromDisk,  "inputdata/LINEITEM" + method + ".index", "inputdata/LINEITEM" + method + ".object", seq,  true);
	
			break;

		case 3:
			
			// Customer
			WriteAndRead.readObjects(method, new Customer(), numberOfObjectsTobeReadFromDisk,  numberOfObjectsTobeReadFromDisk, "inputdata/CUSTOMER" + method + ".index", "inputdata/CUSTOMER" + method + ".object", seq, true);
			break;

		}
		
	}
}
