#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <string>

using namespace std;

enum cache_type {LRU, TTL};

struct request
{
  int time_stamp;
  string client_id;
  int video_duration;
  string server_name;
  string file_name;
  int file_size;
  int priority;
};

class cache{
public:
  cache();
  ~cache();
private:

};
#endif
