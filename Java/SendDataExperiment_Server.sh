
#!/bin/bash

for dataType in 2 3 4 
do

if [ $dataType = "2" ]; then
   loopFrom=100000
   loopTo=100000000 
fi

if [ $dataType = "3" ]; then
   loopFrom=100000
   loopTo=10000000
fi

if [ $dataType = "4" ]; then
   loopFrom=10000
   loopTo=1000000
fi



for fromDisk in false true 
do

for ((num=$loopFrom;  num<= $loopTo ;num=num*10));
do

for x in {1..2}
do 

echo "$x $1 $dataType  fromdisk = $fromDisk "

rm -rf tmp/* 

java -Xms16g -Xmx30g -cp ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar  edu.rice.pdb.serial.networking.SocketServer 5454 10 $num  $1 $dataType true  $fromDisk  false 


done

done

done

done
