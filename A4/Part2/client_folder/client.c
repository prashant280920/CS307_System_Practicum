#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>


#define MAX_CHAR_LEN 1024

void clientHandler(int clientSocket);
void readLine(char* line);


int main(int argc, char const* argv[]){

	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1)
    {
        perror("Socket Failed");
        return 0;
    }

	struct sockaddr_in serverAddr;
	int server_port = 1111;
	// char server_ip[] = "127.0.0.1";

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(server_port); 
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0) {
		perror("Server Connection Error");
		return 0;
	}

	printf("Connection with Server %s at port %d \n",argv[1], server_port);

	clientHandler(clientSocket);

	if (close(clientSocket) == -1)
	{
		perror("Failed to close socket");
	}
	else printf("Closed Socket successfully\nThank U;)\n Bye Bye!\n");

	return 0;

}

void readLine(char* line)
{
    if (fgets(line, MAX_CHAR_LEN+1, stdin) == NULL)
    {
        if (feof(stdin))
        {
            printf("Bye! ;)\n" );
            exit(0);
        }
        printf("Command Input: Too long input, out of buffer range.");
        return;
    }
}



void clientHandler(int clientSocket)
{
	char input[MAX_CHAR_LEN];
	char data[MAX_CHAR_LEN];
	int read;
	char* filename;

	while(1)
	{

		readLine(input);
		input[strlen(input)-1] = '\0';
		send(clientSocket,input, MAX_CHAR_LEN,0);

		char* arg;
		char* rest = input;
		arg = strtok_r(rest, " ", &rest);

		if (strcmp(arg, "reqFile") == 0)
		{
			printf("hello!!\n");
			arg = strtok_r(rest, " ", &rest);
			filename = strdup(arg);
		}
		printf("filename 1 : %s\n", filename);

		read = recv(clientSocket, data, MAX_CHAR_LEN, 0);
		data[read] = '\0';

		if (strcmp(data, "SEND_START") == 0)
		{
			printf("In send_start\n");
			read = recv(clientSocket, data, MAX_CHAR_LEN, 0);
			data[read] = '\0';
			printf("%s\n\n", data);

			

			printf("filename - %s\n", filename);

			FILE *fp;
			// char *filename = arg;

			fp = fopen(filename, "w");

			if (fp == NULL) 
			{
				printf("ERROR detected!!\n");
			}

			while (1) 
			{
				printf("In fwrite while loop\n");
				read = recv(clientSocket, data, MAX_CHAR_LEN, 0);

				if (strcmp(data, "SEND_COMPLETE") == 0 || strcmp(data, "SEND_FAILED") == 0)
				{
					break;
				}

				int data_len = strlen(data);

				printf("data_len: %d\n", data_len);

				// read = recv(clientSocket, data, MAX_CHAR_LEN, 0);

				fwrite(data, sizeof(char), data_len, fp);
				bzero(data, MAX_CHAR_LEN);
			}
			fclose(fp);
			read = recv(clientSocket, data, MAX_CHAR_LEN, 0);
			data[read] = '\0';
			printf("%s\n\n",data);
		}
		else
		{
			printf("%s\n\n",data);
		}

		if (strcmp(input, "exit") == 0) 
		{
			free(filename);
			return;
		}

		bzero(input, MAX_CHAR_LEN);
	}

}
