#!/bin/bash
svn up

mvn clean compile assembly:single

java -XX:+UseNUMA  -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -d64 -Xms20g -Xmx40g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.localSort.GenerateDataForExternalSort   1  10010000



