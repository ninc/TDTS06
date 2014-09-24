#include "content_filter.hpp"
#include <string>

//This class content_filter reaches the following reuqirement:
//4."Detects inappropriate content bytes within a Web page before it is returned to the user, and redirecting to this error page"

content_filter::content_filter()
{

}


//The words to be filtered out, same as url_filter
content_filter::content_filter(vector<string> *keys)
{
  key_words = keys;
}


content_filter::~content_filter()
{



}

//Filters out bad content from the server
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

