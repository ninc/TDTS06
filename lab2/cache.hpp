#ifndef CACHE_HPP
#define CACHE_HPP
#include <iostream>
#include <string>
#include <unordered_map>
#include "url_filter.hpp"
#include "socket_server.hpp"
#include "socket_client.hpp"

using namespace std;
class cache{
public:
  cache(url_filter *url_filter, socket_server *sr);
  ~cache();
  int start(string url, string http_request, string host_name, bool url_red);
private:
  int handle_request(string request, string host_url, string host);
  int socket_stream(char *recv_buffer, char *msg_buffer, int recv_s);
  int check_response_type(char *recv_buffer, char *msg_buffer);
  string recv_all(char *recv_buffer, char *msg_buffer, int recv_s);


  int m_text;
  unordered_map<string, string> table;
  url_filter *uf;
  socket_server *sr;
  socket_client *sc;
  static const int BUFFER_SIZE = 100000;

  struct request_info
  {
    string url;
    string host_name;
    string request;
  } url_redirect, content_redirect;

  //struct request_info url_redirect;
  //struct request_info content_redirect;
};
#endif
