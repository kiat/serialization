# An Experimental Comparison of Complex Object Implementations for Big Data Systems



# Java
Java Implementation is in Directory ./Java


# C++ 
C++ Implementation is in Direcotry ./C++ 
Directory ./C++



# TPC-H Benchmark
http://www.tpc.org/tpch/

Our Experimental Dataset is in Directory ./tables_scale_0.1


# Methods Tested

Here, we describe in detail the various implementations tested in this paper.

## 1. Java objects with Java de/serialization
This method simply uses built-in Java de/serialization, and serves as the straw man to which we compare the other
Java-based methods.  Strings such as Part.comment are implemented using the
Java String class.  Types are built up recursively, so that Customer contains an ArrayList of
Order objects, for example.
Objects are de/serialized from/to a Java ByteArrayOutputStream which then converted to a ByteArray in the
case of serialization, and from a ByteArray for deserialization.


## 2. Java objects with Kryo, version 3.0.3. 
Kryo is a common serialization package, used, for example, by Spark.
Java Kryo requires the instantiation of a Kryo instance (Kryo myKryo= new Kryo();) and then the various types need
to be registered with Kryo (kryo.register(Part.class);) one time at startup.  
This registration enables fast subsequent de/serialization.
Each of our Java classes have a Base class that has the following code to self-serialize:

```Java
public  byte[] kryoSerialization() {
  ByteArrayOutputStream bos =  new ByteArrayOutputStream();
  Output output = new Output(bos);
  KryoSinglton.getInstance().getKryo().writeObject(output, this);
  output.flush();
  byte[] buf = bos.toByteArray();
  return buf;
}
```
