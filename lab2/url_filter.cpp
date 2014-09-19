#include "url_filter.hpp"
#include <vector>
#include <sstream>
#include <string>

using namespace std;

url_filter::url_filter()
{
	key_words.push_back("SpongeBob");
	key_words.push_back("Britney Spears");
	key_words.push_back("Paris Hilton");
	key_words.push_back("Norrköping");
	redirect_url = "www.ida.liu.se";
	redirect_get_request =
	  "GET http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error1.html HTTP/1.1\r\n"
	  "Host: www.ida.liu.se\r\n"
	  "User-Agent: Mozilla/5.0 (X11; Linux armv6l; rv:24.0) Gecko/20140727 Firefox/24.0 Iceweasel/24.7.0\r\n"
	  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
	  "Accept-Language: sv-SE,sv;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
	  "Accept-Encoding: gzip,deflate\r\n"
	  "Connection: keep-alive\r\n"
	  "\r\n";
	//cout << "The redirection get request....." << endl;
	//cout << redirect_get_request << endl;
}

url_filter::~url_filter()
{

}

bool url_filter::check_url(string url){
  std::vector<string>::iterator it1;
  for(it1=key_words.begin(); it1!=key_words.end(); ++it1){
    if(url.find(*it1) > 0){
	return true;
      }
      
  }
    return false;
}

//Sort header elements into a dictionary called header
string url_filter::sort_header(string http)
{
  string delimiter = ": ";
  stringstream ss;
  size_t pos = 0;
  string line;
  string key;
  string value;
  string url;
  ss << http;
  
  //Sort header string into a dictionary for easy access
  while(getline(ss,line))
    {
      if(line != "\r")
	{
	  //cout << line << endl;
	  pos = line.find(delimiter);
	  if(pos > 0)
	    {
	      key = line.substr(0, pos);
	      value = line.substr(pos+delimiter.size(), line.size());
	      value.erase(value.size()-1,value.size());
	      
	      if(key.compare("Connection") == 0){
		  value = "close";
	      }else if(key.compare("Host") == 0){
		url = value;
		//Check after Illegal material
		if(check_url(url)){
		  url = redirect_url;
		}
		}
		first_url_part.push_back(key);
		second_url_part.push_back(value);
		
	      //header[key] = value;
	    }
	}
    }
  return url;
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
      //Remove request header
      header.erase(0, header.find("\n") + 1);
      url = sort_header(header);
  
    }
  else
    {
      cout << "Not a GET request" << endl;
      }
  
  //cout << header["Host"] << endl;
  cout << "url filter finished" << endl;
  
  //return  build_request();
  //return header["Host"];

  return url;
}

string url_filter::build_request()
{
  
  string temp_string = "GET / HTTP/1.1\r\n";
  std::vector<string>::iterator it1;
  std::vector<string>::iterator it2 = second_url_part.begin();
  for(it1=first_url_part.begin(); it1!=first_url_part.end(); ++it1){
    temp_string += *it1 + ": " + *it2 + "\r\n";
     ++it2;
  }
 
    temp_string += "\r\n";
  return temp_string;
}


