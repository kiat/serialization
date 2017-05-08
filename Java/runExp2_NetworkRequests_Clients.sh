#!/bin/bash

for dataType in 2 3 4 
do

if [ $dataType = "2" ]; then
   loopFrom=10000
   loopTo=10000000 
fi

if [ $dataType = "3" ]; then
   loopFrom=10000
   loopTo=1000000
fi

if [ $dataType = "4" ]; then
   loopFrom=1000
   loopTo=100000
fi



for fromDisk in false true
do


for ((num=$loopFrom;  num<= $loopTo ;num=num*10));
do

for x in {1..2}
do 

echo 3 >  /proc/sys/vm/drop_caches  

echo "$x $1 $dataType  fromdisk = $fromDisk "
 
taskset -c 0 java -Xms16g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.SocketClient 54.167.67.207 5454  $num  $1  $dataType  $fromDisk false 


if [ "$num" -gt "1000000" ]; then

sleep 200

else 

sleep 100

fi



done

done

done

done
