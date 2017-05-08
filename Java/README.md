Experiments on Complex Object Models and Serialization with Java 


This project contains our java implementation of the complex object experiments with different java serialization techniques. 


# Install maven - compile 
To compile the project and create a single jar file with all dependencies: 
	
	mvn clean compile assembly:single
	

# Generate Experiment Data 

run the following script to generate data. 

	./generateData.sh 
 
 
 
# Run Experiments 

For each Experiment you can find run shell scripts. Results of the scripts will be stored in the log file. 


## Experiment 1 -  Read from disk - Sequential and Random Access read. 
 
 run the following script 
 
	./runExperiment_1_DiskIO.sh

## Experiment 2 - Network Data Requests 

 You need to run a server and 10 Clients. 

 For the server, you can run
  
	./runExp2_NetworkRequests_Server.sh
	
Use ClusterSSH to login to the 10 client machines and run the following script

	./runExp2_NetworkRequests_Clients.sh
	

## Experiment 3 - External Data Sort
We provided 3 different shell scripts for each data types, *Part* , *Lineitem* and *Customer* 

	./runExp3_ExternalSort_Part.sh
	
	./runExp3_ExternalSort_Lineitem.sh
	
	./runExp3_ExternalSort_Customer.sh
	

	
## Experiment 4 - Sparse Vector Aggregation 

This experiment needs 8 machines. 
1 machine is the final server that does the final data aggregation. 
3 machines are servers and clients that receive data, aggregate and forward them to final server.
4 machines are simple clients generate and send vector data. 


Run the final server or the aggregation servers with the following example arguments

	java    -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.serial.networking.SocketServer 5454 2 2000 5 2  true false false

 

	PortNumber,  NumberOfClients, numberOfExpectedObjectsToReceive, an int for Serialization Type, an int for Data type, true/false (final node), true/false send from disk or not, true/false is this a aggeragation server (if true server address and port for forwarding of data) 
	
	
	Serialization Types: 1 JAVADEFAULT, 2. JSON, 3. BSON, 4. PROTOCOL, 5. KRYO, 6. BYTEBUFFER 
	DataTypes: 1. Element 2. Part 3. LineItem 4. Customer
	
	
	
Use the clusterSSH and run the data sending clients with the following argument 

	java  -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.serial.networking.SocketClient localhost 5454 1000 5 2 false  

	


Serialization Methods 
----------------------

# Java Default
# JSON
javax.json 

# BSON
https://github.com/michel-kraemer/bson4jackson

https://docs.mongodb.com/ecosystem/drivers/java/

# google protocol buffers
https://developers.google.com/protocol-buffers/


**# google protocol buffer commands 
 
It is needed to install protocbuf package in Ubuntu system.
To run the Protocol Buffer compuler and generate java class, 
 
protoc -I=./protos --java_out=./src/main/java/  protos/Part.proto


# Kryo 

https://github.com/EsotericSoftware/kryo 


# Futher Serialization Framework for Java 

There are many futher serialization frameworks for Java that we have not addressed in our experiments. We think that they are similar to one of the frameworks that we choosen for our experiments like google protocol buffer or Kryo. 

Protostuff
https://github.com/protostuff/protostuff

Avro 
http://avro.apache.org/
https://github.com/phunt/avro-rpc-quickstart

Thrift 
http://thrift.apache.org/
