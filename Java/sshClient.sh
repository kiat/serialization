#!/bin/bash

ssh -i "Kia-Sourav-VLDB2016.pem" ubuntu@$1  'cd /home/ubuntu/Serialization/Java  && nohup time  java -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.SocketClient ec2-54-160-97-110.compute-1.amazonaws.com 5454 '"$2"' 1 2 false > /dev/null 2>&1 &'

