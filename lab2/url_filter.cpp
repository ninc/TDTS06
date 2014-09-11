#include "url_filter.hpp"
#include <vector>
#include <sstream>

using namespace std;

url_filter::url_filter()
{
	key_words.push_back("SpongeBob");
	key_words.push_back("Britney Spears");
	key_words.push_back("Paris Hilton");
	key_words.push_back("Norrköping");
}

url_filter::~url_filter()
{

}

//Sort header elements into a dictionary called header
void url_filter::sort_header(string http)
{
  string delimiter = ": ";
  stringstream ss;
  size_t pos = 0;
  string line;
  string key;
  string value;

  ss << http;
  
  //Sort header string into a dictionary for easy access
  while(getline(ss,line))
    {
      if(line != "\r")
	{
	  cout << line << endl;
	  pos = line.find(delimiter);
	  if(pos > 0)
	    {
	      key = line.substr(0, pos);
	      value = line.substr(pos+delimiter.size(), line.size());
	      value.erase(value.size()-1,value.size());
	      header[key] = value;
	    }
	}
    }
}

string url_filter::filter(string http)
{
  cout << "url filter startas" << endl;
  string temp = http;
  string request;
  string url;
  string http_version;
  stringstream ss;
  ss << temp;
  ss >> request >> url >> http_version;
  
  // Get request header
  if(request.substr(0,2).compare("GET"))
    {
      string header = http;
      header.erase(0, header.find("\n") + 1);
      sort_header(header);
  
    }
  else
    {
      cout << "Not a GET request" << endl;
    }

  cout << header["Host"] << endl;
  cout << "url filter finished" << endl;
  return header["Host"];

}
