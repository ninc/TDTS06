#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
class socket_client{
public:
  socket_client(int port);
  ~socket_client();
  int start();
private:
  int m_getaddrinfo();
  int m_socket();
  int m_bind();
  int m_connect(int sock);
  int m_send(int sock, string message);
  int m_recv(int sock);
  int m_handle(int sock);
  struct sockaddr_in serv_addr, cli_addr;
  int portno;
  int sockfd;
  int newsockfd;
  socklen_t clilen;
  static const int BUFFER_SIZE = 1023;
  char recv_buffer[BUFFER_SIZE];
};
#endif
