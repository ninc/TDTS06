#include "cache.hpp"


using namespace std;
cache::cache(cache_type mode, int x, int y, int t, string stats_file)
  {
  
  cache_mode = mode;
  m_size = x;
  m_priority = y;
  m_time_limit = t;
  m_stats_file = stats_file;


  string start;

  if(mode == LRU)
    {
      start = "LRU";
    }
  else if(mode == TTL)
    {
      start = "TTL";
    }

  //Startup output
  cout << "###############################" << endl
       << "Starting cache with " << start << " scheme" << endl << "Cache_size: " 
       << m_size << endl << "Priority " << m_priority << endl << "Timelimit " 
       << m_time_limit << endl << "Statsfile: " << m_stats_file << endl
       << "###############################" << endl;


}


cache::~cache()
{

}


int cache::new_request(struct request req)
{

  if(cache_mode == LRU)
    {
      new_request_lru(req);
    }
  else if(cache_mode == TTL)
    {
      new_request_ttl(req);
    }

  return 0;
}



int cache::new_request_lru(struct request req)
{

  if(req.time_stamp == 0)
    {
      return 1;
    }


  return 0;
}


int cache::new_request_ttl(struct request req)
{
 if(req.time_stamp == 0)
    {
      return 1;
    }

  return 0;
}
