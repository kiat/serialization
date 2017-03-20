package edu.rice.pdb.util;

import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Path;
import java.nio.file.Paths;

public class TestIO2 {

	public static void main(String args[]) {
		try {
//			File it = new File(args[0]);
//			RandomAccessFile fis = new RandomAccessFile(it, "r");
//			FileChannel inChannel = fis.getChannel();

			Path path = Paths.get(args[0]);
			FileChannel inChannel = FileChannel.open(path);
			
			
			long fileSize = Long.parseLong(args[1]);
		
			int pageSize = 256 * 1024; // 262144

			long numPages = fileSize / pageSize;
			int numReads = Integer.parseInt(args[2]);


			long beginTime = System.nanoTime();
			for (int i = 0; i < numReads; i++) {
				ByteBuffer curBuffer = ByteBuffer.allocateDirect(pageSize);

				long startPage = (long) (Math.random() * numPages);
//				fis.seek(startPage*pageSize);
				inChannel.position(startPage*pageSize);
				inChannel.read(curBuffer);

//				curBuffer.clear();
			}
			double elapsedIOTime = (System.nanoTime() - beginTime) / 1000000000.0;
			System.out.println(elapsedIOTime);
			
			inChannel.close();
		} catch (Exception ex) {
			System.out.println(ex);
		}
	}
	
	
	
	
//	public static int generateRandom(int min, int max) {
//		Random rand = new Random();
//		int randomNum = rand.nextInt((max - min) + 1) + min;
//		return randomNum;
//	}
	
	
}