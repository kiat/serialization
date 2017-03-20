package edu.rice.pdb.serial.garbagecollector;

import java.lang.management.GarbageCollectorMXBean;
import java.lang.management.ManagementFactory;
import java.util.List;

// Run this Test with the following virtual machine arguments 
// -Xmx3g -Xms1g  -verbose:gc
// -Xmx4g -Xms1g   -verbose:gc  -XX:+PrintGCTimeStamps   -XX:+PrintGCDetails   -XX:+UseSerialGC

public class GarbageCollectorTest {

	public static void main(String[] args) {
		// This will print out a list of all of the Garbage Collectors currently
		// loaded in your JVM.
		List<GarbageCollectorMXBean> l = ManagementFactory
				.getGarbageCollectorMXBeans();
		for (GarbageCollectorMXBean b : l) {
			System.out.println(b.getName());
		}

		// Create a hot garbage collector
		Garbage.activateGarbageCollector();
	}

}
