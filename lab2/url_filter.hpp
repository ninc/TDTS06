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
private:
  vector<string> key_words;
  void sort_header(string http);
  map<string, string> header;

};
#endif
