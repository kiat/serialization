#!/bin/bash
PDB_CLUSTER_CONFIG_FILE="clusterIPs.txt"


old_IFS=$IFS     # save the field separator
IFS=$'\n'        # new field separator, the end of line

counter=1


for line in $(cat "$PDB_CLUSTER_CONFIG_FILE")
do
   echo "$counter Stating ssh into $line"
   counter=$((counter+1))

ssh -oStrictHostKeyChecking=no -i "Kia-Sourav-VLDB2016.pem" ubuntu@$line  'cd Serialization && rm  Java/externalSortPart.sh && svn up && cd Java  && sudo rm loging.log nohup.out result_JAVA_ExternalSort-* && sudo chown -R  ubuntu:ubuntu   *  && svn up && mvn clean compile assembly:single '

# ssh -i "Kia-Sourav-VLDB2016.pem" ubuntu@$line  ''
# ssh -i "Kia-Sourav-VLDB2016.pem" ubuntu@$line  'cd /home/ubuntu/Serialization/Java  && sudo chown ubuntu:ubuntu * ' 
# ssh -i "Kia-Sourav-VLDB2016.pem" ubuntu@$line  'cd /home/ubuntu/Serialization/Java  && svn up && mvn clean compile assembly:single '
# ssh -i "Kia-Sourav-VLDB2016.pem" ubuntu@$line  'cd /home/ubuntu/Serialization/Java  && sudo rm nohup.out loging.log'
# ssh -i "Kia-Sourav-VLDB2016.pem" ubuntu@$line  'cd /home/ubuntu/Serialization/Java  && ps a && cat nohup.out && cat loging.log '



done


IFS=$old_IFS     # restore default field separator 
