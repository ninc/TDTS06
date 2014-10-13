#include "cache.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

int main( int argc, const char* argv[] )
{
  int k = argc;
  cache_type cache_mode = LRU;
  string arg;
  int x = 1;
  int y = 1;
  int t = 0;
  size_t found;
  string stats_file = "";
  string invalid_args = "Invalid arguments!\nTry: cat input.txt | ./msc option=0 x=1 y=1 stats.txt > output.txt";


  //Check for arguments
  if(k < 4)
    {
      cout << invalid_args << endl;
      return 1;
    }

  for(int i = 1; i<k; i++)
    {
      arg = string(argv[i]);
      //Fix lowercase
      transform(arg.begin(), arg.end(), arg.begin(), ::tolower);
      //Sort inputs
      if(i == 1)
	{
	  if(arg == "option=0")
	    {
	      cache_mode = LRU;
	    }
	  else if(arg == "option=1")
	    {
	      cache_mode = TTL;
	    }
	}
      else if(i == 2)
	{
	  found = arg.find("x=");
	  
	  if(found != string::npos)
	    {
	      //cout << arg.substr(found) << endl;
	      x = atoi(arg.substr(found+2).c_str());
	      continue;
	    }

	  found = arg.find("y=");
	  
	  if(found != string::npos)
	    {
	      //cout << arg.substr(found) << endl;
	      y = atoi(arg.substr(found+2).c_str());
	      continue;
	    }

	  cout << invalid_args <<  endl;
	  return 1;
	   

	}
      else if(i == 3)
	{

	  found = arg.find("y=");
	  
	  if(found != string::npos)
	    {
	      //cout << arg.substr(found) << endl;
	      y = atoi(arg.substr(found+2).c_str());
	      continue;
	    }

	  found = arg.find("t=");
	  
	  if(found != string::npos)
	    {
	      //cout << arg.substr(found) << endl;
	      t = atoi(arg.substr(found+2).c_str());
	      continue;
	    }
	  
	  cout << invalid_args << endl;
	  return 1;


	}
      else if(i == 4)
	{
	  stats_file = arg;
	}
    }


  //Create new cache object with LRU or TTL scheme
  cache c = cache(cache_mode, x, y, t, stats_file);

  string line;
  size_t comment;
  stringstream ss;
  while(getline(cin, line))
    {

      comment = line.find("#");   
      //If comment or empty line, skip line
      if(comment != string::npos || line.length() == 0)
	{
	  //cout << line << endl;
	  continue;
	}
      //Handle input
      else
	{
	  struct request req;
	  //Read input
	  stringstream ss;
	  ss << line;
	  ss >> req.time_stamp >> req.client_id >> req.video_duration 
	     >> req.server_name >> req.file_name >> req.file_size >> req.priority;
	  
	  req.request_rate = 0;

	  //Cache input
	  c.new_request(req);
	    
	}
    }

  //Creates the stats file
  c.create_stats();

  return 0;
}
