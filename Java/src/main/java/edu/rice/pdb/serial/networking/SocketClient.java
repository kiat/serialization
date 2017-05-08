package edu.rice.pdb.serial.networking;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import edu.rice.dmodel.Customer;
import edu.rice.dmodel.Element;
import edu.rice.dmodel.IndexData;
import edu.rice.dmodel.LineItem;
import edu.rice.dmodel.Part;
import edu.rice.dmodel.RootData;
import edu.rice.pdb.read.SerializationMethod;
import edu.rice.pdb.read.WriteAndRead;
import edu.rice.pdb.serialization.Const;
import edu.rice.pdb.serialization.KryoSinglton;
import edu.rice.pdb.util.Utils;

public class SocketClient {
	Socket clientSocket;
	DataInputStream dis;
	DataOutputStream dos;

	int numberOfObjects;
	String hostName;
	int portNumber;

	private boolean clientForAggergation = false;
	private int singalingServerPortNumber;
	private String singalingServerHostName;

	public boolean isClientForAggergation() {
		return clientForAggergation;
	}

	public void setClientForAggergation(boolean clientForAggergation) {
		this.clientForAggergation = clientForAggergation;
	}

	public int getSingalingServerPortNumber() {
		return singalingServerPortNumber;
	}

	public void setSingalingServerPortNumber(int singalingServerPortNumber) {
		this.singalingServerPortNumber = singalingServerPortNumber;
	}

	public String getSingalingServerHostName() {
		return singalingServerHostName;
	}

	public void setSingalingServerHostName(String singalingServerHostName) {
		this.singalingServerHostName = singalingServerHostName;
	}

	public SocketClient(String hostName, int portNumber, int numberOfObjects) {
		this.hostName = hostName;
		this.portNumber = portNumber;
		this.numberOfObjects = numberOfObjects;

		// try {
		// clientSocket = new Socket(hostName, portNumber);
		// dis = new DataInputStream(clientSocket.getInputStream());
		// dos = new DataOutputStream(clientSocket.getOutputStream());
		// } catch (Exception e) {
		// e.printStackTrace();
		// }

	}

	final static Logger logger = Logger.getLogger(SocketClient.class);

	// Example run if this client is
	// java -Xms4g -Xmx8g -cp ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.serial.networking.SocketClient localhost 5454 1000 1
	// 1 true
	// true means reading from client or not
	// Order of arguments are: hostname, port number, number of objects to read, serialization type, data type

	public static void main(String[] args) throws IOException {

		PropertyConfigurator.configure("log4j.properties");

		String hostname;
		int portNumber, numberOfObjects, serializationTypeInt = 0, dataTypeInt = 0;
		boolean readFromDisk = false, clientForAggergation = false;
		List<RootData> mydata = null;

		SocketClient myClient = null;
		if (args.length < 6) {
			System.out.println("run the following arguments");
			System.out.println("hostname portNumber numberOfObjects   an int for SerializationMethod 1-JavaDefault 2-JSON 3-BSON  4-PROTOCOL 5-KRYO 6-BYTEBUFFER");
			System.out.println("DataTypes: 1. Element 2. Part 3. LineItem 4. Customer ");
			System.out.println("Readtype: true read from disk, false read from RAM. ");
			System.exit(0);
		} else {

			hostname = args[0];
			try {
				portNumber = Integer.parseInt(args[1]);
				numberOfObjects = Integer.parseInt(args[2]);
				serializationTypeInt = Integer.parseInt(args[3]);
				dataTypeInt = Integer.parseInt(args[4]);
				readFromDisk = Boolean.parseBoolean(args[5]);
				clientForAggergation = Boolean.parseBoolean(args[6]);

				System.out.println("Client connects to " + hostname + ":" + portNumber + " for Sending No. of Objects: " + numberOfObjects);

				// create a client
				myClient = new SocketClient(hostname, portNumber, numberOfObjects);

				// if this client is used for vector aggregation experiment then we have to setup a signaling server to let all clients start at the same time.
				if (clientForAggergation) {
					System.out.println("This client is for Aggergation Experiment");
					myClient.setClientForAggergation(clientForAggergation);
					myClient.setSingalingServerHostName(args[7]);
					myClient.setSingalingServerPortNumber(Integer.parseInt(args[8]));
				}
			} catch (NumberFormatException e) {
				e.printStackTrace();
			}
		}

		SerializationMethod serialMethod = null;
		switch (serializationTypeInt) {
		case 1:
			serialMethod = SerializationMethod.JAVADEFAULT;
			break;
		case 2:
			serialMethod = SerializationMethod.JSON;
			break;
		case 3:
			serialMethod = SerializationMethod.BSON;
			break;
		case 4:
			serialMethod = SerializationMethod.PROTOCOL;
			break;
		case 5:
			serialMethod = SerializationMethod.KRYO;
			break;
		case 6:
			serialMethod = SerializationMethod.BYTEBUFFER;
			break;
		}

		System.out.println("Client sending data  " + dataTypeInt + " Serilization: " + serialMethod + " from disk " + readFromDisk);

		if (readFromDisk) {
			// In this case we read serialized data from disk and send it as is without any deserialization.
			// Read from disk and send it over the socket as ByteArray no deserialization and serialization.
			myClient.readByteArrayAndSend(dataTypeInt, serialMethod, myClient.numberOfObjects);
		} else {
			// In this case we read serialized data from disk deserialize it and then serialize it again to send over the network.
			mydata = myClient.readData(dataTypeInt, serialMethod);

			System.out.println("Data in RAM with size : " + mydata.size());

			// Select based on the serialization method.
			switch (serialMethod) {
			case JAVADEFAULT:
				myClient.prepareAndSend(mydata, dataTypeInt, SerializationMethod.JAVADEFAULT, false);
				break;
			case JSON:
				myClient.prepareAndSend(mydata, dataTypeInt, SerializationMethod.JSON, false);
				break;
			case BSON:
				myClient.prepareAndSend(mydata, dataTypeInt, SerializationMethod.BSON, false);
				break;
			case PROTOCOL:
				myClient.prepareAndSend(mydata, dataTypeInt, SerializationMethod.PROTOCOL, false);
				break;
			case KRYO:
				myClient.prepareAndSend(mydata, dataTypeInt, SerializationMethod.KRYO, false);
				break;
			case BYTEBUFFER:
				myClient.prepareAndSend(mydata, dataTypeInt, SerializationMethod.BYTEBUFFER, false);
				break;
			default:
				logger.error("SocketClient - Serialization Method is not defined. ");
				break;
			}
		}
	}

