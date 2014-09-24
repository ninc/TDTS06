#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
class socket_server{
public:
  socket_server(int port);
  ~socket_server();
  int start();
  int m_send(string message);
private:
  int m_listen();
  int m_accept();
  int m_bind();
  int m_recv(int sock);
  int m_socket();
  int m_handle_request(int sock);
  struct sockaddr_in m_serv_addr, m_cli_addr;
  int m_portno;
  int m_sockfd;
  int m_newsockfd;
  socklen_t m_clilen;
  string request;

};
#endif
