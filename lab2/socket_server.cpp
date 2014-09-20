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
    cout << "Failed to accept connection in socket server" << endl;
    return -1;
    //Todo throw exception
  }else{
    return 0;
  }
} 

int socket_server::m_bind(){



  //Setup socket to allow rebind of socket
  int optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);


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
int socket_server::m_recv(int sock, char *msg_buffer){

  //Empty buffer
  //cout << "Socket server trying to recv" << endl;
  char recv_buffer[256];
  int i = 0;
  int recv_size = 1;
  do{
    recv_size = recv(sock, recv_buffer, sizeof(recv_buffer), 0);

    for(int j = 0; j<recv_size; j++)
      {
	msg_buffer[i] = recv_buffer[j];
	i++;
      }

    //If end of HTTP request
    if(!strcmp("\r\n\r\n", &msg_buffer[i -4]))
       break;

  } while(recv_size > 0);

  //cout << "Socket server recv COMPLETE" <<  endl;
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


int socket_server::m_handle_request(int sock){

  //Receive message
  //cout << "Started handling of request" << endl;
  char msg_buffer[BUFFER_SIZE];
  //char *recv_buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
  memset(&msg_buffer, 0, sizeof(msg_buffer));
  //cout << "recieve buffers first position: " << msg_buffer[0] << " slut " << endl;

  
  if(m_recv(sock, msg_buffer)){
    cout << "Failed to recieve message in socket server" << endl;   
    return -1;
  }
  
  
  //Do processing
  url_filter uf = url_filter();
  
  string message(msg_buffer);

  string response = uf.start(message);

  cout << response << endl;

  //Write back to client
  m_send(sock, response);
  
  return 0;
}


int socket_server::m_send(int sock, string message){

  //cout << "Socket server m_send started" << endl;

  int sent_left = message.length();
  int reply_size;
  char *msg_ptr = (char*)message.c_str();

  //cout << sent_left << endl;
  while(sent_left > 0)
    {
      //cout << "msg left: " << msg_ptr << endl;

      //Reply via socket
      reply_size = send(sock, msg_ptr, sent_left, 0);
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
  //cout << "Socket server m_send finished" << endl;
  return 0;

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
      //cout << "Accepted request in socket server" << endl;
      int pid = fork();
      if (pid < 0)
        {
	  perror("ERROR on fork");
	  exit(1);
        }
      if (pid == 0)  
        {
	  /* This is the client process */
	  //cout << "Initating client processes in socket server" << endl;
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
