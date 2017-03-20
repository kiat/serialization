package edu.rice.dmodel;


public class IndexData {

	private long[] starts;
	private int[] lenghts;

	public IndexData() {
	}

	public IndexData(long[] starts, int[] lenghts) {
		this.starts = starts;
		this.lenghts = lenghts;
	}

	public long[] getStarts() {
		return starts;
	}

	public void setStarts(long[] starts) {
		this.starts = starts;
	}

	public int[] getLenghts() {
		return lenghts;
	}

	public void setLenghts(int[] lenghts) {
		this.lenghts = lenghts;
	}

}
