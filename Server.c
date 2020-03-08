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

	int sd, ret;
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
			}
		}
		else {
				close(newSocket);
		}
		close(newSocket);
	}

	close(newSocket);


	return 0;
}