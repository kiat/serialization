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

This method simply uses built-in Java de/serialization, and serves as the straw man to which we compare the other Java-based methods.  Strings such as Part.comment are implemented using the Java String class.  Types are built up recursively, so that Customer contains an ArrayList of Order objects, for example. Objects are de/serialized from/to a Java ByteArrayOutputStream which then converted to a ByteArray in the case of serialization, and from a ByteArray for deserialization.


## 2. Java objects with Kryo, version 3.0.3. 

Kryo is a common serialization package, used, for example, by Spark. Java Kryo requires the instantiation of a Kryo instance (Kryo myKryo= new Kryo();) and then the various types need to be registered with Kryo (kryo.register(Part.class);) one time at startup. This registration enables fast subsequent de/serialization. Each of our Java classes have a Base class that has the following code to self-serialize:

```Java
public  byte[] kryoSerialization() {
  ByteArrayOutputStream bos =  new ByteArrayOutputStream();
  Output output = new Output(bos);
  KryoSinglton.getInstance().getKryo()
    .writeObject(output, this);
  output.flush();
  byte[] buf = bos.toByteArray();
  return buf;
}
```

Deserialization is implemented similarly.

## 3. Java objects with Protocol Buffers, version 3.0.0-alpha-3.1. 

PBs are quite different from the other two methods, in that they require a text description of the object, which is then compiled into a set of PB classes that can then be used by the programmer (or by the data management/analytics system). For example,

```
message PartP {
  required int32 partID = 1;
  required string name = 2;
  required string mfgr = 3;
  ...
}
```

In practice, the resulting PB classes are typically used directly, without conversion to/from a ``regular'' Java class. To ensure that we could use the same Java code for our experiments, we instead converted the PB classes into our own Java classes after deserialization (as we will describe subsequently, the cost of doing this is negligible).  Each of our Java Classes has toProtocolBuffer() method that converts a Java class to a byte array. 
First we need to instantiate a PB builder that can be used for serialization:  
```
PartP.Builder m\_part = PartP.newBuilder();
```

Then we need to access our object attributes and set them to the PB own object; for example:
```Java
m_part.setPartID(this.partID);
m_part.setName(this.name);
m_part.setBrand(this.brand);
...
```

And then we use again a ByteArrayOutputStream and write the serialized data into it and convert it to a byte array:

```
ByteArrayOutputStream bos =  new ByteArrayOutputStream();
m_part.build().writeTo(bos);
```


To transform a PB object to our own Java class during deserialization, we need to create a new  empty object, then build a PB object and deserialize data to PB object, and finally call the getter and 
setter methods to set the content of the our empty object and return it.  For example,

```Java
Part protocolBufferRead(byte[] buf) {
  Part m_part = new Part();
  ByteArrayInputStream b = new ByteArrayInputStream(buf);
  PartP.Builder protocPart = ProtocolBufferSinglton.getInstance().getPart();
  CodedInputStream stream = CodedInputStream.newInstance(b);
  m_part.setName(protocPart.getName());
  m_part.setBrand(protocPart.getBrand());
  ....
  return m_part; 
}
```


Since conversion between our Java implementation and the PB representation is, strictly speaking, unnecessary, we wanted to make sure that this did not introduce any additional cost.  Through a series of micro-experiments, we found that conversion was costless.  For example, we found that deserialization of a million Part objects without conversion (that is, simply calling all of the PB getter methods) took 1.185 seconds.  Doing the same thing, converting into Java objects, took 1.183 seconds.  This is not surprising; conversion into a Part object requires only a single additional Java allocation (a new Part object). Populating this object requires only simple Java Object reference assignments, or primitive type assignments.

# 4. Java objects with hand-coded using Java ByteBuffer

We also hand-coded our own Java Object serialization.  This should serve as something of an upper bound on the performance of any Java method that does not use the Java **unsafe** interface (which has perennially faced removal with each subsequent release of Java).  All hand-coded serialization directly encodes the various data structures by writing bytes, ints, and doubles into a Java *byteBuffer*.  In our implementation, to serialize an Object, we first convert all strings to byte arrays.

After this, we compute the size of the object and allocate a *ByteBuffer* with that size. After this step, write all bytes, in order into the buffer. Strings are pre-prended with their size, in bytes.  

We then convert this ByteBuffer to a byte array. Deserialization reverses this process. Here is an example serialization code:

The following Java code snippet illustrates our process for the handcoded serialization in Java.  

