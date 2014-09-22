#ifndef URL_FILTER_HPP
#define URL_FILTER_HPP
#include <iostream>
#include <string>
#include <vector>
#include "socket_server.hpp"

using namespace std;
class url_filter{
public:
  url_filter(socket_server *socket_server);
  ~url_filter();
  int start(string http_request);
  vector<string> key_words;
private:
  socket_server *m_sr;
  string m_filter(string http);
  string sort_header(string http);
  bool check_url(string url);
  string build_request(string request_header);

  string url;
  string request;
  string host_name;
  vector<string> first_url_part;
  vector<string> second_url_part;
  bool url_redirect;
  
};
#endif
