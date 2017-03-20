package edu.rice.pdb.serialization;

public class Const {

	
	public final static int NumberOfObjects = 10000;
	public final static int NumberOfObjectsToRead = 1000;
	
	// 1 MB Page Size 
    public final static int PAGESIZE=262144;
    
    public final static int IndexPAGESIZE=800000; // this must be a number divisible by 4. 

	
	public final static int serverPort = 10009;

	// In a cluster of 10 machines, access N objects from each machine's local
	// disk, ship to a central location
	// JSON - creation of vector with the vectotrSize
	// from that JSON file we read randomly "NumberOfRandomReads" values
	public final static int vectorSize = 1000000;
	public final static int NumberOfRandomReads = 10;
	public final static String OutputFileVectorJSON ="output-PDB-Vector-JSON.json";

	// other configurations about the example with data encapsulated -
	// University, classes and students
	public final static int NumberOfClasses = 2500;
	public final static int NumberOfStudentsInEachClass = 200;
	public final static int NumberOfTakenCoursesOfEachStudent = 20;

}
