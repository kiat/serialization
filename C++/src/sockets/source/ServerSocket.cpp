// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"


ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }
}

ServerSocket::~ServerSocket()
{
}





long ServerSocket::write(char * buffer, long contentSize) {
  long status = Socket::write (buffer, contentSize);
  if (!status) {
      throw SocketException ( "Could not write to socket." );
  }
  else if(status != contentSize) {
      throw SocketException ( "ServerSocket: Mismatch in write()" );
  }

  return status;
}

long ServerSocket::read(char * buffer, long contentSize) {
  long status = Socket::read (buffer, contentSize);
  if (!status) {
      throw SocketException ( "Could not read to socket." );
  }
  else if(status != contentSize) {
      throw SocketException ( "ServerSocket: Mismatch in read()" );
  }

  return status;
} 




void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Could not accept socket." );
    }
}
