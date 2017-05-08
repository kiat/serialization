package edu.rice.pdb.read;

import java.io.FileNotFoundException;
import java.io.IOException;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import edu.rice.dmodel.Customer;
import edu.rice.dmodel.Element;
import edu.rice.dmodel.LineItem;
import edu.rice.dmodel.Part;
import edu.rice.pdb.serialization.Const;
import edu.rice.pdb.util.Utils;

public class MainWriteAndReadNObject {

	private final static String objectDIR = "inputdata/";
	private final static String indexDIR = "inputdata/";
	
	private final static String objectExt = ".object";
	private final static String indexExt = ".index";


	final static Logger logger = Logger.getLogger(MainWriteAndReadNObject.class);

	public static void main(String[] args) throws IOException, ClassNotFoundException {
		PropertyConfigurator.configure("log4j.properties");
		Utils.deleteOldFiles(new String[] { "*.obj", "*.index" });

		runExperiments(SerializationMethod.JAVADEFAULT);
		runExperiments(SerializationMethod.JSON);
		runExperiments(SerializationMethod.BSON);
		runExperiments(SerializationMethod.PROTOCOL);
		runExperiments(SerializationMethod.KRYO);
		runExperiments(SerializationMethod.BYTEBUFFER);

	}

	public static void runExperiments(SerializationMethod serialMethod) throws FileNotFoundException, IOException, ClassNotFoundException {
		
		
		// Generate Objects and write to Files. 
		WriteAndRead.writeObjects(serialMethod, SerializedData.ELEMENT, Const.NumberOfObjects,  indexDIR+"ELEMENT"+serialMethod+indexExt , objectDIR+"ELEMENT"+serialMethod+objectExt);
		WriteAndRead.writeObjects(serialMethod, SerializedData.PART, Const.NumberOfObjects, indexDIR+"PART"+serialMethod+indexExt , objectDIR+"PART"+serialMethod+objectExt);
		WriteAndRead.writeObjects(serialMethod, SerializedData.LINEITEM, Const.NumberOfObjects, indexDIR+"LINEITEM"+serialMethod+indexExt , objectDIR+"LINEITEM"+serialMethod+objectExt);
		WriteAndRead.writeObjects(serialMethod, SerializedData.CUSTOMER, Const.NumberOfObjects, indexDIR+"CUSTOMER"+serialMethod+indexExt , objectDIR+"CUSTOMER"+serialMethod+objectExt);

		
		
		// Reading Data back 

		// Element
		WriteAndRead.readObjects(serialMethod, new Element(), Const.NumberOfObjectsToRead, Const.NumberOfObjects, indexDIR+"ELEMENT"+serialMethod+indexExt , objectDIR+"ELEMENT"+serialMethod+objectExt, false, true);
		WriteAndRead.readObjects(serialMethod, new Element(), Const.NumberOfObjectsToRead, Const.NumberOfObjects,indexDIR+"ELEMENT"+serialMethod+indexExt  , objectDIR+"ELEMENT"+serialMethod+objectExt, true, true);

		// Part
		// Calculate the time only before and after reading objects from file read not sequentially
		WriteAndRead.readObjects(serialMethod, new Part(), Const.NumberOfObjectsToRead, Const.NumberOfObjects,indexDIR+"PART"+serialMethod+indexExt, objectDIR+"PART"+serialMethod+objectExt, false, true);
		// Read sequentially
		WriteAndRead.readObjects(serialMethod, new Part(), Const.NumberOfObjectsToRead, Const.NumberOfObjects,indexDIR+"PART"+serialMethod+indexExt, objectDIR+"PART"+serialMethod+objectExt, true, true);
		
		// LineItem
		// Calculate the time only before and after reading objects from file read not sequentially
		WriteAndRead.readObjects(serialMethod, new LineItem(), Const.NumberOfObjectsToRead,Const.NumberOfObjects, indexDIR+"LINEITEM"+serialMethod+indexExt, objectDIR+"LINEITEM"+serialMethod+objectExt, false, true);
		WriteAndRead.readObjects(serialMethod, new LineItem(), Const.NumberOfObjectsToRead, Const.NumberOfObjects,indexDIR+"LINEITEM"+serialMethod+indexExt, objectDIR+"LINEITEM"+serialMethod+objectExt, true, true);
		
		// Customer
		WriteAndRead.readObjects(serialMethod, new Customer(), Const.NumberOfObjectsToRead, Const.NumberOfObjects,indexDIR+"CUSTOMER"+serialMethod+indexExt,  objectDIR+"CUSTOMER"+serialMethod+objectExt, false, true);
		WriteAndRead.readObjects(serialMethod, new Customer(), Const.NumberOfObjectsToRead, Const.NumberOfObjects,indexDIR+"CUSTOMER"+serialMethod+indexExt,  objectDIR+"CUSTOMER"+serialMethod+objectExt, true, true);
	
	}
}
