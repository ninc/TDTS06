/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;

     //Buffer for socket transfer
     char buffer[256];

     // Socket init struct
     struct sockaddr_in serv_addr, cli_addr;
     
     int n;

     // If port was not provided
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     
     // Setup a socket using an INTERNET socket via TCP/IP using standard protocol
     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     // Error check for opening socket
     if (sockfd < 0) 
        error("ERROR opening socket");

     
     // Set serv_addr struct to zero (bzero set memory to 0 values for n bytes)
     bzero((char *) &serv_addr, sizeof(serv_addr));

     // Read port number from command line
     portno = atoi(argv[1]);

     // Set the server addr struct
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     // Try to bind the socket to an address and a port number
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     //Listen to request on socket
     listen(sockfd,5);
     
     // Client length??
     clilen = sizeof(cli_addr);
     
     //Accept socket connection request
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     //Fill buffer with zeroes
     bzero(buffer,256);

     // Read socket into buffer
     n = read(newsockfd,buffer,255);

     //Check if socket has been read
     if (n < 0) error("ERROR reading from socket");

     
     printf("Here is the message: %s\n",buffer);

     //Reply via socket
     n = write(newsockfd,"I got your message",18);
     //If reply was sent
     if (n < 0) error("ERROR writing to socket");
     
     //Close sockets
     close(newsockfd);
     close(sockfd);
     return 0; 
}
