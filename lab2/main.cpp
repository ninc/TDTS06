#include "socket_server.hpp"


int main( int argc, const char* argv[] )
{
  int k = argc;
  if(k > 0)
    {
      cout << argv[0] << endl;
      cout << "Starting Mädz super proxy" << endl;

      socket_server ss = socket_server(6060);

      ss.start();
    }
  return 0;
}
