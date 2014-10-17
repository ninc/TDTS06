#include "cache.hpp"
#include <sys/time.h>
#include <sstream>
#include <fstream>
#include <climits>

using namespace std;


cache::cache(cache_type mode, int x, int y, int t, string stats_file)
{
  //Initialize class variables
  cache_mode = mode;
  if(x > 0)
    m_size = x;
  else
    m_size = INT_MAX;
  m_priority = y;
  m_time_limit = t;
  m_stats_file = stats_file;
  m_elements = 0;
  m_max_time_stamp = 0;
  m_bytes = 0;
  m_time = 0;


  //STATS
  m_requests = 0;
  m_cache_hit = 0;
  m_cache_miss = 0;
  m_unstored = 0;
  m_cache_requests = 0;
  m_request_rates = "";

  m_scheme = "";

  if(mode == LRU)
    {
      m_scheme = "LRU";
    }
  else if(mode == TTL)
    {
      m_scheme = "TTL";
    }

  //Startup output
  cout << "###############################" << endl
       << "Starting cache with " << m_scheme << " scheme" << endl << "Cache_size: " 
       << m_size << endl << "Priority " << m_priority << endl << "Timelimit " 
       << m_time_limit << endl << "Statsfile: " << m_stats_file << endl
       << "###############################" << endl;


  //Initialize vector
  m_cache.resize(1);

}


cache::~cache()
{

}

//Returns a string with information about the cache
string cache::cache_info()
{
  stringstream ss;

  if(cache_mode == LRU)
    ss << m_elements << " cached requests and " << m_bytes << "/" << m_size << " bytes. ";
  else if(cache_mode == TTL)
    ss << m_elements << " cached requests and " << m_bytes << " bytes. ";

  string info = ss.str();

  return info;
}

//Formats a request struct for output
string cache::request_to_string(struct request req)
{

  stringstream ss;
  
  ss << req.time_stamp << " " << req.client_id << " " << req.video_duration << " " 
     << req.server_name << " " << req.file_name << " " << req.file_size << " " 
     << req.priority << ": ";

  string output = ss.str();

  return output;
}


