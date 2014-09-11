#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iomanip>

using namespace std;
class socket_client{
public:
  socket_client(string u, string message, int port);
  socket_client(string u, string message);
  ~socket_client();
  string start();
private:
  int m_socket();
  int m_connect();
  int m_send();
  int m_recv();
  string portno;
  string msg;
  string url;
  int sockfd;
  struct addrinfo hints;
  struct addrinfo *result;
  static const int BUFFER_SIZE = 1023;
  char recv_buffer[BUFFER_SIZE];
};
#endif
