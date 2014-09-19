#ifndef URL_FILTER_HPP
#define URL_FILTER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
class url_filter{
public:
  url_filter();
  ~url_filter();
  string filter(string http);
  string build_request();
  bool check_url(string url);
private:
  vector<string> key_words;
  string sort_header(string http);
  //map<string, string> header;
  vector<string> first_url_part;
  vector<string> second_url_part;
  string redirect_url;
  string redirect_get_request;
  
};
#endif
