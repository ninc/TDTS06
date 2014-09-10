#include <stdio.h>
#include "socket_server.hpp"
int main( int argc, const char* argv[] )
{
  int k = argc;
  if(k > 0)
    {
      printf(argv[0]);
      printf( "\nHello World\n\n" );

      socket_server ss = socket_server(6060);

      ss.start();
    }
  return 0;
}
