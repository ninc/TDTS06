#ifndef CACHE_HPP
#define CACHE_HPP
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;
class cache{
public:
  cache();
  ~cache();
  string start(string url, string http_request, string host_name);
private:
  unordered_map<string, string> table;
};
#endif
