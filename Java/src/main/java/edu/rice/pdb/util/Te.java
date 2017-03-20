package edu.rice.pdb.util;

import org.apache.log4j.PropertyConfigurator;

public class Te {
	public static void main(String[] args) {
		PropertyConfigurator.configure("log4j.properties");

		// ArrayList<Integer> indexLenghts=new ArrayList<Integer>();
		// ArrayList<Long> indexStarts=new ArrayList<Long>();

		int[] indexLenghts = new int[240000000];
		long[] indexStarts = new long[240000000];

		for (int i = 0; i < 240000000; i++) {
			// indexStarts.add((long) i);
			// indexLenghts.add(i);
			indexStarts[i]=(long)i;
			indexLenghts[i] = i;
		}

		System.out.println("ArrayList created");

		Utils.writeIndex(indexStarts, indexLenghts, "myTest.index");

		System.out.println("done!");

		try {
			Thread.sleep(2000000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

//		 ArrayList<PositionSize> myindex2 = Utils.readIndex("myTest.index", 0, 200000);

		// System.out.println(myindex2.get(111011).getStart());

		// int pageSize=10000000;
		// double mz = 12.5;
		// long size = (long) (4 + mz * pageSize);
		//
		// System.out.println("Size is "+size);
		// long pageNumber = size / pageSize;
		//
		//
		// for (int k = 0; k < pageNumber; k++) {
		//
		//
		// byte[] data;
		//
		// if (k == pageNumber)
		// data = new byte[(int) (size - k * pageSize)];
		// else
		// data = new byte[pageSize];
		//
		//
		// System.out.println(k +" "+data.length);
		// }

	}
}
