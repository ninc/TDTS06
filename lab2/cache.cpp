#include "cache.hpp"
#include "content_filter.hpp"
#include "socket_client.hpp"
#include <string>




cache::cache(url_filter *url_filter)
{
  uf = url_filter;
  url_redirect.request= "GET http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error1.html HTTP/1.1\r\n" 
    "Host: www.ida.liu.se\r\n"
    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:32.0) Gecko/20100101 Firefox/32.0\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "Connection: close\r\n\r\n";

  url_redirect.host_name = "www.ida.liu.se";
  url_redirect.url = "http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error1.html";
  
  content_redirect.request = "GET http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error2.html HTTP/1.1\r\n"
    "Host: www.ida.liu.se\r\n"
    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:32.0) Gecko/20100101 Firefox/32.0\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "Connection: close\r\n\r\n";
    content_redirect.url = "http://www.ida.liu.se/~TDTS04/labs/2011/ass2/error2.html";
    content_redirect.host_name = "www.ida.liu.se";
}


cache::~cache()
{



}

string cache::handle_request(string request, string host_url, string host)
{
  unordered_map<string, string>::iterator in_cache = table.find(host_url);
  string response;

  //Not in cache
  if(in_cache == table.end())
    {
      //Connect via socket_client
      //cout << "Not in cache" << endl;
      socket_client sc(host, request);  
      response = sc.start();
      
      //cout << endl << endl << response << endl;
    }
  else
    {
      //Fetch from cache instead
      cout << "In cache!!" << endl;
    }
  return response;
}
string cache::start(string url, string http_request, string host_name, bool url_red)
{

  string response = "";
  string request = http_request;
  string host_url = url;
  string host = host_name;

  //cout << endl << endl << request << endl << endl;

  //URL redirect
  if(url_red)
    {
      //cout << "Redirecting, inappropriate URL" << endl;
      request = url_redirect.request;
      host = url_redirect.host_name;
      host_url = url_redirect.url;
      response = handle_request(request, host_url, host);
    }
  else
    {
      response = handle_request(request, host_url, host);
      content_filter cf = content_filter(&uf->key_words);
      bool bad_content = cf.start(response);
      //Content redirect
      if(bad_content){
	//cout << "Redirecting, inappropriate CONTENT" << endl;
	request = content_redirect.request;
	host = content_redirect.host_name;
	host_url = content_redirect.url;
	response = handle_request(request, host_url, host);
	//	cout << endl << endl << "bad content" << endl << endl;
      }
    }
  //cout << endl << endl << response << endl;

  cout << "In cache: " << endl << response << endl;

  //handle_request

  return response;

}