	// this method reads or generates data into RAM for sending over socket.
	public List<RootData> readData(int dataType, SerializationMethod serialMethod) throws IOException {

		System.out.println("readData: data type is " + dataType + " Serialization is " + serialMethod);

		List<RootData> mydata = null;

		try {
			switch (dataType) {
			case 1:
				// generate a random sparse vector with 100M size, density is numberOfObjects
				mydata = new Element().generateRandomObjects(100000000, this.numberOfObjects);
				break;
			case 2:
				mydata = WriteAndRead.readObjects(serialMethod, new Part(), this.numberOfObjects, this.numberOfObjects + 1, "inputdata/PART" + serialMethod + ".index", "inputdata/PART" + serialMethod + ".object", true,
						false);
				break;
			case 3:
				mydata = WriteAndRead.readObjects(serialMethod, new LineItem(), this.numberOfObjects, this.numberOfObjects + 1, "inputdata/LINEITEM" + serialMethod + ".index", "inputdata/LINEITEM" + serialMethod
						+ ".object", true, false);
				break;
			case 4:
				mydata = WriteAndRead.readObjects(serialMethod, new Customer(), this.numberOfObjects, this.numberOfObjects + 1, "inputdata/CUSTOMER" + serialMethod + ".index", "inputdata/CUSTOMER" + serialMethod
						+ ".object", true, false);
				break;
			}
		} catch (ClassNotFoundException e) {
			logger.error(e);
		}
		return mydata;
	}

	/**
	 * This method prepare the data in byte arrays and sends them over the socket.
	 * 
	 * @param mydata
	 *            - is the data that has to be sent over the socket.
	 * @paran serializationMethod - is the choice for serialization of objects.
	 */

