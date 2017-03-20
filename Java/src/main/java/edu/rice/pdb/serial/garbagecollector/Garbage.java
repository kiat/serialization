package edu.rice.pdb.serial.garbagecollector;

import java.util.HashMap;
import java.util.Random;

public class Garbage extends Stuff {

	public static int SIZE = 1000;
	public static int SMALL = 10000;
	public static int MEDIUM = 100000;
	public static int LARGE = 1000000;

	private Integer[] myInt;

	public Integer[] getMyInt() {
		return myInt;
	}

	public void setMyInt(Integer[] myInt) {
		this.myInt = myInt;
	}

	public Garbage() {

	}

	public Garbage(int number) {
		myInt = new Integer[number];
	}

	public static void activateGarbageCollector() {

		HashMap<Integer, Stuff> myMap = new HashMap<Integer, Stuff>();

		Random myRand = new Random();

		for (int i = 0; i < SIZE * 1000; i++) {

			// set SIZE so as to regulate the size of the RAM used
			Integer whichSlot = myRand.nextInt(SIZE);

			if (i % 1000 == 0) {
				myMap.put(whichSlot, new Garbage(LARGE));
			} else if (i % 10 == 0) {
				myMap.put(whichSlot, new Garbage(MEDIUM));
			} else {
				myMap.put(whichSlot, new Garbage(SMALL));
			}
		}

	}

}