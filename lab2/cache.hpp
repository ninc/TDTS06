#ifndef CACHE_HPP
#define CACHE_HPP
#include <iostream>
#include <string>
#include <unordered_map>
#include "url_filter.hpp"

using namespace std;
class cache{
public:
  cache(url_filter *url_filter);
  ~cache();
  string start(string url, string http_request, string host_name, bool url_red);
private:
  string handle_request(string request, string host_url, string host);

  unordered_map<string, string> table;
  url_filter *uf;

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
