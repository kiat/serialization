#!/bin/bash

mvn clean compile assembly:single

rm loging.log

echo "Mutli Thread experiment for serialization type $1"

for serialization in  6 5 4 3 2 1
do
echo "next serialization type"

	for dataType in  1 2 3
	do

	rm -rf inputdata/*
	echo "Generating data for data type $dataType and Serializatin method $serialization "

	java -XX:-UseGCOverheadLimit  -XX:+UseConcMarkSweepGC -d64 -Xms10g   -Xmx60g   -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.dmodel.GenerateData  $serialization 0.1 $dataType


if (( $dataType==3 )) ; then
    lessObjectForCustomer=100
else
    lessObjectForCustomer=100000
fi

		for numberOfObjects in 1000 10000 $lessObjectForCustomer
		do

		echo "Running Multithread experiment for $numberOfObjects No. Of Objects in data type: $dataType, Serializatin method: $serialization "

		echo 3 > /proc/sys/vm/drop_caches && sync
		sleep 3

		time taskset -c 1,2,3,4 java -XX:-UseGCOverheadLimit  -XX:+UseConcMarkSweepGC -d64 -Xms10g   -Xmx60g   -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar   edu.rice.pdb.experiments.MultiThreadRead_Experiment $numberOfObjects $serialization  $dataType 

		# END OF different object numbers
		done
	# End of different data types - Part, LineItem, Customer
	done
# End of different serialization types
done
