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
  int start();
  int m_recv(char *recv_buffer);
  int m_recv_header(char *recv_buffer);
  int m_close();
  void set_host(string host);
  void set_request(string request);

private:
  int m_socket(struct addrinfo *result);
  int m_connect(struct addrinfo *result);
  int m_send(string message);
  string portno;
  string msg;
  string host;
  int sockfd;
  struct addrinfo hint;
  struct addrinfo result;


};
#endif
