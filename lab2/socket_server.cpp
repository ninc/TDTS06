#include "socket_server.hpp"
#include "url_filter.hpp"
#include "socket_client.hpp"
#include <cstring>

using namespace std;

socket_server::socket_server(int port){
  portno = port;
  clilen = sizeof(cli_addr);

  if(m_socket())
    {
      cout << "Failed to init socket in socket server" << endl;
    }
  if(m_bind())
    {
      cout << "Failed to bind socket in socket server" << endl;
    }
  
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
  cout << "Socket server memset" << endl;
 
  memset(&recv_buffer, 0, sizeof(recv_buffer));


  cout << "buffer 0 " << recv_buffer[0] << endl;

  int recv_size = recv(sock, recv_buffer, BUFFER_SIZE, 0);
  cout << "Socket server memset COMPLETE" << endl;
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
  if(m_recv(sock)){
    cout << "Failed to recieve message in socket server" << endl;
    return -1;
  }


  //Do processing
  //cout << recv_buffer << endl;
  
  url_filter uf = url_filter();
  
  string message = "hej"; //recv_buffer;
  
  string url = uf.filter(message);
  socket_client sc = socket_client(url, message);
  sc.start();

  //Write back to client
  m_send(sock, message);

  return 0;
}


int socket_server::m_send(int sock, string message){

     
  //cout << message << endl;

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
      if(m_accept()){
	continue;
      }
      /* Create child process */
      cout << "Accepted request in socket server" << endl;
      int pid = fork();
      if (pid < 0)
        {
	  perror("ERROR on fork");
	  exit(1);
        }
      if (pid == 0)  
        {
	  /* This is the client process */
	  cout << "Initating client processes in socket server" << endl;
	  close(sockfd);
	  if(m_handle_request(newsockfd)){
	    cout << "Error, failed to hande request of server socket" << endl;
	  }
	  exit(0);
        }
      else
        {
	  close(newsockfd);
        }
    } /* end of while */

  return 0;
}
