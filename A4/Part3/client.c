#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>

#define MAXD 82
#define MAXIN 200
#define MAXOUT 200

// Function Description
void CRC(char divisor[], char data[], int N, int dataLen, char remainder[]);
void sending_data(char *inbuf, int len);
void client_function(int socket_fd);
int findRandom();



int main(int argc, char const *argv[])
{

	int socket_fd, portno = 2001;
	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(portno);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}
	if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Coonection failed");
		exit(EXIT_FAILURE);
	}
	printf("Connected to %s:%d\n", argv[1], portno);

	client_function(socket_fd);
	close(socket_fd);

	return 0;
}


int findRandom()
{
	int num = ((int)rand() % 2);
	if (num)
	{
		return '1';
	}
	else
	{
		return '0';
	}
}

void client_function(int socket_fd)
{
	int n;
	char send_buf[MAXIN];
	char recive_buf[MAXOUT];
	sending_data(send_buf, MAXIN);

	while (strlen(send_buf) > 0)
	{
		write(socket_fd, send_buf, strlen(send_buf));

		memset(recive_buf, 0, MAXOUT);
		n = read(socket_fd, recive_buf, MAXOUT - 1);
		write(STDOUT_FILENO, recive_buf, n);

		sending_data(send_buf, MAXIN);
	}
}



void sending_data(char *inbuf, int len)
{

	char divisor[] = "11001";
	int N = (int)(sizeof(divisor) / sizeof(divisor[0])) - 1;

	int chunkLength = 8;
	int n;
	scanf("%d", &n);

	char data[8 * n];
	for (int i = 0; i < 8 * n; i++)
	{
		data[i] = findRandom();
	}
	printf("Binary String: ");
	int dataLen = strlen(data) - 1;

	if (dataLen % chunkLength)
	{
		int i = dataLen;
		while (i % chunkLength)
		{
			data[i] = '0';
			dataLen++;
			i++;
		}
	}
	data[dataLen] = '\0';

	int modified_Data_Len = (chunkLength + N - 1) * ceil(dataLen / (1.0 * chunkLength));
	char *newData = (char *)(malloc(sizeof(char) * (modified_Data_Len + 1)));

	// CRC on every chunks
	memset(newData, 0, modified_Data_Len + 1);
	for (int i = 0, j = 0; i < dataLen; i += chunkLength, j += chunkLength + N - 1)
	{
		for (int k = 0; k < chunkLength; k++)
		{
			newData[j + k] = data[i + k];
		}
		for (int k = 0; k < N - 1; k++)
			newData[j + chunkLength + k] = '0';

		char remainder[N + 1];
		remainder[N] = '\0';
		CRC(divisor, newData + j, N, chunkLength + N - 1, remainder);
		for (int k = 0; k < N - 1; k++)
		{
			newData[j + chunkLength + k] = remainder[k + 1];
		}
	}

	memset(inbuf, 0, len);
	for (int i = 0; i < modified_Data_Len; i++)
		inbuf[i] = newData[i];
	printf("%s\n", inbuf);
	free(newData);

	return;
}


void CRC(char divisor[], char data[], int N, int dataLen, char remainder[])
{
	char dividend[N + 1];
	dividend[N] = '\0';

	// sub dividend
	for (int i = 0; i < N; i++)
		dividend[i] = data[i];

	// divisor,dividend -> remainder
	if (dividend[0] == '1')
	{
		for (int j = 0; j < N; j++)
		{
			if (dividend[j] - divisor[j])
				remainder[j] = '1';
			else
				remainder[j] = '0';
		}
	}
	else
	{
		for (int j = 0; j < N; j++)
			remainder[j] = dividend[j];
	}

	for (int i = N; i < dataLen; i++)
	{
		// sub dividend
		for (int j = 1; j < N; j++)
			dividend[j - 1] = remainder[j];
		dividend[N - 1] = data[i];

		// divisor,dividend -> remainder
		if (dividend[0] == '1')
		{
			for (int j = 0; j < N; j++)
			{
				if (dividend[j] - divisor[j])
					remainder[j] = '1';
				else
					remainder[j] = '0';
			}
		}
		else
		{
			for (int j = 0; j < N; j++)
				remainder[j] = dividend[j];
		}
	}

	return;
}

