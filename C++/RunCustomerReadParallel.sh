#Create data generator:
make smartdatagen;
sleep 5;

# Fresh Compilation:
make parallelReadExperiment;
sleep 5;

# Cleanup cache:
echo 3 > /proc/sys/vm/drop_caches;
sleep 5;


foo="results_parallel_reads.out";



#Customer only:
# Number of runs (k): Run each experiment 5 times:
for k in 1;
do

# Which serialization type?
for j in 0 1 2 3; 
do 

#Object Type: Customer
for m in 2 
do

echo "Generating data: Serialization Type: " $j " Object Type: " $m;
# Cleanup previous data:
make clean;
sleep 5;

# Generate data:
./smartdatagen $j $m
sleep 5;

# Cleanup cache:
echo 3 > /proc/sys/vm/drop_caches;
sleep 5;

#Number of objects to read:
for i in 100 1000 10000; 
do 

# Run Experiments:
#echo ./a.out $i $j $m;
./a.out $i $j $m >> $foo;
sleep 5;

#Cleanup cache:
echo 3 > /proc/sys/vm/drop_caches;
sleep 5;

done;

done; 

done;

done;