```Java
byte[] commentBytes =  comment.getBytes(Charset.forName("UTF-8"));
ByteBuffer byteBuffer =  ByteBuffer.allocate(40 + ... +  commentBytes.length);
byteBuffer.putInt(partID); 
byteBuffer.putInt(size); 
byteBuffer.putDouble(retailPrice); 
... 
byteBuffer.putInt(commentBytes.length);
byteBuffer.put(commentBytes);
return byteBuffer.array();
```

# 5. Java JSON objects compressed using GZip

JSON is a common interchange format, often associated with document databases.

In our implementation, we use standard Java as the in-memory representation, and then use JSON purely as in interchange and storage format.  This is required since performing the computations required by our experiments directly on a *javax.json.JsonObject* object is not possible.

An example of the text associated with a JSON *Part* object is:

```JSON
{"partid":6038,
 "name":"dim almond red burnished lace",
 "mfgr":"Manufacturer#2",
 "brand":"Brand#24",
 "type":"MEDIUM ANODIZED TIN",
 "container":"WRAP PKG",
 "retailprice":944.03,
 "comment":" deposits."
}
```

For the Java JSON serialization we use the *javax.json* package. We use the *JsonObjectBuilder* to create a JSON object in Java and the *JsonWriterFactory* to write the JSON object properties to the *JsonObjectBuilder*, 
and then use *JsonWriter* to  write the created JSON object into an a *ByteArrayOutput*  *Stream*. 

To compress the output array we first use the *JsonWriter* to write the output stream to a *GZIPOutputStream* which is initialized with a *ByteArrayOutputStream*. At the end we can convert the *ByteArrayOutputStream* to a byte array. 

In the case of class that include lists of sub-objects, we need to have separate *JsonObjectBuilder*, and use a *for* loop to iterate over the list, and generate each of the sub-objects, which are added to the parent
JSON object.

The deserialization process is the inverse of the serialization process.  We first obtain an array bytes, and  use *GZIPInputStream* and *ByteArrayInputStream* and convert the input stream to a *JsonReader* and then to 
a *JsonObject*, which is then converted to a *Part*, *Lineitem* or *Customer* type by using the JSON object's getter/setter methods to get the JsonObject content. 


# 6. Java objects with BSON, *bson4jackson* version 2.7.0 and the *FasterXML/jackson* packages.

Again, we use standard Java objects as our in-memory representation.

To serialize those objects, we first initialize a *ByteArrayOutput}\-*Stream} 
and access a *de.undercouch.bson4jackson* *.Bson* *Factory* which is set to our output stream and create 
a *JSONGener*-*ator* of type *com.fasterxml.jackson.core.JsonGener.ator* 

Then we access the object attributes using the getter methods and write them to *JsonGenerator*. The *BsonFactory* coverts this JSON to BSON format and writes it to the output stream. For each attribute of the object we first write a field name and then write values, is in the following Java code snippet:

```Java
ByteArrayOutputStream baos =  new ByteArrayOutputStream();
com.fasterxml.jackson.core.JsonGenerator gen  = BsonSinglton.getInstance()
.getFactory().createJsonGenerator(baos);

gen.writeStartObject();
gen.writeFieldName("partid");
gen.writeNumber(this.getPartID());
gen.writeFieldName("name");
gen.writeString(this.getName());
...
return baos.toByteArray();
```

To avoid re-instantiation of *BsonFactory* objects we implemented the singleton pattern. 

The deserialization process starts with with conversion of a byte array to an input stream,
and then we use *JsonParser} and *Bson  *Factory* to convert the stream to a parser and access its tokens. We 
create a new object of the requested object type and use the parser's getters to access the various 
fields (*parser.getIntValue()*, *parser.getText()*, etc.).
This is given in the following Java code snippet:

```JAVA
byte[] buf;
ByteArrayInputStream bais =  new ByteArrayInputStream(buf);
Part tmp = new Part();
com.fasterxml.jackson.core.JsonParser 
  parser = BsonSinglton.getInstance().
  getFactory().createJsonParser(bais);
parser.nextToken();

while (parser.nextToken() != JsonToken.END_OBJECT) {
  String fieldname = parser.getCurrentName();
  parser.nextToken();
  switch (fieldname) {
	  case "partid":
	  	tmp.setPartID(parser.getIntValue());
		break;
	  case "name":
		tmp.setName(parser.getText());
		 break;
  ...
  }
return tmp;
```
        

For the serializaiton of this kind of complex, recursive objects, the *bson4jackson} package 
provides an API (the *ObjectMapper} API) 
that iterates over a list of objects and serializes them.


# 7. C++ objects with BOOST serialization/deserialization, version 1.59. 

