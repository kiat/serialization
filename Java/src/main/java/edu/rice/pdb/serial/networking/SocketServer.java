package edu.rice.pdb.serial.networking;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import edu.rice.dmodel.Customer;
import edu.rice.dmodel.Element;
import edu.rice.dmodel.LineItem;
import edu.rice.dmodel.Part;
import edu.rice.dmodel.RootData;
import edu.rice.pdb.read.SerializationMethod;

public class SocketServer {

	final static Logger logger = Logger.getLogger(SocketServer.class);

	ServerSocket myServerSocket;
	static boolean serverOn = true;
	static boolean gotData = false;
	static boolean gotGOSignalFromOtherServer = false;

	static boolean sendFromDisk = false;
	static boolean aggergationServer = false;

	// static boolean startTIimeCalculation = false;

	static double startTime;

	static int port;
	static int signallingPort ; 
	static int numberOfClients = 1;
	static int numberOfExpectedObjectsToReceive;

	int threadID = 0;
	CountDownLatch startSignal;
	CountDownLatch stopSignal;
	static SocketClient forwardClient;
	static boolean readyToRecieveData = false;

	/*
	 * If the localStorage is true received data will be deserialized and stored locally. If the locaStorage is false received data will be forward to another Server
	 */
	static boolean localStorage = false;
	static String forwardServer;
	static int forwardPort;
	static ArrayList<RootData> pastData = null;

	static ArrayList<ArrayList<RootData>> receivedData = null;
	static ArrayList<RootData> serverData = null;

