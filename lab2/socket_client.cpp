#include "socket_client.hpp"
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>

using namespace std;


socket_client::socket_client(string u, string message) : socket_client(u, message, 80){
 
}

socket_client::socket_client(string u, string message, int port){

  //cout << "Initiating socket client" << endl;
  stringstream ss;
  ss << port;
  ss >> portno;
  msg = message;
  url = u;
  //cout << msg << endl;
  //cout << "Init finished, socket client" << endl;
  
}
socket_client::~socket_client(){
  //Free allocated memory
  // freeaddrinfo(&result);
	
} 

int socket_client::m_recv(){

  //Empty buffer
  //cout << "Socket client trying to recv" << endl;
  char recv_buffer[256];
  int i = 0;
  int recv_size = 1;
  int k = 0;
  //string html_response;
  int text_file;
  do{
    recv_size = recv(sockfd, recv_buffer, sizeof(recv_buffer), 0);
    
    //Check the first lines of the HTML response
    if(i == 0)
      {
	//Encode HTML response to check if its text format or not
	string html_response(recv_buffer);
	text_file = html_response.find("Content-Type: text");

	if(text_file > 0)
	  {
	    cout << "recv_buffer for k: " << k << " " << recv_buffer << endl;

	  }
      }

    for(int j = 0; j<recv_size; j++)
      {
	msg_buffer[i] = recv_buffer[j];
	i++;
      }
    k++;
    //cout << "recv_size: " << recv_size << " last in buffer: " << msg_buffer[i-4] 
    //	 << msg_buffer[i-3] << msg_buffer[i-2] << msg_buffer[i-1]
    //	 << msg_buffer[i] << endl;
    
    //If end of HTTP request
    if(!strcmp("\r\n\r\n", &msg_buffer[i -4]))
       break;

  } while(recv_size > 0);

  //cout << "Socket client recv COMPLETE" <<  endl;
  //Check if socket has been read
  if (recv_size < 0) {
    //Todo throw exception
    return -1;
  }
  else
    {
    return 0;
  }

} 


int socket_client::m_send(string message){

     
  //cout << "Socket client m_send started" << endl;

  int sent_left = message.length();
  int reply_size;
  char *msg_ptr = (char*)message.c_str();

  //cout << sent_left << endl;
  while(sent_left > 0)
    {
      //cout << "msg left: " << msg_ptr << endl;

      //Reply via socket
      reply_size = send(sockfd, msg_ptr, sent_left, 0);
      //If reply error
      if (reply_size < 0){
	//Todo throw exception
	return -1;
      }

      //Bytes left to send
      sent_left -= reply_size;
      //Move message pointer
      msg_ptr += reply_size;
      //cout << "sent left: " <<  sent_left << endl;
    }
  //cout << "Socket client m_send finished" << endl;
  return 0;
}

int socket_client::m_connect(struct addrinfo *result){
  
  //cout << "Starting connect socket client " << endl;

  //cout << result->ai_addr << endl;
  //cout << result->ai_addrlen << endl;
  //Trying to connect to socket
  int status = connect(sockfd, result->ai_addr, result->ai_addrlen);
  //cout << "connect status: " << status << endl;
  if (status < 0) {
    perror("Connect");
    return -1;
    //Todo throw exception
  }else{
    //cout << "Finished connect socket client " << endl;
    return 0;
  }
}

int socket_client::m_socket(struct addrinfo *result){
  

  //cout << "Started m_socket in socket client" << endl;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
  
  //Get addr_info
  int status = getaddrinfo(url.c_str(), portno.c_str(), &hints, &result);
  if(status != 0){
    cout << "Failed to getaddrinfo " << gai_strerror(status) << endl;
    return -2;
  }
  //setup socket with addr_info data
  sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  // Error check for opening socket
  if (sockfd < 0) {
    return -1;
    //Todo throw exception
  }
  
  if(m_connect(result)){
    return -3;
  }

  //cout << "Finished m_socket in socket client" << endl;
  return 0;
}


string socket_client::start()
{
  //cout << "Starting socket client " << endl;
  
  //check that socket was setup succsefully
  if(m_socket(&result)){
    return "Error, socket setup failed";
  }

  if(m_send(msg)){
    return "Error, failed to send";
  }
  if(m_recv()){
    return "Error, failed to recieve";
  }
  if(close(sockfd)){
    return "Error, failed to close";
  }
  
  string response(msg_buffer);

  //cout << "Socket client finished" << endl;
  return response;
}
