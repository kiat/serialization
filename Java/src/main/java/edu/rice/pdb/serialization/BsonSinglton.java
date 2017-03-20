package edu.rice.pdb.serialization;

import de.undercouch.bson4jackson.BsonFactory;

public class BsonSinglton {
	
    static class SingletonHolder {
    	static 	final BsonSinglton instance = new BsonSinglton();
    }
    
    
    public static BsonSinglton getInstance() {
		return SingletonHolder.instance;

    }
    
    public  BsonFactory getFactory() {
		return factory;
	}


	private BsonFactory factory;

    private BsonSinglton() {
		factory = new BsonFactory();

	}
	
	
}
