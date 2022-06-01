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
#include <stdbool.h>
#include <string.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

#define MAX_THREADS 1024
#define MAX_CHAR_LEN 1024

struct usageDetails
{
	int numFilesReq;
	int numFilesDown;
	int sizeDataTransferred;
};

struct client
{

	int index;
	int sockID;
	struct sockaddr_in clientAddr;
	int len;
	bool isOccupied;
	struct usageDetails usageDetails;
};

struct client Client[MAX_THREADS];
pthread_t thread[MAX_THREADS];

// Function Discription
void *ConnectNetwork(void *ClientDetail);

int main()
{

	int currThreadIdx = 0;
	for (int i = 0; i < MAX_THREADS; i++)
		Client[i].isOccupied = false;

	// setup socket
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		printf("Socket Error\n");
		return 0;
	}

	struct sockaddr_in serverAddr;
	unsigned short server_port = 1111;
	char server_ip[] = "127.0.0.1";

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(server_port);

	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		perror("Bind Error\n");
		return 0;
	}

	if (listen(serverSocket, 1024) == -1)
	{
		perror("Listen Error\n");
		return 0;
	}

	printf("Server started listenting on port %hu \n", server_port);
	while (1)
	{

		for (int i = 0; i < MAX_THREADS; i++)
		{
			currThreadIdx = (currThreadIdx + i) % MAX_THREADS;
			if (!Client[currThreadIdx].isOccupied)
				break;
		}

		if (Client[currThreadIdx].isOccupied)
		{
			printf("Cappacity of server reached. Try again later\n");
		}
		else
		{
			Client[currThreadIdx].sockID = accept(serverSocket, (struct sockaddr *)&Client[currThreadIdx].clientAddr, &Client[currThreadIdx].len);
			Client[currThreadIdx].index = currThreadIdx;
			Client[currThreadIdx].isOccupied = true;

			pthread_create(&thread[currThreadIdx], NULL, ConnectNetwork, (void *)&Client[currThreadIdx]);

			currThreadIdx++;
		}
	}

	for (int i = 0; i < MAX_THREADS; i++)
		if (Client[i].isOccupied)
			pthread_join(thread[i], NULL);

	close(serverSocket);

	printf("server closed. Bye!\n");

	return 0;
}

void *ConnectNetwork(void *ClientDetail)
{

	struct client *clientDetail = (struct client *)ClientDetail;
	int index = clientDetail->index;
	int clientSocket = clientDetail->sockID;

	printf("Client %d connected.\n", index + 1);

	clientDetail->usageDetails.numFilesDown = 0;
	clientDetail->usageDetails.numFilesReq = 0;
	clientDetail->usageDetails.sizeDataTransferred = 0;

	char data[MAX_CHAR_LEN];
	char response[MAX_CHAR_LEN];

	while (1)
	{
		printf("In MAIN LOOP\n");
		bzero(data, MAX_CHAR_LEN);
		int read = recv(clientSocket, data, MAX_CHAR_LEN, 0);

		if (read == 0)
		{
			sprintf(response, "Bye!");
			send(clientSocket, response, MAX_CHAR_LEN, 0);

			close(clientSocket);

			clientDetail->isOccupied = false;

			printf("client socked %d closed\n", clientSocket);

			return NULL;
		}

		printf("DEBUG: read - %d\n", read);
		data[read] = '\0';

		printf("DATA - %s\n", data);

		char *arg;
		char *rest = data;
		arg = strtok_r(rest, " ", &rest);

		printf("arg 1: %s\n", arg);

		if (strcmp(arg, "usage-details") == 0)
		{

			sprintf(response, "=== Usage Details===\nNumber of files requested : %d\nNumer of files received : %d\nSize of data transferred : %d Bytes\n==============",
					clientDetail->usageDetails.numFilesDown, clientDetail->usageDetails.numFilesReq, clientDetail->usageDetails.sizeDataTransferred);

			send(clientSocket, response, MAX_CHAR_LEN, 0);
		}

		else if (strcmp(arg, "reqFile") == 0)
		{

			clientDetail->usageDetails.numFilesReq++;

			arg = strtok_r(rest, " ", &rest);

			printf("arg 2: %s\n", arg);

			bool file_found = true;

			FILE *fp = fopen(arg, "r");
			if (fp == NULL)
			{
				file_found = false;
			}

			if (file_found)
			{

				sprintf(response, "Requested file %s is available. Do you want to downlaod the file? [Y/N] ", arg);
				send(clientSocket, response, MAX_CHAR_LEN, 0);

				read = recv(clientSocket, data, MAX_CHAR_LEN, 0);
				data[read] = '\0';

				while (1)
				{
					if (strcmp(data, "Y") || strcmp(data, "y") || strcmp(data, "Yes") || strcmp(data, "yes"))
					{

						sprintf(response, "SEND_START");
						send(clientSocket, response, MAX_CHAR_LEN, 0);

						sprintf(response, "We are sending the file %s", arg);
						send(clientSocket, response, MAX_CHAR_LEN, 0);

						bool success = true;

						while (fread(response, sizeof(char), sizeof(response), fp) > 0)
						{
							printf("In fread %d\n", sizeof(response));
							if (send(clientSocket, response, sizeof(response), 0) == -1)
							{
								perror("Error in sending file");
								success = false;
								break;
							}

							clientDetail->usageDetails.sizeDataTransferred += sizeof(response);
							bzero(response, MAX_CHAR_LEN);
						}
						if (success)
						{

							clientDetail->usageDetails.numFilesDown++;

							printf("file send complete\n");
							sprintf(response, "SEND_COMPLETE");
							send(clientSocket, response, MAX_CHAR_LEN, 0);

							sprintf(response, "File %s successfully sent!", arg);
							send(clientSocket, response, MAX_CHAR_LEN, 0);
						}
						else
						{
							printf("file send failed\n");
							sprintf(response, "SEND_FAILED");
							send(clientSocket, response, MAX_CHAR_LEN, 0);

							sprintf(response, "File %s transfer Failed!", arg);
							send(clientSocket, response, MAX_CHAR_LEN, 0);
						}

						break;
					}
					else if (strcmp(data, "N") || strcmp(data, "n") || strcmp(data, "No") || strcmp(data, "no"))
					{
						break;
					}
					else
					{
						sprintf(response, "Please choose Y (for yes) or N (for no) ");
						send(clientSocket, response, MAX_CHAR_LEN, 0);
					}
				}
			}
			else
			{

				sprintf(response, "Sorry the requested file %s is NOT AVAILABLE on the server", arg);
				send(clientSocket, response, MAX_CHAR_LEN, 0);
			}
		}
		else if (strcmp(arg, "exit") == 0)
		{
			sprintf(response, "Bye!");
			send(clientSocket, response, MAX_CHAR_LEN, 0);

			close(clientSocket);

			clientDetail->isOccupied = false;

			printf("client socked %d closed\n", clientSocket);

			return NULL;
		}
		else if (strcmp(arg, "help") == 0)
		{
			sprintf(response, "COMMANDS : \nusage-details : usage details (list of files client has downloaded so far, size of data transferred etc)\nreqFile <filename> : request file <filename> to be downloaded from the server\nexit : exit the application");
			send(clientSocket, response, MAX_CHAR_LEN, 0);
		}
		else
		{
			sprintf(response, "ERROR: no such command");
			send(clientSocket, response, MAX_CHAR_LEN, 0);
		}
	}
}