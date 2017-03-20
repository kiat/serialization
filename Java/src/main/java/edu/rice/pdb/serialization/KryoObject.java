package edu.rice.pdb.serialization;

import java.util.LinkedHashMap;

import com.esotericsoftware.kryo.Kryo;

import edu.rice.dmodel.Customer;
import edu.rice.dmodel.Element;
import edu.rice.dmodel.LineItem;
import edu.rice.dmodel.Order;
import edu.rice.dmodel.Part;
import edu.rice.dmodel.Supplier;

public class KryoObject {
	
    public  Kryo getKryo() {
		return kryo;
	}


	private Kryo kryo;

    public KryoObject() {
    	kryo = new Kryo();
    	
    	kryo.register(LinkedHashMap.class);
    	
    	kryo.register(long.class);
    	kryo.register(Long.class);
    	kryo.register(Integer.class);

    	
		kryo.register(Element.class);
		kryo.register(Supplier.class);
		kryo.register(Part.class);
		kryo.register(LineItem.class);
		kryo.register(Order.class);
		kryo.register(Customer.class);
		
	}
}