	void prepareAndSend(List<RootData> mydata, int dataType, SerializationMethod serializationMethod, boolean isForwarding) {

		try {
			clientSocket = new Socket(hostName, portNumber);
			dis = new DataInputStream(clientSocket.getInputStream());
			dos = new DataOutputStream(clientSocket.getOutputStream());
		} catch (Exception e) {
			e.printStackTrace();
		}

		if (this.isClientForAggergation()) {

			try {
				clientSocket = new Socket(this.getSingalingServerHostName(), this.getSingalingServerPortNumber());
				DataInputStream mySignallingDis = new DataInputStream(clientSocket.getInputStream());
				// DataOutputStream mySignallingDos = new DataOutputStream(clientSocket.getOutputStream());
				if (mySignallingDis.readInt() == 1)
					System.out.println("GotSignal From Signalling Server.");

			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		System.out.println("Prepare and send: data type is " + dataType + " Serialization is " + serializationMethod);

		try {
			ByteBuffer m_buffer = ByteBuffer.allocate(80 * Const.PAGESIZE);

			int position = 0;
			/*
			 * wait until get a signal 1 from the server to start sending data over.
			 */
			if (isForwarding || dis.readInt() == 1) {

				byte[] s_data = null;

				byte[] myDataToSend = new byte[80 * Const.PAGESIZE];
				int bigPageCounter = 0;

				int pageCounter = 0;
				// send objects as byte array one by one
				for (RootData rootData : mydata) {

					// Select based on the serialization method.
					switch (serializationMethod) {
					case JAVADEFAULT:
						s_data = rootData.javaDefaultSerialization();
						break;
					case JSON:
						s_data = rootData.jsonSerialization();
						break;
					case BSON:
						s_data = rootData.bsonSerialization();
						break;
					case PROTOCOL:
						s_data = rootData.protocolBufferWrite();
						break;
					case KRYO:
						s_data = rootData.kryoSerialization(KryoSinglton.getInstance().getKryo());
						break;
					case BYTEBUFFER:
						s_data = rootData.writeByteBuffer();
						break;
					default:
						logger.error("SocketClient - Serialization Method is not defined. ");
						break;
					}

					position = position + 4 + s_data.length; // 4 bytes + length of serialized data

					if (position > Const.PAGESIZE) {

						m_buffer = ByteBuffer.allocate(Const.PAGESIZE);

						byte[] tmp_data = m_buffer.array();

						System.arraycopy(tmp_data, 0, myDataToSend, 0, tmp_data.length);

						if (pageCounter == 80) {
							// send the page over and get a new page
							dos.writeInt(1); // tell server that it has a page
							// We write an integer if the client has a page to send and write 0 if the client has no more pages to sind and is finished.

							dos.write(myDataToSend);
							dos.flush();

							// Get a new big page
							myDataToSend = new byte[80 * Const.PAGESIZE];
							pageCounter = 0;
							bigPageCounter++;
							System.out.println("Sent " + bigPageCounter + " pages to the Server.");
						}

						// set back the position in buffer
						position = 4 + s_data.length;
						pageCounter++;
					}
					m_buffer.putInt(s_data.length);
					m_buffer.put(s_data);

				} // End of For loop

				// At the final End, write the rest data which is smaller than a page
				// send the page over and get a new page
				dos.writeInt(1); // tell server that it has a page

				byte[] mydataToSend = m_buffer.array();
				byte[] dataForSendOverNetwork = new byte[80 * Const.PAGESIZE];

				// we need to send always a large page over the network
				// if we convert the rest byte array into a page then it is smaller than our standard page 80x pageSize.
				System.arraycopy(mydataToSend, 0, dataForSendOverNetwork, 0, mydataToSend.length);

				dos.write(dataForSendOverNetwork);

				// At the final End
				// tell server that it has no more pages
				dos.writeInt(0);
				dos.flush();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	void readByteArrayAndSend(int dataType, SerializationMethod serialMethod, int numberOfObjects) {
		// ByteBuffer bb = ByteBuffer.allocateDirect(Const.PAGESIZE);



		String indexFileName = "", objectFilename = "";

		switch (dataType) {
		case 1:
			indexFileName = "inputdata/ELEMENT" + serialMethod + ".index";
			objectFilename = "inputdata/ELEMENT" + serialMethod + ".object";
			break;
		case 2:
			indexFileName = "inputdata/PART" + serialMethod + ".index";
			objectFilename = "inputdata/PART" + serialMethod + ".object";
			break;
		case 3:
			indexFileName = "inputdata/LINEITEM" + serialMethod + ".index";
			objectFilename = "inputdata/LINEITEM" + serialMethod + ".object";
			break;
		case 4:
			indexFileName = "inputdata/CUSTOMER" + serialMethod + ".index";
			objectFilename = "inputdata/CUSTOMER" + serialMethod + ".object";
			break;
		}

		
		
		// ArrayList<byte[]> myData = new ArrayList<byte[]>();
		try {
			clientSocket = new Socket(hostName, portNumber);
			dis = new DataInputStream(clientSocket.getInputStream());
			dos = new DataOutputStream(clientSocket.getOutputStream());
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		try {

			IndexData sizeIndex = Utils.readIndex(indexFileName, 0 );
			long[] indexStarts = sizeIndex.getStarts();

			/*
			 * wait until get a signal 1 from the server to start sending data over.
			 */
			if (dis.readInt() == 1) {

				long startTime = 0;
				double elapsedIOTime = 0;

				// NOW read the objects from memory
				// START OF Time calculation

				Path path = Paths.get(objectFilename);
				FileChannel fileChannel = FileChannel.open(path);

				// NOW read the objects from memory
				// START OF Time calculation
				startTime = System.nanoTime();

				ByteBuffer data = ByteBuffer.allocate(80 * Const.PAGESIZE);

				long position_each_object = indexStarts[numberOfObjects - 1];

				int pageID = (int) (position_each_object / Const.PAGESIZE);
				int bigPageCounts = pageID / 80;

				System.out.println("Sending " + (bigPageCounts + 1) + " pages to the Server.");

				for (long i = 0; i < bigPageCounts; i++) {
					fileChannel.position((i + 1) * (long) 80 * (long) Const.PAGESIZE);
					fileChannel.read(data); // read object data from disk

					// If we send a page we write a 1 to the socket so that the server knows that we have a page to send
					dos.writeInt(1);
					dos.write(data.array());
					dos.flush();

					// clean up the ByteBuffer for the next read.
					data.flip();
					data.clear();
				}

				// if we have no page to send we write a zero.
				dos.writeInt(0);

				// End of time calculation
				long endTime = System.nanoTime();
				double elapsedTotalTime = (endTime - startTime) / 1000000000.0;
				logger.info("[ReadTimeJAVA]#" + String.format("%.9f", elapsedIOTime) + "#" + String.format("%.9f", elapsedTotalTime));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}