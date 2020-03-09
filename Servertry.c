#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


/* portul folosit */
#define PORT 3101

/* codul de eroare returnat de anumite apeluri */
extern int errno;

int main(){

	int sd, ret, n;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char message[1024];
	pid_t childpid;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		printf("[-]Error in connection.\n");
		return errno;
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		return errno;
	}
	printf("[+]Bind to port %d\n", 3101);

	if(listen(sd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
		return errno;
	}


	while(1){
		newSocket = accept(sd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
			return errno;
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sd);

			while(1){
				recv(newSocket, message, 1024, 0);
				if(strcmp(message, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client: %s\n", message);
					send(newSocket, message, strlen(message), 0);
					bzero(message, sizeof(message));
				}
				if(strcmp(message, ":calc") == 0){
					int num1, num2, answer, choice;

					s : n=write(newSocket,"Enter Number 1: ",strlen("Enter Number 1")); //Ask for number 1
					if(n < 0) error("ERROR writing to socket");
					read(newSocket, &num1, sizeof(int));            //Read No 1
					printf("Client - Number 1 is: %d\n" , num1);  

					n=write(newSocket,"Enter Number 2: ",strlen("Enter Number 2")); //Ask for number 2
					if(n < 0) error("ERROR writing to socket");
					read(newSocket, &num2, sizeof(int));            //Read No 2
					printf("Client - Number 2 is: %d\n" , num2);   

					n=write(newSocket,"Enter your choice: \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n",strlen("Enter your choice : : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n"));   //Ask for choice
					if(n < 0) error("ERROR writing to socket");
					read(newSocket, &choice, sizeof(int));            //Read choice
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

					write(newSocket, &answer, sizeof(int));
					if(choice != 5)
						  goto s;

					Q: close(newSocket);
					   close(sockd);
					   return 0;
					}
				}
				
				
				
			}
		}
	
	}
	close(newSocket);


	return 0;
}