package edu.rice.pdb.serial.networking;

import java.io.DataInputStream;
import java.net.Socket;

public class FinalNodeGetSignalService extends Thread {

	Socket myClientSocket;

	FinalNodeGetSignalService(Socket myClientSocket) {
		this.myClientSocket = myClientSocket;
	}

	public void run() {
		
		System.out.println("FinalNodeGetSignalService: Running a Thread for this client connection.  ");
		// Sockets
		try {

			DataInputStream dis = new DataInputStream(myClientSocket.getInputStream());

			if(dis.readInt()==1){
				// START the Timer if all clients are connected and ready.
				SocketServer.startTime = System.nanoTime();
				myClientSocket.close();
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}