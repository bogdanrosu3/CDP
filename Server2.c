    
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
exit(1);
}

int main (int argc , char *argv[])
{
   if(argc < 2)
     {
       fprintf(stderr , "Port no not provided.\n");
       exit(1);
     }
   int sockd, newsockd , portno , n;
   char buffer[255];
   struct sockaddr_in serv_adr;	
   struct sockaddr_in cl_adr;
   socklen_t clilen;

   sockd = socket (AF_INET, SOCK_STREAM, 0);
   if(sockd < 0)
       {
          error("Error opening socket.");
        }
   
   bzero((char *) &serv_adr , sizeof(serv_adr));
   portno = atoi(argv[1]);

   serv_adr.sin_family = AF_INET;	
   serv_adr.sin_addr.s_addr = INADDR_ANY;
   serv_adr.sin_port = htons (portno);

   if (bind (sockd, (struct sockaddr *) &serv_adr, sizeof (serv_adr)) < 0)
         error("Binding failled.");

   listen(sockd, 5);
   clilen = sizeof(cl_adr);
    
   newsockd = accept(sockd, (struct sockaddr *) &cl_adr, &clilen);

  if(newsockd < 0)
  error("Error on accept");

int num1, num2, answer, choice;

s : n=write(newsockd,"Enter Number 1: ",strlen("Enter Number 1")); //Ask for number 1
if(n < 0) error("ERROR writing to socket");
read(newsockd, &num1, sizeof(int));            //Read No 1
printf("Client - Number 1 is: %d\n" , num1);  

n=write(newsockd,"Enter Number 2: ",strlen("Enter Number 2")); //Ask for number 2
if(n < 0) error("ERROR writing to socket");
read(newsockd, &num2, sizeof(int));            //Read No 2
printf("Client - Number 2 is: %d\n" , num2);   

n=write(newsockd,"Enter your choice: \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n",strlen("Enter your choice : : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n"));   //Ask for choice
if(n < 0) error("ERROR writing to socket");
read(newsockd, &choice, sizeof(int));            //Read choice
printf("Client - Choise is: %d\n" , choice); 

switch(choice)
{
   case 1:
          answer = num1 + num2;
          break;
   case 2:
          answer = num1 - num2;
          break;
   case 3:
          answer = num1*num2;
          break;
   case 4:
          answer = num1/num2;
          break;
   case 5:
          goto Q;
          break;
}

write(newsockd, &answer, sizeof(int));
if(choice != 5)
      goto s;

Q: close(newsockd);
   close(sockd);
   return 0;
}