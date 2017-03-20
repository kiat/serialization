#!/bin/bash

mvn clean compile assembly:single 


for serializationType in 1 2 3 4 5 6
do


for dataType in 1 2 3
do

echo "DataTyple $dataType  - Serialization Type: $serializationType"

java -XX:-UseGCOverheadLimit  -XX:+UseConcMarkSweepGC -d64 -Xms10g   -Xmx60g   -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.dmodel.GenerateData  $serializationType  0.1  $dataType

done
done
