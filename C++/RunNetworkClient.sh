#!/bin/bash

#$1 = Port Number, $2 = Serialization Type

for dataType in 0 1 2
do

if [ $dataType = "0" ]; then
   loopFrom=10000
   loopTo=10000000 
fi

if [ $dataType = "1" ]; then
   loopFrom=10000
   loopTo=1000000
fi

if [ $dataType = "2" ]; then
   loopFrom=1000
   loopTo=100000
fi



for fromDisk in false true
do


for ((num=$loopFrom;  num<= $loopTo ;num=num*10));
do

for x in 1 2 3 4 5
do 

sleep 5;
echo 3 >  /proc/sys/vm/drop_caches; 
sleep 5;

echo "Run = $x Serialization = $2 dataType = $dataType  fromdisk = $fromDisk "


if [ "$fromDisk" == "false" ]; then

taskset -c 0 ./NetworkClientFromRAM localhost $1 $dataType $num 

else 

taskset -c 0 ./NetworkClientFromDisk localhost $1 $dataType $num 

fi


if [ "$num" -gt "1000000" ]; then

sleep 60


else 

sleep 30

fi



done
done
done
done
# done
