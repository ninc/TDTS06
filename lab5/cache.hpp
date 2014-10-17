#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

enum cache_type {LRU, TTL, NONE};

struct request
{
  unsigned long int time_stamp;
  string client_id;
  int video_duration;
  string server_name;
  string file_name;
  int file_size;
  int priority;
  int request_rate;
};

class cache{
public:
  cache(cache_type mode, int x, int y, int t, string stats_file);
  ~cache();
  int new_request(struct request req);
  string request_to_string(struct request req);
  string cache_info();
  void create_stats();
  string get_stats();
private:
  int new_request_lru(struct request req);
  int new_request_ttl(struct request req);
  int find_request(struct request req);
  int lru_store_request(struct request req);
  int ttl_store_request(struct request req);
  void lru_update_time_stamp(int pos);
  int find_least_recently_used();
  void add_request_to_stats(struct request req);
  string get_cache_hit();
  string get_cache_miss();

  //Internal variables
  cache_type cache_mode;
  string m_scheme;
  int m_time_limit;
  int m_priority;
  int m_size;
  int m_elements;
  int m_bytes;
  string m_stats_file;
  unsigned long int m_time;
  unsigned long int m_max_time_stamp;

  //STATS
  unsigned int m_requests;
  unsigned int m_cache_requests;
  unsigned int m_cache_hit;
  unsigned int m_cache_miss;
  unsigned int m_unstored;
  string m_request_rates;

  vector<struct request> m_cache;

};
#endif
