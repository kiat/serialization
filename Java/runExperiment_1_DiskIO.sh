#!/bin/bash

mvn clean compile assembly:single

for num in  10 100 1000 10000 100000
do

  for counter in  1 2 3 4 5
  do
	rm -rf loging.log


	# write the first line
	echo "language#method#seq#datatype#iotime#totaltime" > loging.log

	for seq in true false
	do

		for dataType in 1 2 3
                do

		for serialType in 1 2 3 4 5 6
                do


                echo 3 > /proc/sys/vm/drop_caches && sync
	        sleep 3
		echo "Reading $num wihth seq $seq in Run number  $counter "
		echo " data type is $dataType serial $serialType"

time taskset -c 0 java -XX:-UseGCOverheadLimit  -Xms15g -Xmx30g   -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.experiments.Experiment_1_4_AccessDataFromLocalDisk    $num $dataType $seq $serialType
# java  -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -d64  -Xms15g -Xmx30g   -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.experiments.Experiment_1_4_AccessDataFromLocalDisk    $num $dataType $seq $serialType

		done
		done



	echo " Seq $seq "

	done


  cat loging.log  >  resultJavaReadObjects_"$num"_"$counter".txt

  done

done
