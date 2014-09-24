#include "socket_server.hpp"
#include "url_filter.hpp"
#include "socket_client.hpp"
#include <cstring>

using namespace std;

socket_server::socket_server(int port){
  m_portno = port;
  m_clilen = sizeof(m_cli_addr);

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
  listen(m_sockfd,5);
  return 0;
} 

int socket_server::m_accept(){
  m_newsockfd = accept(m_sockfd, (struct sockaddr *) &m_cli_addr, &m_clilen);
  if (m_newsockfd < 0){ 
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
  setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);


  m_serv_addr.sin_family = AF_INET;
  m_serv_addr.sin_addr.s_addr = INADDR_ANY;
  m_serv_addr.sin_port = htons(m_portno);
  // Try to bind the socket to an address and a port number
  if (bind(m_sockfd, (struct sockaddr *) &m_serv_addr, sizeof(m_serv_addr)) < 0) {
    return  -1;
    //Todo throw exception
  }else{
    return 0;
  }
} 
int socket_server::m_recv(int sock){

  //Empty buffer
  //cout << "Socket server trying to recv" << endl;
  char recv_buffer[1023];
  int recv_size = 1;
  request = "";
  string http_end = "";
  do{
    recv_size = recv(sock, recv_buffer, sizeof(recv_buffer), 0);

    request += string(recv_buffer, recv_size);


    //http_end = request.substr(request.length() -4, request.length());
    
    //cout << http_end << endl;
    //Find end of HTTP request
    if(request.find("\r\n\r\n") > 0)
      {
	break;
      }

  } while(recv_size > 0);

  if(request == "")
    {
      cerr << "Error: Recieved an empty http request" << endl;
      return -2;
    }
  
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
  if(m_recv(sock)){
    cerr << "Error: Failed to recieve message in socket server" << endl;   
    return -1;
  }
  
  //Do processing
  url_filter uf = url_filter(this);
 
  //cout << "Start filtering" << endl;
  if(uf.start(request) < 0){
    cout << "Failed to apply URL filtering" << endl;
    return -1;
  }

  return 0;
}


int socket_server::m_send(string message){

  //cout << "Socket server m_send started" << endl;

  int sent_left = message.length();
  int reply_size;
  string msg = message;
  char *msg_ptr = (char*)msg.c_str();

  //cout << sent_left << endl;
  while(sent_left > 0)
    {
      //Reply via socket
      reply_size = send(m_newsockfd, msg_ptr, sent_left, 0);
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
 
  m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // Error check for opening socket
  if (m_sockfd < 0) {
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
	  close(m_sockfd);
	  if(m_handle_request(m_newsockfd)){
	    cerr << "Error: failed to handle socket server request" << endl;
	  }
	  exit(0);
        }
      else
        {
	  close(m_newsockfd);
        }
    } /* end of while */

  return 0;
}
