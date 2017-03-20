package edu.rice.pdb.serial.networking;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.CountDownLatch;

public class FinalNodeSignalSendingService extends Thread {

	Socket myClientSocket;
	private final CountDownLatch startSignal;

	FinalNodeSignalSendingService(Socket myClientSocket, CountDownLatch startSignal) {
		this.myClientSocket = myClientSocket;
		this.startSignal = startSignal;
	}

	public void run() {
		
		System.out.println("FinalNodeSignalSendingService: Running a Thread for this client connection.  ");
		// Sockets
		try {
			DataOutputStream dos = new DataOutputStream(myClientSocket.getOutputStream());

			// wait until get the start signal and all of the other clients are connected and ready.
			startSignal.await();
			
			// Only if all servers are up then threads wake up and write signal 1 to the clients so that they start sending data.
			dos.writeInt(1);

		} catch (IOException | InterruptedException e) {
			e.printStackTrace();
		}
	}
}