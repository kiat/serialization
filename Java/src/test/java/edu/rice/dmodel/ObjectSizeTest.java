package edu.rice.dmodel;

import org.apache.log4j.PropertyConfigurator;
import org.junit.Test;

import edu.rice.pdb.serialization.KryoSinglton;

public class ObjectSizeTest {

	public ObjectSizeTest() {
		PropertyConfigurator.configure("log4j.properties");

	}

	@Test
	public void objectSizeTest() {

		Part myPart = new Part(1, "tmpName", "tmpMfgr", "tmpBrand", "tmpType", 1, "tmpContainer", 1.0, "tmpComment");

		for (int i = 1; i < 7; i++) {

			switch (i) {
			case 1:
				byte[] data1 = myPart.javaDefaultSerialization();
				System.out.println("Java Default Size:" + data1.length);
				break;
			case 2:
				byte[] data2 = myPart.jsonSerialization();
				System.out.println("JSON Size:" + data2.length);
				break;
			case 3:
				byte[] data3 = myPart.bsonSerialization();
				System.out.println("BSON Size:" + data3.length);
				break;
			case 4:
				byte[] data4 = myPart.protocolBufferWrite();
				System.out.println("Protocol Buffer Size:" + data4.length);
				break;
			case 5:
				byte[] data5 = myPart.kryoSerialization(KryoSinglton.getInstance().getKryo());
				System.out.println("Kryo Size:" + data5.length);
				break;
			case 6:
				byte[] data6 = myPart.writeByteBuffer();
				System.out.println("HandCoded ByteBuffer Size:" + data6.length);
				break;

			}

		}

	}

	@Test
	public void elementObjectSizeTest() {

		Element my = new Element(1L, 1.0);

		byte[] mydata = my.kryoSerialization(KryoSinglton.getInstance().getKryo());
		System.out.println(mydata.length);
		System.out.println("Size of Long: " + Long.SIZE / 8);
		System.out.println("Size of Double: " + Double.SIZE / 8);

		for (int i = 1; i < 7; i++) {

			switch (i) {
			case 1:
				byte[] data1 = my.javaDefaultSerialization();
				System.out.println("Java Default Size:" + data1.length);
				break;
			case 2:
				byte[] data2 = my.jsonSerialization();
				System.out.println("JSON Size:" + data2.length);
				break;
			case 3:
				byte[] data3 = my.bsonSerialization();
				System.out.println("BSON Size:" + data3.length);
				break;
			case 4:
				byte[] data4 = my.protocolBufferWrite();
				System.out.println("Protocol Buffer Size:" + data4.length);
				break;
			case 5:
				byte[] data5 = my.kryoSerialization(KryoSinglton.getInstance().getKryo());
				System.out.println("Kryo Size:" + data5.length);
				break;
			case 6:
				byte[] data6 = my.writeByteBuffer();
				System.out.println("HandCoded ByteBuffer Size:" + data6.length);
				break;

			}

		}

	}

}