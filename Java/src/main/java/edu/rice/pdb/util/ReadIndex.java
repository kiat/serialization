package edu.rice.pdb.util;

import org.apache.log4j.PropertyConfigurator;

import edu.rice.dmodel.IndexData;
import edu.rice.pdb.serialization.Const;

public class ReadIndex {

	public static void main(String[] args) {
		PropertyConfigurator.configure("log4j.properties");
		int toNumberOfObjects = Integer.parseInt(args[1]);

		IndexData myIndex = Utils.readIndex(args[0], 0);
		System.out.println(myIndex.getStarts().length);
		
		long sum=0;


		long[] indexStarts = myIndex.getStarts();
		int[] indexLenghts = myIndex.getLenghts();
		
		int lenght_each_object;

		for (int x = 0; x < toNumberOfObjects; x++) {
			long position_each_object = indexStarts[x];
			int pageID = (int) (position_each_object / Const.PAGESIZE);

			if (x != indexStarts.length - 1) {
				// if we do not read the last object in the file.  
				int pageIDNext = (int) (indexStarts[x + 1] / Const.PAGESIZE);
				
				if (pageID == pageIDNext) {
				// if this object and next object are on the same page then it is simple :)
					lenght_each_object = (int) (indexStarts[x + 1] - indexStarts[x]);
				} else {
				// if there are not, then we get the length of the last object on this page from the index file. 						
					lenght_each_object = indexLenghts[pageID];
				}
			} else {
				// if this is the last object then we get its length from the index file. 
				lenght_each_object = indexLenghts[pageID];
			}
			
			
			sum+=lenght_each_object;
		}
		
		
	
		
		float result=(float)  sum  / (float) myIndex.getStarts().length;

		System.out.println("[SIZE]#"+args[0] +"#"+ myIndex.getStarts().length);
		System.out.println("[SUM]#"+args[0]+"#" +sum);
		System.out.printf(args[0] +"#  %.3f \n", result);
	}
}
