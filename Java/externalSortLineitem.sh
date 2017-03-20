#!/bin/bash
# svn up

# mvn clean compile assembly:single

rm -rf loging.log

echo "language#method#seq#datatype#iotime#indextime#totaltime"  > loging.log 


rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time taskset -c 0 java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.localSort.ExternalSort  30  4000000  3  1


rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time taskset -c 0 java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.localSort.ExternalSort  20  6000000  3  2



rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time taskset -c 0 java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.localSort.ExternalSort  16  7500000  3  3



rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time taskset -c 0  java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.localSort.ExternalSort  12  10000000  3  4


rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time taskset -c 0 java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.localSort.ExternalSort  12  10000000  3  5


rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time taskset -c 0  java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.localSort.ExternalSort  12  10000000  3  6


cat loging.log  >  result_JAVA_ExternalSort-LineItem.txt
