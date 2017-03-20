#!/bin/bash

for dataType in 0 1 2 
do

if [ $dataType = "0" ]; then
   loopFrom=100000
   loopTo=100000000 
fi

if [ $dataType = "1" ]; then
   loopFrom=100000
   loopTo=10000000
fi

if [ $dataType = "2" ]; then
   loopFrom=10000
   loopTo=1000000
fi



for fromDisk in false true 
do

for ((num=$loopFrom;  num<= $loopTo ;num=num*10));
do

for x in 1 2 3 4 5
do 

echo "Run $x  dataType = $dataType  fromdisk = $fromDisk "
./NetworkServer 10 $dataType

rm Client*.out;
echo 3 > /proc/sys/vm/drop_caches;

done

done
done
done

