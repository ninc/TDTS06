#include "socket_server.hpp"

using namespace std;

socket_server::socket_server(int port){
  portno = port;
  clilen = sizeof(cli_addr);

  m_socket();
  m_bind();
  
}
socket_server::~socket_server(){
	
	
} 
int socket_server::m_listen(){
  listen(sockfd,5);
  return 0;
} 
int socket_server::m_accept(){
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0){ 
    return -1;
    //Todo throw exception
  }else{
    return 0;
  }
} 

int socket_server::m_bind(){
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
int socket_server::m_recv(int sock){

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


int socket_server::m_handle_request(int sock){

  //Receive message
  m_recv(sock);

  //Do processing
  cout << recv_buffer << endl;

  string message = recv_buffer;

  //Write back to client
  m_send(sock, message);

  return 0;
}


int socket_server::m_send(int sock, string message){

     
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

int socket_server::m_socket(){
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // Error check for opening socket
  if (sockfd < 0) {
    return -1;
    //Todo throw exception
  }else{
    return 0;
  }
}


int socket_server::start()
{
  m_listen();
  
  //Modified code from: http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
  
  /* Now start listening for the clients, here 
   * process will go in sleep mode and will wait 
   * for the incoming connection
   */
  while (true) 
    {
      m_accept();
      /* Create child process */
      int pid = fork();
      if (pid < 0)
        {
	  perror("ERROR on fork");
	  exit(1);
        }
      if (pid == 0)  
        {
	  /* This is the client process */
	  close(sockfd);
	  m_handle_request(newsockfd);
	  exit(0);
        }
      else
        {
	  close(newsockfd);
        }
    } /* end of while */

  return 0;
}
