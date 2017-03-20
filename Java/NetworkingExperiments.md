Networking Experiments
----------------------





Server Startup 
--------------


Not forwarding to other port, and only collect on the node. 

java    -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.serial.networking.SocketServer 5454 2 2000 5 2 false





Client Startup 
--------------

java  -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.serial.networking.SocketClient localhost 5454 1000 5 2 false 



And if Data should be sent from RAM. 
java  -cp  ./target/PDBSerialization-1.0-SNAPSHOT-jar-with-dependencies.jar edu.rice.pdb.serial.networking.SocketClient localhost 5454 1000 5 2 false 


