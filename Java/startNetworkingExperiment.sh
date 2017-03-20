#!/bin/bash
PDB_CLUSTER_CONFIG_FILE="clusterIPs.txt"


old_IFS=$IFS     # save the field separator
IFS=$'\n'        # new field separator, the end of line

counter=1

for line in $(cat "$PDB_CLUSTER_CONFIG_FILE")
do
   echo "$counter Stating ssh into $line"
   counter=$((counter+1))

#  ./sshClient.sh  $line $1 
ssh -i "Kia-Sourav-VLDB2016.pem" ubuntu@$1  'cd /home/ubuntu/Serialization/Java  && nohup time  java -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.SocketClient ec2-54-160-97-110.compute-1.amazonaws.com 5454 '"$1"' 1 2 false > /dev/null 2>&1 &;'

ssh -i "Kia-Sourav-VLDB2016.pem" ubuntu@$line  'cd /home/ubuntu/Serialization/Java  && nohup time  java -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.SocketClient   localhost 5454 1000  1 2 false > /dev/null 2>&1 &; '
# ssh -i "Kia-Sourav-VLDB2016.pem" ubuntu@$line  " cd /home/ubuntu/Serialization/Java  && ls -la & "



done


IFS=$old_IFS     # restore default field separator 

