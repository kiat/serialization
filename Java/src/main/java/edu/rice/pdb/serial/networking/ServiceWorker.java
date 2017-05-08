package edu.rice.pdb.serial.networking;

import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;

import org.apache.log4j.Logger;

import com.esotericsoftware.kryo.Kryo;

import edu.rice.dmodel.Customer;
import edu.rice.dmodel.Element;
import edu.rice.dmodel.LineItem;
import edu.rice.dmodel.Order;
import edu.rice.dmodel.Part;
import edu.rice.dmodel.RootData;
import edu.rice.dmodel.Supplier;
import edu.rice.pdb.read.SerializationMethod;
import edu.rice.pdb.serialization.Const;
import edu.rice.pdb.util.Utils;

class ServiceWorker extends Thread {
	Socket myClientSocket;
	boolean m_bRunThread = true;

	SerializationMethod serializationMethod;
	RootData myDataType;

	private final CountDownLatch startSignal;
	private final CountDownLatch stopSignal;

	String outPutFileName;
	Kryo kryo;

	int threadID;
	int numberOfExpectedObjectsToReceive;
	ArrayList<RootData> aggregatedData = new ArrayList<RootData>();

	final static Logger logger = Logger.getLogger(ServiceWorker.class);

	ServiceWorker(Socket myClientSocket, int threadID, CountDownLatch startSignal, CountDownLatch stopSignal, SerializationMethod serializationMethod, RootData myDataType, int numberOfExpectedObjectsToReceive) {
		this.myClientSocket = myClientSocket;
		this.startSignal = startSignal;
		this.stopSignal = stopSignal;
		this.myDataType = myDataType;
		this.threadID = threadID;
		this.serializationMethod = serializationMethod;

		this.numberOfExpectedObjectsToReceive = numberOfExpectedObjectsToReceive;

		if (serializationMethod == SerializationMethod.KRYO) {
			// Kryo is not multi-thread safe, so we need to create separate instances of kryo for each thread.
			this.kryo = new Kryo();
			this.kryo.register(Element.class);
			this.kryo.register(Supplier.class);
			this.kryo.register(Part.class);
			this.kryo.register(LineItem.class);
			this.kryo.register(Order.class);
			this.kryo.register(Customer.class);
		}
	}

