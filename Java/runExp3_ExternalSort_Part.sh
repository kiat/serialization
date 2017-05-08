#!/bin/bash
# svn up

# mvn clean compile assembly:single

rm -rf loging.log

echo "language#method#seq#datatype#iotime#indextime#totaltime"  > loging.log 



# rm -f tmp/*
# delete OS Cache
# echo 3 > /proc/sys/vm/drop_caches && sync
# time  taskset -c 0  java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.sort.ExternalSort    12    20000000    2  6




rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time  taskset -c 0  java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.sort.ExternalSort    10    24000000    2  5


# rm -f tmp/*
# delete OS Cache
# echo 3 > /proc/sys/vm/drop_caches && sync
# time  taskset -c 0  java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.sort.ExternalSort    10    24000000    2  4



rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time  taskset -c 0  java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.sort.ExternalSort    20    12000000    2  3




rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time  taskset -c 0  java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.sort.ExternalSort    30	  8000000     2  2



# delete old tmp files 
rm -f tmp/*
# delete OS Cache
echo 3 > /proc/sys/vm/drop_caches && sync
time   taskset -c 0  java   -XX:-UseGCOverheadLimit -XX:+UseConcMarkSweepGC -Xms15g -Xmx30g -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.sort.ExternalSort    20   12000000    2  1



cat loging.log  >  result_JAVA_ExternalSort-LineItem-$method.txt
# 1 Java JSON GZIP        20	12000000
# 2 Java Default          30	8000000   
# 3 Java BSON             20	12000000 
# 4 Java Protocol Buffer  10	24000000
# 5 Java Kryo  	          10	24000000
# 6 Java Hand Coded ByteBuffer  12	20000000








