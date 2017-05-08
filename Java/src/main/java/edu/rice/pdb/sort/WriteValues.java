package edu.rice.pdb.sort;

import java.util.ArrayList;

public class WriteValues {

	// Number of pages can be very long, longer than MAX_Integer.  
	
	
//	private HashMap<Integer, Integer> numberOfObjectsInEachPage= new HashMap<Integer, Integer>();
	
	private ArrayList<Integer> numberOfObjectsInEachPage= new ArrayList<Integer>();


	private double indexTime;
	private double ioTime;
	
	
	public ArrayList<Integer> getNumberOfObjectsInEachPage() {
		return numberOfObjectsInEachPage;
	}
	
	public void setNumberOfObjectsInEachPage(ArrayList<Integer>  numberOfObjectsInEachPage) {
		this.numberOfObjectsInEachPage = numberOfObjectsInEachPage;
	}

	public double getIoTime() {
		return ioTime;
	}
	public void setIoTime(double ioTime) {
		this.ioTime = ioTime;
	}
	
    public int getNumberOfPages(){
    	return this.numberOfObjectsInEachPage.size();
    }
	
	
	public void addNumberOfObjects(int number){
		this.numberOfObjectsInEachPage.add(number);
	}
	public double getIndexTime() {
		return indexTime;
	}
	public void setIndexTime(double indexTime) {
		this.indexTime = indexTime;
	}
	

}
