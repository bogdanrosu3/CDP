#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#define PORT 9998

void error(const char *msg)
{
perror(msg);
exit(0);
}

int main(){

	int clientSocket, ret, n;
	struct sockaddr_in serverAddr;
	char buffer[255];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");
	bzero((char *) &serverAddr , sizeof(serverAddr));
	//memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		printf("Client: \t");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		    break;
		}
		if(strcmp(buffer, ":calc") == 0){
			    int num1, num2, answer, choice;

				s:(buffer,255);
				n=read(clientSocket,buffer,255);                           //read server string
				if(n<0) {error("ERROR reading from socket");}
				printf("Server - %s\n",buffer);
				scanf("%d", &num1);                              //enter nr 1
				write(clientSocket, &num1, sizeof(int));                    //send nr 1 to server

				bzero(buffer,255);
			    n=read(clientSocket,buffer,255);                          //read server string
				if(n<0) {error("ERROR reading from socket");}
				printf("Server - %s\n",buffer);
				scanf("%d", &num2);                              //enter nr 2
				write(clientSocket, &num2, sizeof(int));                    //send nr 2 to server

				bzero(buffer,255);
				n=read(clientSocket,buffer,255);                            //read server string
				if(n<0) {error("ERROR reading from socket");}
				printf("Server - %s",buffer);
				scanf("%d", &choice);                              //enter choice
				write(clientSocket, &choice, sizeof(int));                    //send choice to server

				if(choice == 5){
					goto Q;
				}
				read(clientSocket, &answer, sizeof(int));
				printf("Server - The answer is: %d\n", answer);

				if(choice != 5){
					goto s;
				}
				  Q:
				   printf("you have selected to exit. Exit Successful.");
				   close(clientSocket);
				   return 0;
		 break;
		}

		if(recv(clientSocket, buffer, 255, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
	}

	return 0;
}
