package edu.rice.pdb.read;

import java.util.ArrayList;

public class DataAndIndex {

	public ArrayList<byte[]> getData() {
		return data;
	}

	public void setData(ArrayList<byte[]> data) {
		this.data = data;
	}

	public ArrayList<Integer> getIndex() {
		return index;
	}

	public void setIndex(ArrayList<Integer> index) {
		this.index = index;
	}

	ArrayList<byte[]>  data;
	ArrayList<Integer>  index;
	
	public DataAndIndex(ArrayList<byte[]>  data, ArrayList<Integer>  index ){
		this.data=data;
		this.index=index;
	}
	
	
}
