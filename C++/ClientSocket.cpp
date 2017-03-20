// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>

ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }
}






long ClientSocket::write(char * buffer, long contentSize) {
  long status = Socket::write (buffer, contentSize);
  if (!status) {
      throw SocketException ( "Could not write to socket." );
  }
  else if(status != contentSize) {
      throw SocketException ( "ClientSocket: Mismatch in write()" );
  }

  return status;
}

long ClientSocket::read(char * buffer, long contentSize) {
  long status = Socket::read (buffer, contentSize);
  if (!status) {
      throw SocketException ( "Could not read to socket." );
  }
  else if(status != contentSize) {
      throw SocketException ( "ClientSocket: Mismatch in read()" );
  }

  return status;
} 
