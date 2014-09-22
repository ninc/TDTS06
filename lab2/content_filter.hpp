#ifndef CONTENT_FILTER_HPP
#define CONTENT_FILTER_HPP
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class content_filter{
public:
  content_filter();
  content_filter(vector<string> *keys);
  ~content_filter();
  bool start(string http_response);
private:
  string filter(string http);
  vector<string> *key_words;
  
};
#endif
