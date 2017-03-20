#!/bin/bash

echo 'Started running the JSON reader ...'
mvn exec:java -Dexec.mainClass="edu.rice.pdb.serialization.json.UniversityJsonReader" -Dexec.args="-Xms4g   -Xmx46g -XX:-UseGCOverheadLimit"  

echo 'Started running the Google Protocol Buffer reader ...'
mvn exec:java -Dexec.mainClass="edu.rice.pdb.serialization.proto.ReadUniversityStudents" -Dexec.args="-Xms4g   -Xmx46g -XX:-UseGCOverheadLimit"  

echo 'Started running the JAVA Serialization reader ...'
mvn exec:java -Dexec.mainClass="edu.rice.pdb.serialization.javas.UniversityReader" -Dexec.args="-Xms4g   -Xmx60g -XX:-UseGCOverheadLimit"

