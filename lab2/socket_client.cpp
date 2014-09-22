#include "socket_client.hpp"
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>

using namespace std;

socket_client::socket_client(string h, string message) : socket_client(h, message, 80){
 
}

socket_client::socket_client(string h, string message, int port){

  //cout << "Initiating socket client" << endl;
  stringstream ss;
  ss << port;
  ss >> portno;
  msg = message;
  host = h;
 
}

socket_client::~socket_client(){
  //Free allocated memory
  // freeaddrinfo(&result);
	
} 

int socket_client::m_recv(char *recv_buffer){

  int recv_size = 0;
  recv_size = recv(sockfd, recv_buffer, sizeof(recv_buffer), 0);


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

void socket_client::set_host(string h)
{
  host = h;
}

void socket_client::set_request(string req)
{
  msg = req;
}


int socket_client::m_send(string message){

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
  int status = getaddrinfo(host.c_str(), portno.c_str(), &hints, &result);
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

int socket_client::m_close()
{
  close(sockfd);
  return 0;
}


int socket_client::start()
{

  //check that socket was setup succsefully
  if(m_socket(&result)){
    cout << "Error, socket setup failed" << endl;
    return -2;
  }

  if(m_send(msg)){
    cout << "Error, failed to send" << endl;
    return -1;
  }
  
  return 0;
}
