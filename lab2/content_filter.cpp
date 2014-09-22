#include "content_filter.hpp"
#include <string>


content_filter::content_filter()
{

}



content_filter::content_filter(vector<string> *keys)
{
  key_words = keys;
}


content_filter::~content_filter()
{



}

bool content_filter::start(string http_response){
  bool bad_content = false;


  for(auto it : *key_words)
    {
      int k = http_response.find(it);
      if(k > 0)
	{
	  cout << "Found bad content: " << it << " at: " << k << endl;
	  bad_content = true;
	  break;
	}
    }

  return bad_content;
}

string content_filter::filter(string http){
  return http;
}

