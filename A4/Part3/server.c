#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#define MAXREQ 200
#define MAXQUEUE 3


void server(int consockfd);
void CRC(char divisor[], char data[], int N, int dataLen, char remainder[]);



int main(int argc, char const *argv[])
{

	int socket_fd, consockfd, client_len, portno = 2001;
	struct sockaddr_in serv_addr, cli_addr;

	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);


	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	if (bind(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Binding Failure");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		printf("Server is on\nListening for incoming connections at port %d \n", portno);

		if (listen(socket_fd, MAXQUEUE) < 0)
		{
			perror("Listen Failed");
			exit(EXIT_FAILURE);
		}

		client_len = sizeof(cli_addr);
		consockfd = accept(socket_fd, (struct sockaddr *)&cli_addr, (socklen_t *)&client_len);

		printf("Connection Accepted\n");

		server(consockfd);

		close(consockfd);
	}
	close(socket_fd);

	return 0;
}


void server(int consockfd)
{

	char divisor[] = "11001";
	int N = (int)(sizeof(divisor) / sizeof(divisor[0])) - 1;

	int chunkLength = 8;

	char reqbuf[MAXREQ];
	int n;
	while (1)
	{
		memset(reqbuf, 0, MAXREQ);
		n = read(consockfd, reqbuf, MAXREQ - 1);
		if (n <= 0)
			return;

		int dataLen = strlen(reqbuf);
		char *data = (char *)(malloc(sizeof(char) * (dataLen + 1)));
		for (int i = 0; i < dataLen; i++)
		{
			data[i] = reqbuf[i];
		}

		bool flag = false; 
		// Checking Error on every chunk
		for (int i = 0; i < dataLen; i += chunkLength + N - 1)
		{
			char remainder[N + 1];
			remainder[N] = '\0';
			CRC(divisor, data + i, N, chunkLength + N - 1, remainder);

			for (int k = 0; k < N; k++)
			{
				if (remainder[k] != '0')
				{
					flag = true;
					break;
				}
			}
		}

		if (flag)
		{
			printf(":( Error Detected \n");

			memset(reqbuf, 0, strlen(reqbuf));
			char message[] = "Error in transmission :(\n";
			for (int i = 0; i < strlen(message); i++)
			{
				reqbuf[i] = message[i];
			}
		}
		else
		{
			printf(";) No Error \n");

			memset(reqbuf, 0, strlen(reqbuf));
			char message[] = "Successful transmission :)\n";
			for (int i = 0; i < strlen(message); i++)
				reqbuf[i] = message[i];
		}
		write(consockfd, reqbuf, strlen(reqbuf));

		free(data);
	}
}


void CRC(char divisor[], char data[], int N, int dataLen, char remainder[])
{
	char dividend[N + 1];
	dividend[N] = '\0';
	for (int i = 0; i < N; i++)
	{
		dividend[i] = data[i];
	}

	if (dividend[0] == '1')
	{
		for (int j = 0; j < N; j++)
		{
			if (dividend[j] - divisor[j])
			{
				remainder[j] = '1';
			}
			else
			{
				remainder[j] = '0';
			}
		}
	}
	else
	{
		for (int j = 0; j < N; j++)
		{
			remainder[j] = dividend[j];
		}
	}

	for (int i = N; i < dataLen; i++)
	{
		for (int j = 1; j < N; j++)
		{
			dividend[j - 1] = remainder[j];
		}

		dividend[N - 1] = data[i];

		if (dividend[0] == '1')
		{
			for (int j = 0; j < N; j++)
			{
				if (dividend[j] - divisor[j])
				{
					remainder[j] = '1';
				}
				else
				{
					remainder[j] = '0';
				}
			}
		}
		else
		{
			for (int j = 0; j < N; j++)
			{
				remainder[j] = dividend[j];
			}
		}
	}

	return;
}

