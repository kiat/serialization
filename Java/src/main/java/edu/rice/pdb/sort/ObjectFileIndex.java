package edu.rice.pdb.sort;

import edu.rice.dmodel.RootData;

public class ObjectFileIndex implements Comparable<ObjectFileIndex> {
	private RootData myObject;
	private int fileIndex;

	ObjectFileIndex(int fileIndex, RootData myObject) {
		this.myObject = myObject;
		this.fileIndex = fileIndex;

	}

	public RootData getValue() {
		return myObject;
	}

	public void setValue(RootData myObject) {
		this.myObject = myObject;
	}

	public int getFileIndex() {
		return fileIndex;
	}

	public void setFileIndex(int fileIndex) {
		this.fileIndex = fileIndex;
	}

	@Override
	public int compareTo(ObjectFileIndex o) {
		return this.myObject.compareTo(o.myObject);
	}
}