BOOST is the classical C++ serialization package, and one of the options that we consider.  
To use BOOST, we simply design C++ classes for each of the various data types, using primitive types such as *int}, *double}, and 
the STL type *string}.  To represent collections of sub-objects, we use the STL vector.
All of those types can be automatically serialized by BOOST.

One challenge with BOOST was providing random access to objects on a page, without deserializing the entire page.
For de/serializing a stream of objects. Boost provides *archives} that can be used as de/serializer, bound to a C++ stream (either a stringstream or a fstream). 
During serialization, boost archive prepends some internal information (like a small header) at the beginning of the stream. The deserializer archive needs to parse this header first, before it can parse the stream of objects.

```C++
// Serialization
stringstream ss = new stringstream();
boost::archive::binary_oarchive oa(ss);
oa<<Object_1<<Object_2<<...<<Object_n;

// Deserialization
stringstream ss = new stringstream();
boost::archive::binary_iarchive ia(ss);
ia>>Object_1>>Object_2>>...>>Object_n;
```

In our data aggregation experiment, since we de/serialize an entire sparse vector at once, we use BOOST in this way.  Each entry in the sparse vector is stored
as a separate object, with a single header for all objects.
However, for other experiments, we need random access to objects (to perform random reads of objects or to perform sequential reads of objects starting
from a random location on a page). Hence, we serialize each object with a separate instance of boost archive. 
Thus, a small header is appended at the beginning of each object.  The downside of this is that it increased the average object size and 
adds some extra overhead to processing each each object; the overhead is higher, relatively speaking, for smaller objects. 
In a sense, this arises from the fact that we are using BOOST in a way that it was not intended to be used: for random access to individual objects in a large page.


# 8. C++ objects with hand-coded serialization/deserialization

Since we hand-code de/serialization when implementing this option, the choice should be considered to provide an upper-bound on
the performance of any C++-based method that requires serialization/
deserialization (note, however, that C++ in-place objects *do not} require de/serialization, and hence may have better performance).

The encoding of data was nearly identical to the encoding used for BOOST:
*int*, *double*, *vector*, etc.
The difference is that de/serialization was implemented by hand.  
The primary key-foreign key relationships in the TPC-H relational diagram 
are implemented as object pointers/references. 

For example, a *Lineitem* object has a *Part* and a *Supplier*:

```C++
class LineItem  {
        // Primitive variables:
        // ...
        // Object types:
        Supplier *supplier;
        Part *part;
}
```

An order object has a collection of lineitems. 
To represent collections we use the STL vector:

```C++
class Order {
        // Primitive variables:
        // ...
        // collection of lineitems:        
        vector<LineItem*> lineItems;
}
```

During serialization, primitive types are written directly to the output page.  *std::string} objects are serialized by first recording the length
of the string, followed by the characters.  Serializing object pointers or collections of pointers to objects requires a recursive 
serialization.  

# 9. C++ objects with Protocol Buffers, version 2.6.1. 

Protocol buffers have been described above, and the C++ implementation does not differ significantly from the Java implementation. 
However, in the case of C++, we do not translate into a separate representation.  The C++ PB classes are simple wrappers over the PB compiler generated classes.


# 10. In-Place C++ objects
We have previously described the philosophy behind in-place objects in detail.  In practice, in-place objects rely on C++ to pack primitive types 
within a contiguous region of memory.  *offset_ptr*s are used to implement character strings and arrays of sub-objects.  For example, we have:

```C++
class CustomerIP : public Object {
private:
  // Primitive variables: 
  int numOrderSupplied;
  int custKey;        
  int nationKey;
  double accbal;
        
  // Strings:
  offset_ptr <char> name;
  offset_ptr <char> address;
  offset_ptr <char> phone;
  offset_ptr <char> mktsegment;
  offset_ptr <char> comment;
                 
  // Collection of orders:
  offset_ptr <OrderIP> orders;
};
```

During object creation, allocation is performed using a special, in-place memory manager that allocates objects directly to a page.
This is accessed via a custom, templated *malloc} function.
For example, to initialize the *name* field from a C++ *std::string*, we have:

```
this->name =  malloc <char> (strlen (name.c_str ()) + 1);
strcpy (this->name, name.c_str ());
```

Since the *Object} base class over-rides *new}, collections (arrays) of objects are allocated in-place
using the C++ *new} operator:


```C++
// Size of the order vector:
this->numOrderSupplied = numOrderSupplied;
        
// Dynamically create offset_ptr's
this->orders = new OrderIP [numOrderSupplied];
        
// Create orders using InPlace memory manager
for (int i = 0; i < numOrderSupplied; i++) {
  (this->orders)[i].setUp(...);
}    
```
