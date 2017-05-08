package edu.rice.pdb.serial.networking;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.CountDownLatch;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

public class AggergationSignalSendingServer {

	final static Logger logger = Logger.getLogger(AggergationSignalSendingServer.class);

	ServerSocket myServerSocket;
	static boolean serverOn = true;
	static int port;
	static int numberOfClients = 1;

	static int portNumberOfFinalNode;
	static String hostnameOfFinalNode;

	CountDownLatch startSignal;

	static SocketClient forwardClient;

	public static void main(String[] args) {

		PropertyConfigurator.configure("log4j.properties");

		if (args.length < 4) {
			System.out.println("Run it with the following arguments");
			System.out.print("portNumber numberOfClients hostnameOfFinalNode  portNumberOfFinalNode ");
			System.exit(0);
		} else {
			AggergationSignalSendingServer.port = Integer.parseInt(args[0]);
			AggergationSignalSendingServer.numberOfClients = Integer.parseInt(args[1]);
			hostnameOfFinalNode = args[2];
			portNumberOfFinalNode = Integer.parseInt(args[3]);
		}

		AggergationSignalSendingServer mySignallingServer = new AggergationSignalSendingServer(port, numberOfClients);

	}

	public AggergationSignalSendingServer(int port, int numberOfClients) {

		startSignal = new CountDownLatch(numberOfClients);

		try {

			Socket finalNodeclientSocket = new Socket(hostnameOfFinalNode, portNumberOfFinalNode);
			DataOutputStream dosTofinalNode = new DataOutputStream(finalNodeclientSocket.getOutputStream());

			myServerSocket = new ServerSocket(port);

			// Server Start
			System.out.println("Signalling Server is up and running! Waiting for Clients to connect ...");

			// Successfully created Server Socket. Now wait for connections.
			while (AggergationSignalSendingServer.serverOn) {
				// Accept incoming connections.
				Socket clientSocket = myServerSocket.accept();

				// Start a Service thread
				System.out.println("Accepted a new client connection. Starting a Thread ... ");

				new FinalNodeSignalSendingService(clientSocket, startSignal).start();

				System.out.println("Start Client number: " + startSignal.getCount());
				startSignal.countDown();

				// START the Timer if all clients are connected and ready.
				if (startSignal.getCount() == 0) {
					// send the start signal to the final node so that it can
					// start its timer.
					System.out.println("Write the signal for final Server. ");
					dosTofinalNode.writeInt(1); // tell server to start the
												// timer

					AggergationSignalSendingServer.serverOn = false;
					finalNodeclientSocket.close();

				}

			} // End of while

		} catch (IOException ioe) {
			System.err.println("Exception encountered on accept. Ignoring. Stack Trace :" + ioe.getStackTrace().toString());
		}
	}
}