	// Example run
	// java -Xms4g -Xmx8g -cp ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.serial.networking.SocketServer 5454 2 2000000 5 1 false
	public static void main(String[] args) {

		PropertyConfigurator.configure("log4j.properties");

		receivedData = new ArrayList<ArrayList<RootData>>();

		// Before any experiments create a hot gc
		// Create a hot garbage collector
		// Garbage.activateGarbageCollector();
		// System.out.println("Activating Java garbage collector!");
		// System.gc();

		System.out.println("Server is up and running!");

		if (args.length < 4) {

			System.out.println("Run it with the following arguments");
			System.out.print("PortNumber,  NumberOfClients, numberOfExpectedObjectsToReceive, an int for Serialization Type, an int for Data type, true/false (final node), true/false send from disk or not, true/false is this a aggeragation server (if true server address and port for forwarding of data) ");
			System.out.println("DataTypes: 1. Element 2. Part 3. LineItem 4. Customer ");
			System.out.println("Forwarding hostname port");

			System.exit(0);
		} else {
			// default is no forwarding to other servers.

			port = Integer.parseInt(args[0]);
			numberOfClients = Integer.parseInt(args[1]);
			numberOfExpectedObjectsToReceive = Integer.parseInt(args[2]);

			System.out.println("numberOfExpectedObjectsToReceive " + numberOfExpectedObjectsToReceive);
			localStorage = Boolean.parseBoolean(args[5]);
			sendFromDisk = Boolean.parseBoolean(args[6]);
			aggergationServer = Boolean.parseBoolean(args[7]);

			System.out.println("LocalStorage=" + localStorage);

			if (aggergationServer) {
				// generate the Server data for aggregation.
				serverData = new Element().generateRandomObjects(100000000, (numberOfExpectedObjectsToReceive) / numberOfClients);
				System.out.println("This is an aggregation Server. Generated Server Data!");
			}
			
			if (!localStorage && aggergationServer) {
				forwardServer = args[8];
				forwardPort = Integer.parseInt(args[9]);
				System.out.println("Forwarding to " + forwardServer + ":" + forwardPort);
			}

			if (localStorage && aggergationServer) {
				signallingPort = Integer.parseInt(args[8]);

				// An inLine Thread to start a new server for signaling
				(new Thread() {
					public void run() {
						// If this is a final node of aggegation experiment.
						System.out.println("This is a final node for aggergation experiment.");
						ServerSocket timeStartSignallingSocket;
						try {
							timeStartSignallingSocket = new ServerSocket(signallingPort);
							// Accept incoming connections.
							Socket signallingClientSocket = timeStartSignallingSocket.accept();
							new FinalNodeGetSignalService(signallingClientSocket).start();
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
				}).start();

			}

			RootData tmp = null;
			switch (Integer.parseInt(args[4])) {
			case 1:
				tmp = new Element();
				break;
			case 2:
				tmp = new Part();
				break;
			case 3:
				tmp = new LineItem();
				break;
			case 4:
				tmp = new Customer();
				break;
			}

			switch (Integer.parseInt(args[3])) {
			case 1:
				new SocketServer(port, numberOfClients, numberOfExpectedObjectsToReceive, SerializationMethod.JAVADEFAULT, tmp);
				break;
			case 2:
				new SocketServer(port, numberOfClients, numberOfExpectedObjectsToReceive, SerializationMethod.JSON, tmp);
				break;
			case 3:
				new SocketServer(port, numberOfClients, numberOfExpectedObjectsToReceive, SerializationMethod.BSON, tmp);
				break;
			case 4:
				new SocketServer(port, numberOfClients, numberOfExpectedObjectsToReceive, SerializationMethod.PROTOCOL, tmp);
				break;
			case 5:
				new SocketServer(port, numberOfClients, numberOfExpectedObjectsToReceive, SerializationMethod.KRYO, tmp);
				break;
			case 6:
				new SocketServer(port, numberOfClients, numberOfExpectedObjectsToReceive, SerializationMethod.BYTEBUFFER, tmp);
				break;
			}

		} // End of else if for arguments.

	}

	public SocketServer(int port, int numberOfClients, int numberOfExpectedObjectsToReceive, SerializationMethod serializationMethod, RootData myDataType) {

		System.out.println("ServerPort: " + port + "  No.ofClients: " + numberOfClients + " ExpectedObjectsToReceive: " + numberOfExpectedObjectsToReceive + " DataType: " + myDataType.getClass().getSimpleName()
				+ " SerializationType:" + serializationMethod);

		startSignal = new CountDownLatch(numberOfClients);
		stopSignal = new CountDownLatch(numberOfClients);

		if (!SocketServer.localStorage) {
			// Send to another Server/Node for the processing.
			System.out.println("Starting a Forwarding Client to " + forwardServer + ":" + forwardPort + " and wait for GO signal ...");
			forwardClient = new SocketClient(forwardServer, forwardPort, numberOfExpectedObjectsToReceive / numberOfClients);

			try {
				// wait until we get a 1 from upper Server if we have to forward data
				if (forwardClient.dis.readInt() == 1)
					gotGOSignalFromOtherServer = true;
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		
		try {
			myServerSocket = new ServerSocket(port);

			// Server Start
			System.out.println("Server is up and running!");

			// Successfully created Server Socket. Now wait for connections.
			while (SocketServer.serverOn) {

				// Accept incoming connections.
				Socket clientSocket = myServerSocket.accept();

				// Start a Service thread
				System.out.println("Accepted a new client connection. Starting a Thread ... ");
				threadID++;

				new ServiceWorker(clientSocket, threadID, startSignal, stopSignal, serializationMethod, myDataType, numberOfExpectedObjectsToReceive).start();
				// send also the start time of the experiment.

				System.out.println("Start Client number: " + startSignal.getCount());

				startSignal.countDown();

				if (startSignal.getCount() == 0 ) {
					
					System.out.println("Started the time in Socket Server. ");
					// START the Timer if all clients are connected and ready.
					SocketServer.startTime = System.nanoTime();
					SocketServer.serverOn = false;
				}

			} // End of while

			System.out.println("Server is DONE!  final node " + SocketServer.localStorage);
			
		} catch (IOException ioe) {
			System.err.println("Exception encountered on accept. Ignoring. Stack Trace :" + ioe.getStackTrace().toString());
		}

	}

}