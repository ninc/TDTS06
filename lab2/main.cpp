#include "socket_server.hpp"


int main( int argc, const char* argv[] )
{
  int k = argc;

  if(k >= 2)
    {
      int port = atoi(argv[1]);
      cout << "Starting Mädz super proxy (MSP) on port: " << port << endl;

      socket_server ss = socket_server(port);

      ss.start();
    }
  else
    {
      // If port was not provided
      fprintf(stderr,"ERROR, no port provided\n");
      cout << "Try: ./msp 6060" << endl;
      exit(1);
    }

  return 0;
}
