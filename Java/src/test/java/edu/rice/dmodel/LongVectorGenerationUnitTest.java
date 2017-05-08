package edu.rice.dmodel;

import java.util.ArrayList;

import org.apache.log4j.PropertyConfigurator;
import org.junit.Test;

import edu.rice.pdb.util.Utils;

public class LongVectorGenerationUnitTest {

	public LongVectorGenerationUnitTest() {
		PropertyConfigurator.configure("log4j.properties");

	}

	@Test
	public void partTest() {

		Element myelement = new Element();

		ArrayList<RootData> myObjectList = myelement.generateRandomObjects(100, 10);

		System.out.println(myObjectList.size());
		System.out.println("#################");
		for (RootData data : myObjectList) {
			Element tmp = (Element) data;

			System.out.println(tmp.getMyInteger());

		}

	}

	@Test
	public void aggergationTest() {

		Element myelement = new Element();

		ArrayList<RootData> listA = myelement.generateRandomObjects(100, 10);
		ArrayList<RootData> listB = myelement.generateRandomObjects(100, 10);
		ArrayList<RootData> listC = myelement.generateRandomObjects(100, 10);

		ArrayList<RootData> listAggergated = Utils.aggergateSparseVector(listA, listB, listC);

		System.out.println("Aggergated List size: " + listAggergated.size());

		for (RootData data : listAggergated) {
			Element tmp = (Element) data;

			System.out.println(tmp.getMyInteger());

		}
	}
}