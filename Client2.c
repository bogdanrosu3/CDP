#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void error(const char *msg)
{
perror(msg);
exit(0);
}

int main (int argc , char *argv[])
{

   int sockd , portno , n;
   struct sockaddr_in serv_adr;
   struct hostent *server;
   
   char buffer[255];
   if(argc < 3)
     {
       fprintf(stderr , "usage %s hostname port\n", argv[0]);
       exit(0);
     }
   portno = atoi(argv[2]);
   sockd = socket (AF_INET, SOCK_STREAM, 0);
   if(sockd < 0)
       {
          error("Error opening socket.");
        }
  
    bzero((char *) &serv_adr , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);	
    serv_adr.sin_port = htons (portno);

    if (connect (sockd, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
    {
      error("Connection Failed");
    }

    int num1, num2, answer, choice;

s:bzero(buffer,256);
  n=read(sockd,buffer,255);                           //read server string
  if(n<0) error("ERROR reading from socket");
  printf("Server - %s\n",buffer);
  scanf("%d", &num1);                              //enter nr 1
  write(sockd, &num1, sizeof(int));                    //send nr 1 to server

  bzero(buffer,256);
  n=read(sockd,buffer,255);                          //read server string
  if(n<0) error("ERROR reading from socket");
  printf("Server - %s\n",buffer);
  scanf("%d", &num2);                              //enter nr 2
  write(sockd, &num2, sizeof(int));                    //send nr 2 to server

  bzero(buffer,256);
  n=read(sockd,buffer,255);                            //read server string
  if(n<0) error("ERROR reading from socket");
  printf("Server - %s",buffer);
  scanf("%d", &choice);                              //enter choice
  write(sockd, &choice, sizeof(int));                    //send choice to server

  if(choice == 5)
          goto Q;

  read(sockd, &answer, sizeof(int));
  printf("Server - The answer is: %d\n", answer);

  if(choice != 5)
      goto s;

  Q:
   printf("you have selected to exit. Exit Successful.");
   close(sockd);
   return 0;
}