//Handles a new request
int cache::new_request(struct request req)
{


  cout << request_to_string(req);

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

//Searches for the request and returns the position if found
int cache::find_request(struct request req)
{

  struct request it;
  for(unsigned int i = 0; i<m_cache.size(); i++)
    {
      it = m_cache[i];

      //If its a cache match
      if(it.video_duration == req.video_duration)
	{
	  if(it.server_name.compare(req.server_name) == 0)
	    {
	      if(it.file_name.compare(req.file_name) == 0)
	      {
	      if(it.file_size == req.file_size)
		{
		  return i;
		}
	      }
	    }
	}
    }

  return -1;
}

//Store the request in the cache according to LRU
int cache::lru_store_request(struct request req)
{

  int file_size = req.file_size;
  int pos = 0;

  //If cache is not full
  if(m_size >= m_bytes + file_size)
    {
      //If first cacheline, dont increase cache size
      if(m_elements == 0)
	{
	  m_cache[0] = req;
	}
      else
	{
	  m_cache.push_back(req); 
	}

      m_bytes += file_size;
      m_elements++;
      cout << "Adding to cache. " << cache_info();

    }
  else
    {
      //Remove cache lines until the new request fits
      int i = 1;
      do
	{
	  pos = find_least_recently_used();
	  add_request_to_stats(m_cache[pos]);
	  m_bytes -= m_cache[pos].file_size;

	  //Replace a current item
	  if(m_size >= m_bytes + file_size)
	    {
	      m_bytes += req.file_size;
	      cout << "Replacing " << i << " requests to make room. " << cache_info();
	      m_cache[pos] = req;
	      break;
	    }
	  //Remove a request to make room
	  else
	    {
	      m_cache.erase(m_cache.begin()+pos);
	      m_elements--;
	    }

	  i++;
	}while(true);
    }

  return 0;
}

//Update the time_stamp for the LRU scheme
void cache::lru_update_time_stamp(int pos)
{
  //Update the requested cache line with the newest time stamp
  m_max_time_stamp++;
  m_cache[pos].time_stamp = m_max_time_stamp;
  //Update requests for this file
  m_cache[pos].request_rate++;

}

//Find the least recently used cache line
int cache::find_least_recently_used()
{
  int least_used = 0;
  struct request it_prev = m_cache[0];
  struct request it;
  for(unsigned int i = 1; i<m_cache.size(); i++)
    {
      it = m_cache[i];

      //Find oldest time_stamp
      if(it.time_stamp < it_prev.time_stamp)
	{
	  least_used = i;
	}

      it_prev = it;

    }


  return least_used;
}

//Store request via LRU (least recently used)
int cache::new_request_lru(struct request req)
{

  //Update maximum time_stamp
  if(m_max_time_stamp < req.time_stamp)
    {

      m_max_time_stamp = req.time_stamp;
    }

  m_requests++;
  //Cache if priority is high enough
  if(req.priority > m_priority)
    {
      m_cache_requests++;
      int in_cache = find_request(req);

      //If it exists in the cache
      if(in_cache >= 0)
	{
	  m_cache_hit++;
	  cout << "Request already in cache. " << cache_info();
	  lru_update_time_stamp(in_cache);
	}
      //File larger than cache
      else if(req.file_size > m_size)
	{
	  //Unable to cache
	  m_cache_requests--;
	  m_unstored++;
	  cout << "Not caching request, file_size larger than cache_size. " << cache_info();

	}
      //Add into cache
      else
	{
	  m_cache_miss++;
	  lru_store_request(req);
	}

    }
  else
    {
      m_unstored++;
      cout << "Not caching request, priority too low. " << cache_info();
      
    }

  cout << endl;

  return 0;
}

//Store requests via TTL (Time to live)
int cache::new_request_ttl(struct request req)
{

  // Get current time
  m_time = req.time_stamp;

  //Priority ok
  if(req.priority > m_priority)
    {
      m_cache_requests++;
      int in_cache = find_request(req);
      

      //Cache hit
      if(in_cache >= 0)
	{
	  //TODO check if old cache line
	  if(m_time > m_cache[in_cache].time_stamp + m_time_limit)
	    {
	      m_cache_miss++;
	      m_cache[in_cache].time_stamp = req.time_stamp;
	      cout << "Cache miss, line too old" << cache_info();
	      m_cache[in_cache].request_rate++;
	      
	    }
	  else
	    {
	      m_cache_hit++;
	      cout << "Cache hit. " << cache_info();
	  

	      //TODO UPDATE TIME STAMP
	      m_cache[in_cache].time_stamp = req.time_stamp;
	      m_cache[in_cache].request_rate++;
	    }
	}
      //Cache miss
      else
	{
	  m_cache_miss++;
	  ttl_store_request(req);
	}
    }
  //Priority too low
  else
    {
      m_unstored++;
      cout << "Not caching request, priority too low. " << cache_info();

    }

  cout << endl;
  return 0;
}

//Store the request in the cache according to TTL
int cache::ttl_store_request(struct request req)
{
 
  //If first cacheline, dont increase cache size
  if(m_elements == 0)
    {
      m_cache[0] = req;
    }
  else
    {
      m_cache.push_back(req);
    }

  
  m_bytes += req.file_size;
  m_elements++;
  cout << "Adding to cache. " << cache_info();

  return 0;
}


//Adds the request to statistics
void cache::add_request_to_stats(struct request req)
{

  stringstream ss;
  ss << request_to_string(req) << " was requested: " << req.request_rate << " times." << endl;
  m_request_rates += ss.str();

}

//Formats the cache hit
string cache::get_cache_hit()
{

  stringstream ss;
  
  double cache_hit = m_cache_hit/(double)m_cache_requests;

  cache_hit = cache_hit * 100;

  ss << cache_hit << "%";

  return ss.str();
}


//Formats the cache miss
string cache::get_cache_miss()
{

  stringstream ss;
  
  double cache_miss = m_cache_miss/(double)m_cache_requests;

  cache_miss = cache_miss * 100;

  ss << cache_miss << "%";

  return ss.str();
}


//Calculate current statistics
string cache::get_stats()
{

  //Add the stats of the current cache
  for(unsigned int i = 0; i<m_cache.size(); i++)
    {
      add_request_to_stats(m_cache[i]);
    }

  stringstream ss;

  //Stats
  ss << "###############################" << endl
     << "Cache stats using " << m_scheme << " scheme" << endl << "Cache_size: " 
     << m_size << endl << "Priority " << m_priority << endl << "Timelimit " 
     << m_time_limit << endl << "Statsfile: " << m_stats_file << endl
     << "###############################" << endl
     << "Total requests: " << m_requests << endl
     << "Total cache requests: " << m_cache_requests << endl
     << "Total cache hits: " << m_cache_hit << " (" << get_cache_hit() << ")" << endl
     << "Total cache misses: " << m_cache_miss << " (" << get_cache_miss() << ")" << endl
     << "Total unstored: " << m_unstored << endl
     << "###############################" << endl
     << m_request_rates << endl;
    

  string stats = ss.str();
  return stats;
}


//Create stats file
void cache::create_stats()
{
  ofstream myfile;

  string stats = get_stats();
  myfile.open (m_stats_file);
  myfile << stats;
  myfile.close();


  //cout << stats << endl;
}