	public void run() {

		try {

			// open final output file.
			FileOutputStream objectFileOutput = new FileOutputStream(new File("tmp/ReceivedDataFile-" + threadID + ".object"), true);
			BufferedOutputStream bout = new BufferedOutputStream(objectFileOutput);
			ObjectOutputStream fileStream = new ObjectOutputStream(bout);

			// Sockets
			DataInputStream dis = new DataInputStream(myClientSocket.getInputStream());
			DataOutputStream dos = new DataOutputStream(myClientSocket.getOutputStream());
			ArrayList<RootData> mydata = new ArrayList<RootData>();

			// wait until get the start signal and all of the other clients are connected and ready.
			startSignal.await();

			if (SocketServer.localStorage) {
				// Write one to signal and request data from client
				dos.writeInt(1);

			} else {
				// if this is a forwarding server wait for the GO signal from upper level servers.
				if (SocketServer.gotGOSignalFromOtherServer)
					dos.writeInt(1);
			}

			System.out.println("Expects to receive " + this.numberOfExpectedObjectsToReceive + " number of objects to receive from client.");


			// do it until we break out of this loop and all data is received.
			while (true) {

				int hasAPage = dis.readInt();

				if (hasAPage == 1) {

					byte[] tmpData = new byte[80 * Const.PAGESIZE];

					// read the main byte array into memory
					dis.readFully(tmpData);

					// Two cases might be possible
					// Case 1 - data should be stored locally
					// Case 2 - data should be aggregated
					if (SocketServer.localStorage && !SocketServer.aggergationServer) {
						// in this case we get data over the network and flush it to disk. 
						
						fileStream.write(tmpData);
						fileStream.flush();
						
						
						
					} else {
						// Only in this case only we need to access individual objects.
						int position = 0;
						ByteBuffer byteBuffer = ByteBuffer.wrap(tmpData);

						while (position <= 80 * Const.PAGESIZE) {
							int objectLegth = byteBuffer.getInt();

							// If we read a zero or negative length
							// Because at the end of each page we have an empty space and if we read that area as int we get zero or negative length.
							if (objectLegth <= 0) {
								// System.out.println("Read one page object counter is: " + objectCounter);
								break;
							}

							byte[] rawDataEachObject = new byte[objectLegth];

							byteBuffer.get(rawDataEachObject);

							// deserialize from tmp array
							RootData m_object = null;

							// Select based on the serialization method.
							switch (this.serializationMethod) {
							case JAVADEFAULT:
								m_object = myDataType.javaDefaultDeserialization(rawDataEachObject);
								break;
							case JSON:
								m_object = myDataType.jsonDeserialization(rawDataEachObject);
								break;
							case BSON:
								m_object = myDataType.bsonDeSerialization(rawDataEachObject, myDataType.getClass());
								break;
							case PROTOCOL:
								m_object = myDataType.protocolBufferRead(rawDataEachObject);
								break;
							case KRYO:
								m_object = myDataType.kryoDeserialization(rawDataEachObject, myDataType.getClass(), this.kryo);
								break;
							case BYTEBUFFER:
								m_object = myDataType.readByteBuffer(rawDataEachObject);
							default:
								break;
							}

							mydata.add(m_object);
							// Checked if we have received objects correctly.
							position = position + 4 + objectLegth;
						}

					}

				} else {

					stopSignal.countDown();

					// Vector aggregation: Add received data into the list.
					synchronized (SocketServer.receivedData) {
						SocketServer.receivedData.add(mydata);
					}

					// Calculate the time when all of the data chunks are received on the server side.
					if (this.stopSignal.getCount() == 0) {

						
						
						
						// ########################################################################################
						// ########################################################################################
						// ########################################################################################
						// Only in case of Network Vector aggregation
						// If we need to do the vector aggregation experiment and this node is not a final destination.
						if (SocketServer.aggergationServer) {
							System.out.println("Received data vectors : " + SocketServer.receivedData.size());
							// add the data of this thread to the received data list of the server.

							System.out.println("Received data vectors : " + SocketServer.receivedData.size());
							System.out.println("Received data part 0 SIZE " + SocketServer.receivedData.get(0).size());
							System.out.println("Received data part 1 SIZE " + SocketServer.receivedData.get(1).size());
							System.out.println("serverData SIZE " + SocketServer.serverData.size());

							if(SocketServer.receivedData.get(0)!= null && SocketServer.receivedData.get(1)!= null && SocketServer.serverData != null)
							aggregatedData = Utils.aggergateSparseVector(SocketServer.receivedData.get(0), SocketServer.receivedData.get(1), SocketServer.serverData);
							else{
								System.out.println("One of the Vectos is null:");
							}
							
							
							System.out.println("Size of Aggegated Vector is " + aggregatedData.size() + " sending to " + SocketServer.forwardServer + ":" + SocketServer.forwardPort);

							// this is the implementation for the large vector aggregation.
							// If we have already a data set and we have to aggregate them.
							// Do vector aggregation.
							// forward to another server if this is not a final aggregation node.
							if (!SocketServer.localStorage) {

								// Select based on the serialization method.
								switch (serializationMethod) {
								case JAVADEFAULT:
									// here true means that we do not wait for the GO signal because we already waited for GO signal from upper server.
									SocketServer.forwardClient.prepareAndSend(aggregatedData, 1, SerializationMethod.JAVADEFAULT, true);
									break;
								case JSON:
									SocketServer.forwardClient.prepareAndSend(aggregatedData, 1, SerializationMethod.JSON, true);
									break;
								case BSON:
									SocketServer.forwardClient.prepareAndSend(aggregatedData, 1, SerializationMethod.BSON, true);
									break;
								case PROTOCOL:
									SocketServer.forwardClient.prepareAndSend(aggregatedData, 1, SerializationMethod.PROTOCOL, true);
									break;
								case KRYO:
									SocketServer.forwardClient.prepareAndSend(aggregatedData, 1, SerializationMethod.KRYO, true);
									break;
								case BYTEBUFFER:
									SocketServer.forwardClient.prepareAndSend(aggregatedData, 1, SerializationMethod.BYTEBUFFER, true);
									break;
								default:
									break;
								}
							}else {
								// it is a final aggregation node that has to aggregate and store
								// we need to serialize data and write down. 
								
								ArrayList<byte[]> rawDatalist = new ArrayList<byte[]>();
								
								for (RootData objectData: aggregatedData) {
									
									
									
									// Select based on the serialization method.
									switch (serializationMethod) {
									case JAVADEFAULT:
										rawDatalist.add(objectData.javaDefaultSerialization());
										 break;
									case JSON:
										rawDatalist.add(objectData.jsonSerialization());
										break;
									case BSON:
										rawDatalist.add(objectData.bsonSerialization());
										break;
									case PROTOCOL:
										rawDatalist.add(objectData.protocolBufferWrite());
										break;
									case KRYO:
										rawDatalist.add(objectData.kryoSerialization(kryo));
										break;
									case BYTEBUFFER:
										rawDatalist.add(objectData.writeByteBuffer());
										break;
									}
									
									
									// 	we append serialized data when we collected 100K serialized data objects
									if(rawDatalist.size()>1000000){
										Utils.appendObjects(rawDatalist, fileStream);
										rawDatalist = new ArrayList<byte[]>();
									}
									
									
								}// end of ForLoop
								// at the end of for loop add the rest. 
								Utils.appendObjects(rawDatalist, fileStream);

								
								
								
								
								
							}
							// ########################################################################################
							// ########################################################################################
							// ########################################################################################
						}
						

						// End of time calculation
						long endTime = System.nanoTime();
						double elapsedSeconds = (endTime - SocketServer.startTime) / 1000000000.0;
						logger.info("[NetworkJAVA]#" + SocketServer.sendFromDisk + "#" + this.numberOfExpectedObjectsToReceive + "#" + this.serializationMethod + "#" + this.myDataType.getClass().getSimpleName() + "#"
								+ elapsedSeconds);

						
						sleep(1); 
						// If and Only if all clients are done with sending data then we can exit.
						System.exit(0);
					}

					// Only if this thread has no more pages to get then we do this
					// Clean up
					dis.close();
					dos.close();
					myClientSocket.close();
					SocketServer.serverOn = false;

					System.out.println("Thread "+threadID+" Stopped ...");
					break; // break out of while loop

				}

			} // END of WHILE LOOP

		} catch (IOException e) {
			logger.error("IOException on ServiceWorker Thread ", e);
			System.err.println("IOException on ServiceWorker Thread: " + e.getStackTrace());
		} catch (InterruptedException e) {
			System.err.println("InterruptedException on ServiceWorker Thread: " + e.getStackTrace());
			logger.error("InterruptedException on ServiceWorker Thread ", e);
		}
	}

}