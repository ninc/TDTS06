#include "cache.hpp"
#include <iostream>

using namespace std;

int main( int argc, const char* argv[] )
{
  int k = argc;

  cache c = cache();
  

  cout << k << endl;
  
  for(int i = 0; i<k; i++)
    {
      cout << argv[i] << " ";
    }

  cout << endl;

  int time_stamp;
  string client_id;
  int video_duration;
  string server_name;
  string file_name;
  int file_size;
  int priority;


  while(cin >> time_stamp >> client_id >> video_duration >> server_name >> file_name >> file_size >> priority)
    {
      cout << time_stamp << " " << client_id << " ";
      cout << video_duration << " " << server_name << " ";
      cout << file_name << " " << file_size << " " << priority << endl;
    }



  return 0;
}
