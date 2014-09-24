#include "url_filter.hpp"
#include "cache.hpp"
#include <vector>
#include <sstream>
#include <string>

using namespace std;

//This class url_filter fills the following requirement:
//Requirement 3: "Blocks requests for undesirable URLs, using HTTP redirection to display this error page instead"

//Initiate the url_filter, the passing of the socket_server will be furth explained in cache.cpp... 
url_filter::url_filter(socket_server *socket_server)
{
  m_sr = socket_server;
  //The values to be filtered in the url, sent by the client
  key_words.push_back("SpongeBob");
  key_words.push_back("Britney Spears");
  key_words.push_back("Paris Hilton");
  key_words.push_back("Norrköping");
  url = "";
  url_redirect = false;
}

url_filter::~url_filter()
{

}

//Check if the url contains any of the inappropriate words
bool url_filter::check_url(string url){
  std::vector<string>::iterator it1;
  int illegal_url;
  //Check if the url contains any  illegal keywords 
  for(it1=key_words.begin(); it1!=key_words.end(); ++it1){
    illegal_url = url.find(*it1);
    //cout << "url: " << url << " *it1: " << *it1 << " illegal_url: " << illegal_url << endl;
    
    if(illegal_url > 0)
      {
	return true;
      }
      
  }
  return false;
}

//Sort header elements into a dictionary called header. Input is the get_request from client.
//Requirement number 1 is reached here, supports HTTP 1.0 and 1.1
string url_filter::sort_header(string http)
{
  string delimiter = ": ";
  stringstream ss;
  size_t pos = 0;
  string line;
  string key;
  string value;
  string host_name = "Unknown host";
  ss << http;
  
  //Sort header string into a arrays for easy access
  while(getline(ss,line))
    {
      if(line != "\r")
	{
	
	  pos = line.find(delimiter);
	  if(pos > 0)
	    {
	      key = line.substr(0, pos);
	      value = line.substr(pos+delimiter.size(), line.size());
	      value.erase(value.size()-1,value.size());
	      
	      //cout << "Key: " << key << " value: " << value << endl;

	      if(key.compare("Connection") == 0){
		//The proxy should support both HTTP/1.0 and HTTP/1.1.
		//i.e. Connection-type: close
		  value = "close";
	      }else if(key.compare("Host") == 0){
		//cout << "Found host_name: " << value << endl;
		host_name = value;
		}
		first_url_part.push_back(key);
		second_url_part.push_back(value);
		
	    }
	}
    }

  return host_name;
}


//URL filter the get_request (http), if it contains inappropriate a redirection-flag is set
string url_filter::m_filter(string http)
{
  string temp = http;
  string request_type;
  string http_version;
  string header = http;
  stringstream ss;
  ss << temp;
  ss >> request_type >> url >> http_version;

  //cout << endl <<  "http request: " << endl << http << endl << endl << endl;

  // Get request header
  if(request_type.substr(0,2).compare("GET"))
    { 
      //Check url for illegal content
      header = header.erase(0, header.find("\n") + 1);
      if(check_url(url))
	{
	  //Set the flag for redirection
	  url_redirect = true;
	  host_name = "url_redirect";
	}
      else
	{
	  host_name = sort_header(header);
	}
  
    }
  else if(request_type.substr(0,3).compare("POST"))
    {
      cout << "POST request are not supported yet" << endl;
    }
  else
    {
      cout << "Incorrect or unsupported HTTP request" << endl;
    }

  //cout << "host_name: " << host_name << endl;
  string request_header = request_type + " " + url + " " + http_version + "\n";
  request = build_request(request_header);
  
  //cout << endl << "Request: " << endl << request << endl;

  return host_name;
}


//Rebuild the modified get_request (http), the Connection property is changed to "close", forcing this function build_request to be carried out
string url_filter::build_request(string request_header)
{
  
  string temp_string = request_header;
  std::vector<string>::iterator it1;
  std::vector<string>::iterator it2 = second_url_part.begin();
  for(it1=first_url_part.begin(); it1!=first_url_part.end(); ++it1){
    temp_string += *it1 + ": " + *it2 + "\r\n";
     ++it2;
  }
 
    temp_string += "\r\n";
  return temp_string;
}


//Start the filtering, in this function start the object "cache" can be seen, the socket_server (object) is passed in to this one,
//this will be explained in cache.cpp...
int url_filter::start(string http_request)
{


  string host_name = m_filter(http_request);

  //Error handling for incorrect requests
  if(host_name == "Unknown host")
    {
      cerr << "Unable to parse host_name from http_request: " << endl << http_request << endl;
      return -2;
    }

  cache c = cache(this, m_sr);

  if(c.start(url, request, host_name, url_redirect) < 0){
    cerr << "Cache.start() failed" << endl;
    return -1;
  }

  //cout << endl <<"IN URL FILTER" << endl;
  return 0;
}


