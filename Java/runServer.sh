#!/bin/bash
svn  up

mvn clean compile assembly:single


svn up &&  mvn clean compile assembly:single 

# Run normal final Server to collect data  
taskset -c 0 java -Xms8g -Xmx16g -cp ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.SocketServer 5454 2 20000 1 1 true false false

 
# A Client for sending data. 
taskset -c 0 java -Xms4g -Xmx16g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar   edu.rice.pdb.serial.networking.SocketClient 54.196.29.199 5454  1000 6 2 false false





# Command for setting a fresh server. 

cd Serialization && rm  Java/externalSortPart.sh && svn up && cd Java 

sudo rm loging.log nohup.out result_JAVA_ExternalSort-* && sudo chown -R  ubuntu:ubuntu   *

svn up &&  mvn clean compile assembly:single 

# Data Generation. 





# A complete Example of Vector Aggregation 

# Singalling Server 
java -Xms8g -Xmx16g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.AggergationSignalSendingServer 5455 4 localhost 5456 

# Collecting Final Server 
taskset -c 0 java -Xms8g -Xmx16g -cp ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.SocketServer 5454 2 20000 1 1 true falserue 5456 

# Two Forwarding Servers 
1. 
taskset -c 1 java -Xms8g -Xmx16g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.SocketServer 5457 2 20000  1 1  false false  true localhost 5454 

2.
taskset -c 2 java -Xms8g -Xmx16g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar   edu.rice.pdb.serial.networking.SocketServer 5458 2 20000  1 1  false false true  localhost 5454

# 4 Clients 
1. 
java -Xms4g -Xmx16g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar   edu.rice.pdb.serial.networking.SocketClient localhost 5457  10000 1 1 false true localhost 5455

2. exactly the same as 1. 

3.
java -Xms4g -Xmx16g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.SocketClient localhost 5458  10000 1 1 false true localhost 5455

4. exactly the same as 3  


 