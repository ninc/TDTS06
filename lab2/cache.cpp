#include "cache.hpp"
#include "socket_client.hpp"
#include <string>




cache::cache()
{

  table["url_redirect"] = HTTP/1.1 200 OK
Date: Sat, 20 Sep 2014 23:07:52 GMT
Server: Apache
Last-Modified: Fri, 19 Sep 2014 08:41:35 GMT
Accept-Ranges: bytes
Content-Length: 311
Connection: close
Content-Type: text/html

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>JSundqvist.se</title>
<body>

SpongeBob, Britney Spears, Paris Hilton, or Norrköping

</body>
</html>
host_name: www.ida.liu.se
Not in cache
HTTP/1.1 200 OK
Date: Sat, 20 Sep 2014 23:08:23 GMT
Server: Apache/2.2.24 (Unix) DAV/2 SVN/1.6.17 PHP/5.3.23 mod_fastcgi/2.4.6 mod_auth_kerb/5.4+ida mod_jk/1.2.31 mod_ssl/2.2.24 OpenSSL/0.9.7d
Last-Modified: Wed, 06 May 2009 18:12:35 GMT
ETag: "4c4174-126-4694256fcaac0"
Accept-Ranges: bytes
Content-Length: 294
Connection: close
Content-Type: text/html

<html>

<title>
Net Ninny Error Page 1 for CPSC 441 Assignment 1
</title>

<body>
<p>
Sorry, but the Web page that you were trying to access
is inappropriate for you, based on the URL.
The page has been blocked to avoid insulting your intelligence.
</p>

<p>
Net Ninny
</p>

</body>

</html>

}


cache::~cache()
{



}


string cache::start(string url, string http_request, string host_name)
{

  string response = "";
  string request = http_request;

  cout << "host_name: " << host_name << endl;
  //No URL redirect
  if(host_name != "url_redirect")
    {
      unordered_map<string, string>::iterator in_cache = table.find(url);

      //Not in cache
      if(in_cache == table.end())
	{
	  //Connect via socket_client
	  cout << "Not in cache" << endl;

	  socket_client sc(host_name, http_request);  
	  response = sc.start();

	}
      else
	{
	  //Fetch from cache instead
	  cout << "In cache!!" << endl;
	}
    }
  //Redirect
  else
    {
      cout << "Redirecting, inappropriate URL" << endl;

    }

  return response;

}
