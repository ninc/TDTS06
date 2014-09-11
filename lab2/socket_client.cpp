#include "socket_client.hpp"
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>

using namespace std;


socket_client::socket_client(string u, string message) : socket_client(u, message, 80){
 
}

socket_client::socket_client(string u, string message, int port){

  cout << "Initiating socket client" << endl;
  stringstream ss;
  ss << port;
  ss >> portno;
  msg = message;
  url = u;
  cout << msg << endl;
  cout << "Init finished, socket client" << endl;
  
}
socket_client::~socket_client(){
  //Free allocated memory
  // freeaddrinfo(&result);
	
} 




int socket_client::m_connect(struct addrinfo *result){
  
  cout << "Starting connect socket client " << endl;

  cout << result->ai_addr << endl;
  cout << result->ai_addrlen << endl;
  //Trying to connect to socket
  int status = connect(sockfd, result->ai_addr, result->ai_addrlen);
  cout << "connect status: " << status << endl;
  if (status < 0) {
    perror("Connect");
    return -1;
    //Todo throw exception
  }else{
    cout << "Finished connect socket client " << endl;
    return 0;
  }
}


int socket_client::m_recv(){

  //Empty buffer
  cout << "Socket client trying to recv" << endl;
  char recv_buffer[256];
  int i = 0;
  int recv_size = 1;
  do{
    recv_size = recv(sockfd, recv_buffer, sizeof(recv_buffer), 0);

    for(int j = 0; j<recv_size; j++)
      {
	msg_buffer[i] = recv_buffer[j];
	i++;
      }

    cout << msg_buffer << endl;

    //If end of HTTP request
    if(!strcmp("\r\n\r\n", &msg_buffer[i -4]))
       break;

  } while(recv_size > 0);

  cout << "Socket client recv COMPLETE" <<  endl;
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

     
  cout << "Socket client m_send started" << endl;

  int sent_left = msg.length();
  int reply_size;
  char *msg_ptr = message.c_str();

  cout << sent_left << endl;
  while(sent_left > 0)
    {
      //Reply via socket
      reply_size = send(sockfd, msg_ptr, send_left, 0);
      //If reply error
      if (reply_size < 0){
	//Todo throw exception
	return -1;
      
	sent_left -= reply_size;
	msg_ptr += reply_size;

	cout << sent_left << endl;
      }

      cout << "Socket client m_send finished" << endl;
      return 0;
}

int socket_client::m_socket(struct addrinfo *result){
  

  cout << "Started m_socket in socket client" << endl;
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

  cout << "Finished m_socket in socket client" << endl;
  return 0;
}


string socket_client::start()
{
  string response;
  //struct addrinfo result;

  cout << "Starting socket client " << endl;
  
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
  
  //Todo
  //Connect port 80 to server address
  //Send get request
  //receive response
  //close connection
  //pass response to content filter
  //parse in contentfilter

  cout << "Socket client finished" << endl;
  return response;
}
