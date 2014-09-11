#include "socket_client.hpp"
#include <sstream>
#include <string>
#include <cstring>

using namespace std;


socket_client::socket_client(string u, string message){
  socket_client(u, message, 80);
}

socket_client::socket_client(string u, string message, int port){
  stringstream ss;
  ss << port;
  ss >> portno;
  msg = message;
  url = u;
  
}
socket_client::~socket_client(){
  //Free allocated memory
  // freeaddrinfo(&result);
	
} 




int socket_client::m_connect(){
  
  //Trying to connect to socket
  if (connect(sockfd, result->ai_addr, result->ai_addrlen) < 0) {
    return -1;
    //Todo throw exception
  }else{
    return 0;
  }
}


int socket_client::m_recv(){

  //Empty buffer
  cout << "Socket client memset" << endl;
  memset(&recv_buffer, 0, sizeof(recv_buffer));

  int recv_size = recv(sockfd, recv_buffer, BUFFER_SIZE, 0);
  
  //Check if socket has been read
  if (recv_size < 0) {
    //Todo throw exception
    return -1;
  }
  else
    {
    return recv_size;
  }
} 


int socket_client::m_send(){

     
  cout << msg << endl;

  //Reply via socket
  int reply_size = send(sockfd, &msg, msg.length(), 0);
  //If reply was sent
  if (reply_size < 0){
    //Todo throw exception
    return -1;
  }
  else
    {
      return 0;
    }


}

int socket_client::m_socket(){
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
  hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  //Get addr_info
  if(getaddrinfo(url.c_str(), portno.c_str(), &hints, &result) != 0){
    return -2;
  }
  //setup socket with addr_info data
  sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  // Error check for opening socket
  if (sockfd < 0) {
    return -1;
    //Todo throw exception
  }else{
    return 0;
  }
}


string socket_client::start()
{
  string response;
 
 
  
  //check that socket was setup succsefully
  if(m_socket()){
    return "Error, socket setup failed";
  }
  if(m_connect()){
    return "Error, failed to connect";
  }
  
  if(m_send()){
    return "Error, failed to send";
  }
  if(m_recv()){
    return "Error, failed to recieve";
  }
  if(close(sockfd)){
    return "Error, failed to close";
  }
  
  //Send get request
  //receive response
  //close connection
  //pass response to content filter
  //parse in contentfilter
  return response;
  //Todo
  //Connect port 80 to server address
  //Send get request
  //receive response
  //close connection
  //pass response to content filter
  //parse in contentfilter
}
