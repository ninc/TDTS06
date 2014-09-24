#include "cache.hpp"
#include "content_filter.hpp"
#include "socket_client.hpp"
#include <string>
#include <cstring>
#include <unistd.h>



cache::cache(url_filter *url_filter, socket_server *socket_server)
{
  sr = socket_server;
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

//Stream the message to the browser without filtering
int cache::socket_stream(char *recv_buffer, string msg)
{

  int recv_size = 1;
  string response = msg;
  //Pass the first message
  sr->m_send(response);

  //Stream the response to the browser
  while(recv_size > 0){	
    recv_size = sc->m_recv(recv_buffer);
    response = recv_buffer;
    sr->m_send(response);
  }

  return 0;
}

//Check the content type for the http response
string cache::check_response_type(char *recv_buffer)
{

  string msg_buffer = "";
  int recv_size = 0;
  string html_response;
  int recieved = 0;
  //int content_found = 0;

  do
    {
      recv_size = sc->m_recv(recv_buffer);
      
      msg_buffer += string(recv_buffer, recv_size);

      /*
      for(int j = 0; j<recv_size; j++)
	{
	  msg_buffer[i] = recv_buffer[j];
	  i++;
	}
      */


      //Encode HTML response to check if its text format or not
      html_response = msg_buffer;
      int text_file = html_response.find("/r/n/r/n");
     
      /*
      //Content-Type found
      if(text_file >= 0)
	{
	  //Make sure that we have recieved content-type properly
	  content_found++;
	  if(content_found > 3)
	    break;

	}
      */
      
      //Found end of HTML header
      if(text_file >= 0)
	break;

      recieved += recv_size;
    }while(recv_size > 0);

	  
  m_text = html_response.find("Content-Type: text");

  //cout << "m_text: " << m_text << endl;

  //cout << msg_buffer << endl << endl << endl;
  

  return msg_buffer;

}

//Store the text message for filtering
string cache::recv_all(char *recv_buffer, string msg)
{
  
  //cout << "In recv_all" << endl;
  
  string * msg_buffer = new string(msg);

  int recv_size = 1;
  while(recv_size > 0)
    {
	
      recv_size = sc->m_recv(recv_buffer);
	      
      *msg_buffer += string(recv_buffer, recv_size);

      /*
      //If end of HTTP request
      if(!strcmp("\r\n\r\n", &msg_buffer[i -4]))
	break;
      */
    }

  string response(*msg_buffer);
  
  delete msg_buffer;

  return response;
}


int cache::handle_request(string request, string host_url, string host)
{
  int recv_buffer_size = 2047;
  m_text = -1;
  unordered_map<string, string>::iterator in_cache = table.find(host_url);
  string response = "";

  //cout << "Request: " << endl << request << endl;

  //Not in cache
  if(in_cache == table.end())
    {
      //Connect via socket_client
      //cout << "Not in cache" << endl;
      sc = new socket_client(host, request);  
      char *recv_buffer = new char[recv_buffer_size]();
      //char *msg_buffer = new char[BUFFER_SIZE];
      if(sc->start() != 0)
	{
	  cout << "Error in socket setup: " << request << endl;
	  return -1;
	}

      response = check_response_type(recv_buffer);

      //cout << response << endl;
      
      // If the response contains a text file
      if(m_text >= 0)
	{
	  //cout << "Text response!!" << endl;

	  response = recv_all(recv_buffer, response);

	  //cout << "First response: " << endl << endl << response << endl;

	  //Filter the content of the message
 
	  content_filter cf = content_filter(&uf->key_words);
	  bool bad_content = cf.start(response);

	  //cout << "Hej hopp!" << endl;


	  //Content redirect
	  if(bad_content){
	    cout << "Redirecting, inappropriate CONTENT" << endl;
	    request = content_redirect.request;
	    host = content_redirect.host_name;
	    host_url = content_redirect.url;
	    sc->m_close();

	    //cout << "Second response: " << endl;

	    //Clear buffers
	    memset(recv_buffer, 0, sizeof(char)*recv_buffer_size);

	    //cout << "Memset complete " << endl;

	    sc->set_host(host);
	    sc->set_request(request);
	    if(sc->start() != 0)
	      {
		cout << "Error in redirect socket" << endl << request << endl;
		return -2;
	      }

	    response = recv_all(recv_buffer, "");
	    //cout << "2nd response done!" << endl;
	  }
	  
       
	  //Send the text message back to the browser
	  sr->m_send(response);
	  
	  //cout << "Send complete" << endl;
	}
      //If its not a text file
      else
	{
	  //cout << "Not a text file... streaming instead" << endl;
	  socket_stream(recv_buffer, response);
	}

      //cout << "Before delete" << endl;
      //Clean up
      delete sc;
      //cout << "sc complete" << endl;
      delete recv_buffer;

      //cout << "msg complete" << endl;
    }
  //Fetch from cache instead
  else
    {
      cout << "In cache!!" << endl;
    }

  
  //cout << "Cache complete" << endl;

  return 0;
}


int cache::start(string url, string http_request, string host_name, bool url_red)
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
      if(handle_request(request, host_url, host) < 0){
	cout << "Error in cache.cpp with URL redirect" << endl;
	return -2;
      }
    }
  else
    {
      if(handle_request(request, host_url, host) < 0){
	cout << "Error in cache.cpp with normal request" << endl;
	return -1;
      }
    }

  return 0;

}
