# Experiments on Complex Object Models and Serialization with C++ 


   g++ -std=c++11 TestObjectModelMain.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp Order.cpp LineItem.cpp Customer.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc LineItemP.pb.cc OrderP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread -lboost_serialization


   g++ -std=c++11 TestHandCodedObjects.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp


   g++ -std=c++11 TestProtoObjects.cpp RootData.cpp ElementProto.cpp ElementP.pb.cc PartProto.cpp PartP.pb.cc SupplierProto.cpp SupplierP.pb.cc LineItemProto.cpp LineItemP.pb.cc OrderProto.cpp OrderP.pb.cc CustomerProto.cpp CustomerP.pb.cc -pthread -lprotobuf


  g++ -std=c++11 TestBoostObjects.cpp RootData.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp -pthread -lpthread -lboost_serialization  



  g++ -std=c++11 TestInPlaceObjects.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp


# HC + IP:
  g++ -std=c++11 TestHandCodedObjects.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp




  valgrind --leak-check=yes ./a.out 


   g++ -O3 -std=c++11 DataGenerator.cpp RootData.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc  -pthread -lprotobuf -pthread -lpthread  -lboost_serialization



   g++ -O3 -std=c++11 TestReadObjects.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp ElementProto.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread  -lboost_serialization


   g++ -O3 -std=c++11 DataGenerator.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp ElementProto.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread  -lboost_serialization



   g++ -O3 -std=c++11 TestElements.cpp RootData.cpp Object.cpp Element.cpp  ElementIP.cpp ElementProto.cpp ElementP.pb.cc -pthread -lprotobuf -lpthread  -lboost_serialization




# SparseVector:
  g++ -O3 -std=c++11 SparseVectors.cpp RootData.cpp Object.cpp Element.cpp ElementIP.cpp ElementBoost.cpp ElementProto.cpp ElementP.pb.cc -pthread -lprotobuf -lpthread  -lboost_serialization


  g++ -O3 -std=c++11 -o NetworkAggregator NetworkAggregator.cpp SparseVectors.cpp ClientSocket.cpp ServerSocket.cpp Socket.cpp RootData.cpp Element.cpp Object.cpp ElementIP.cpp ElementBoost.cpp ElementProto.cpp ElementP.pb.cc -pthread -lprotobuf -lpthread  -lboost_serialization


  g++ -O3 -std=c++11 TestElements.cpp SparseVectors.cpp RootData.cpp Object.cpp Element.cpp ElementIP.cpp ElementBoost.cpp ElementProto.cpp ElementP.pb.cc -pthread -lprotobuf -lpthread  -lboost_serialization
