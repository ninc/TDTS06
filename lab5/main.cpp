#include "cache.hpp"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main( int argc, const char* argv[] )
{
  int k = argc;

  cout << k << endl;
  
  for(int i = 0; i<k; i++)
    {
      //LOOK FOR LRU or TTL scheme
      cout << argv[i] << " ";
    }

  cout << endl;


  //Create new cache object with LRU or TTL scheme
  cache c = cache();

  string line;
  size_t comment;
  stringstream ss;
  /*
  int time_stamp;
  string client_id;
  int video_duration;
  string server_name;
  string file_name;
  int file_size;
  int priority;
  */

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
	  ss << line;
	  ss >> req.time_stamp >> req.client_id >> req.video_duration 
	     >> req.server_name >> req.file_name >> req.file_size >> req.priority;
  
	  //Cache input
	  //cache.add(req);

	  //Output
	  cout << req.time_stamp << " " << req.client_id << " ";
	  cout << req.video_duration << " " << req.server_name << " ";
	  cout << req.file_name << " " << req.file_size << " " << req.priority << endl;
	  
	    
	}
    }



  return 0;
}
