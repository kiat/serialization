package edu.rice.pdb.util;

import org.apache.log4j.PropertyConfigurator;

import edu.rice.dmodel.IndexData;

public class WriteReadIndex {

	public static void main(String[] args) {
		PropertyConfigurator.configure("log4j.properties");

		int numberOfObjects = 100;

		int[] indexLenghts = new int[12];
		long[] indexStarts = new long[numberOfObjects];

		System.out.println("generating an index with size:" + numberOfObjects);

		for (int i = 0; i < numberOfObjects; i++) {
			indexStarts[i] = (long) i;
		}

		for (int i = 0; i < 12; i++) {
			indexLenghts[i] = i;
		}

		System.out.println("write index file down to disk. ");

		Utils.writeIndex(indexStarts, indexLenghts, "./inputdata/indexTest.index");


		IndexData sizeIndexNew = Utils.readIndex("./inputdata/indexTest.index", 0);
		System.out.println("sizeIndexNew is : " + sizeIndexNew.getStarts().length);
		System.out.println("sizeIndexNew is : " + sizeIndexNew.getLenghts().length);
		for (int i = 0; i < indexStarts.length; i++) {
			System.out.println("indexStarts : " + indexStarts[i]);
		}

		for (int i = 0; i < indexLenghts.length; i++) {
			System.out.println("indexLenghts : " + indexStarts[i]);
		}


	}
}
