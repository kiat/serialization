datagen:
	g++ -O3 -std=c++11 DataGenerator.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp ElementProto.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread  -lboost_serialization

smartdatagen:
	g++ -O3 -std=c++11 -o smartdatagen NewDataGenerator.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp ElementProto.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread  -lboost_serialization

clean:
	rm -rf *.txt	

parallelReadExperiment:
	g++ -O3 -std=c++11 TestParallelReads.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp ElementProto.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread  -lboost_serialization

readExperiment:
	g++ -O3 -std=c++11 TestReadObjects.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp ElementProto.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread  -lboost_serialization

NetworkServer:
	g++ -O3 -std=c++11 -o NetworkServer NetworkServer.cpp ServerSocket.cpp Socket.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp ElementProto.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread  -lboost_serialization

NetworkClientFromDisk:
	g++ -O3 -std=c++11 -o NetworkClientFromDisk NetworkClientFromDisk.cpp ClientSocket.cpp Socket.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp ElementProto.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread  -lboost_serialization

NetworkClientFromRAM:
	g++ -O3 -std=c++11 -o NetworkClientFromRAM NetworkClientFromRAM.cpp ClientSocket.cpp Socket.cpp RootData.cpp Element.cpp Part.cpp Supplier.cpp LineItem.cpp Order.cpp Customer.cpp ElementBoost.cpp PartBoost.cpp SupplierBoost.cpp LineItemBoost.cpp OrderBoost.cpp CustomerBoost.cpp ElementProto.cpp PartProto.cpp SupplierProto.cpp LineItemProto.cpp OrderProto.cpp CustomerProto.cpp Object.cpp ElementIP.cpp PartIP.cpp SupplierIP.cpp LineItemIP.cpp OrderIP.cpp CustomerIP.cpp CustomerP.pb.cc OrderP.pb.cc LineItemP.pb.cc SupplierP.pb.cc PartP.pb.cc ElementP.pb.cc -pthread -lprotobuf -pthread -lpthread  -lboost_serialization

NetworkClean:
	rm -rf NetworkServer NetworkClientFromDisk NetworkClientFromRAM

NetworkAggregator:
	g++ -O3 -std=c++11 -o NetworkAggregator NetworkAggregator.cpp SparseVectors.cpp ClientSocket.cpp ServerSocket.cpp Socket.cpp RootData.cpp Element.cpp Object.cpp ElementIP.cpp ElementBoost.cpp ElementProto.cpp ElementP.pb.cc -pthread -lprotobuf -lpthread  -lboost_serialization
