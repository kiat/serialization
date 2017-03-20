# Fresh Compilation:
make readExperiment;
sleep 5;

# Cleanup cache:
echo 3 > /proc/sys/vm/drop_caches;
sleep 5;


# Number of runs (k): Run each experiment 5 times:
for k in 1 2 3 4 5;
do

#Number of objects to read:
for i in 10 100 1000 10000 100000; 
do 

echo "Run: " $k " NumObjects: " $i;

foo="results_"$i"_"$k".txt";
echo "language#method#seq#datatype#iotime#totaltime" >> $foo;

# Which serialization type?
for j in 0 1 2 3; 
do 

#Object Type:
for m in 0 1 2
do

# Read Random or Sequential:
for n in 0 1
do

./a.out $i $j $m $n >> $foo;
sleep 5;
echo 3 > /proc/sys/vm/drop_caches;
sleep 5;

done;

done;

done; 

done;

done;