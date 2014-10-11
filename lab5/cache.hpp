#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <string>

using namespace std;

enum cache_type {LRU, TTL, NONE};

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
  cache(cache_type mode, int x, int y, int t, string stats_file);
  ~cache();
  int new_request(struct request req);
private:
  int new_request_lru(struct request req);
  int new_request_ttl(struct request req);
  cache_type cache_mode;
  int m_time_limit;
  int m_priority;
  int m_size;
  string m_stats_file;

};
#endif
