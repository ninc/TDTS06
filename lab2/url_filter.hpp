#ifndef URL_FILTER_HPP
#define URL_FILTER_HPP
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class url_filter{
public:
  url_filter();
  ~url_filter();
  string start(string http_request);
private:
  string filter(string http);
  string sort_header(string http);
  bool check_url(string url);
  string build_request(string request_header);

  string url;
  string request;
  string host_name;
  vector<string> first_url_part;
  vector<string> second_url_part;
  vector<string> key_words;
  string redirect_url;
  string redirect_get_request;
  
};
#endif
