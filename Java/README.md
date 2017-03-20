Experiments on Complex Object Models and Serialization with Java 




This project contains our java code for experimenting the different java serialization techniques. 

# Install maven - compile 
mvn clean compile assembly:single
 
# run experiments 

 


google protocol buffer commands 
--------------------------------

It is needed to install protocbuf package in Ubuntu system.
To run the Protocol Buffer compuler and generate java class, 
 
protoc -I=./protos --java_out=./src/main/java/  protos/Part.proto







---------------

To run all of the experiments run the following shell scripts on a linux machine with 32GB RAM

# ./writefiles.sh
# ./readFiles.sh



Here are the various alternatives I'd suggest we look at:

1) (Java) over the wire: JSON, in memory: javax.json
2) (Java) over the wire: Java serialization, in memory: Java objects
3) (Java) over the wire: Protocol buffers, in memory: Java objects
4) (C++) over the wire: Protocol buffers, in memory: C++ objects
5) (C++) over the wire: Hand-coded serialization, in memory: C++ objects
6) (C++) our own PDB solution

And here's the tasks we might consider:

1) Randomly access N objects from across a local disk
2) In a cluster of 10 machines, access N objects from each machine's local disk, ship to a central location
3) In a cluster of 10 machines, access N objects from each machine's RAM, ship to a central location
4) Access N objects sequentially from a local disk
5) Do an external sort of 100GB of data on a local disk 


Serialization Methods
----------------------

# Java Default
# JSON
javax.json 

# BSON
https://github.com/michel-kraemer/bson4jackson

https://docs.mongodb.com/ecosystem/drivers/java/

# google protocol buffers
https://developers.google.com/protocol-buffers/

# Protostuff

https://github.com/protostuff/protostuff

# Kryo 

https://github.com/EsotericSoftware/kryo 


# Avro 

http://avro.apache.org/

https://github.com/phunt/avro-rpc-quickstart


# Thrift 

http://thrift.apache.org/





Interesting for C++
----------------------

https://github.com/mongodb/libbson



https://avro.apache.org/docs/1.7.0/api/cpp/html/index.html





