package edu.rice.pdb.util;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

import edu.rice.pdb.serialization.Const;
 
public class ReadFilesInPages 
{
    public static void main(String[] args) throws IOException 
    {
        RandomAccessFile aFile = new RandomAccessFile  ("ObjectsBSONCustomer.obj", "r");
        FileChannel inChannel = aFile.getChannel();
        ByteBuffer buffer = ByteBuffer.allocate(Const.PAGESIZE);
        
        for (int x = 0; x < 100000; x++) {
        	inChannel.read(buffer);
        	buffer.flip();
            for (int i = 0; i < buffer.limit(); i++)
            {
            	System.out.println(inChannel.position());
//                System.out.print((char) buffer.get());
            }
            buffer.clear(); // do something with the data and clear/compact it.
		}
        
        
        inChannel.close();
        aFile.close();
    }
}