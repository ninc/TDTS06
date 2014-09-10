#include "socket_client.hpp"

using namespace std;

socket_client::socket_client(int port){
  portno = port;
  clilen = sizeof(cli_addr);

  m_socket();
  m_bind();
  
}
socket_client::~socket_client(){
	
	
} 



int socket_client::m_bind(){
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  // Try to bind the socket to an address and a port number
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    return  -1;
    //Todo throw exception
  }else{
    return 0;
  }
} 

int socket_client::m_connect(int sock){
  if(getsockname(sock,(struct sockaddr *) &serv_addr, &clilen) < 0){
    return -1;
    //Todo throw exepction
  }else{
    if (connect(sock,(struct sockaddr *) &serv_addr, clilen) < 0) {
      return -1;
      //Todo throw exception
    }else{
      return 0;
    }
  }
}

int socket_client::m_recv(int sock){

  //Empty buffer
  memset(&recv_buffer, 0, sizeof(recv_buffer));

  int recv_size = recv(sock, recv_buffer, BUFFER_SIZE, 0);
  
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

//Uncertain function...
int socket_client::m_handle(int sock){

  //Receive message
  m_recv(sock);

  //Do processing
  cout << recv_buffer << endl;

  string message = recv_buffer;

  //Write back to client
  m_send(sock, message);

  return 0;
}


int socket_client::m_send(int sock, string message){

     
  cout << message << endl;

  //Reply via socket
  int reply_size = send(sock, &message, message.length(), 0);
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
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // Error check for opening socket
  if (sockfd < 0) {
    return -1;
    //Todo throw exception
  }else{
    return 0;
  }
}


int socket_client::start()
{
  return 0;
  //Todo
  //Connect port 80 to server address
  //Send get request
  //receive response
  //close connection
  //pass response to content filter
  //parse in contentfilter
}
