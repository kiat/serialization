package edu.rice.dmodel;
import java.util.List;

import org.apache.log4j.PropertyConfigurator;
import org.junit.Test;

import edu.rice.pdb.read.DataAndIndex;
import edu.rice.pdb.read.SerializationMethod;
import edu.rice.pdb.read.WriteAndRead;
import edu.rice.pdb.serial.networking.SocketClient;


public class ReadDataUnitTests {
	
	@Test
	public void readObjectBytes() {
		PropertyConfigurator.configure("log4j.properties");


		 try {
			DataAndIndex myRawData= WriteAndRead.readByteArrayObjects(1000000, "inputdata/PARTBYTEBUFFER.index", "inputdata/PARTBYTEBUFFER.object");
			
			
			System.out.println("DataSize: "+ myRawData.getData().size());
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	
	
	
	}
	
	
	
	@Test
	public void readObjectTest() {
		PropertyConfigurator.configure("log4j.properties");

		List<RootData> mydata = null;
		SerializationMethod serialMethod=SerializationMethod.KRYO;

		try {
			 mydata = WriteAndRead.readObjects(serialMethod, new Part(), 1000, 10000, "inputdata/PART" + serialMethod + ".index", "inputdata/PART" + serialMethod
						+ ".object", true, false);
						
			
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	
	
	
	}
	
	
	
	
	
	
	

}
