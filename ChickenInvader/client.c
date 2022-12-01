#include<stdio.h>



#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "Game.h"
#define SERV_PORT 5550
#define SERV_IP "127.0.0.1"
#define BUFF_SIZE 1024

int main()
{
	// Step 1: Construct a UDP socket
	if ((this->client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		// Call socket() to create a socket
		perror("\nError: ");
		return 0;
	}

	// Step 2: Define the address of the server
	//bzero(&server_addr, sizeof(server_addr));
	this->server_addr.sin_family = AF_INET;
	this->server_addr.sin_port = htons(SERV_PORT);
	this->server_addr.sin_addr.s_addr = inet_addr(SERV_IP); // converse ip adderss from string to network ip (int)
	printf("Server IP: %s - Port: %d\n", SERV_IP, SERV_PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, SERV_IP, &this->server_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported\n");
		return -1;
	}

	if (connect(this->client_sock, (struct sockaddr*)&this->server_addr, sizeof(this->server_addr)) < 0)
	{
		printf("Connection Failed\n");
		return -1;
	}

	recv(this->client_sock, &this->server_response, sizeof(this->server_response), 0);

	printf("Tu server: %s\n", server_response);
	char password[BUFF_SIZE] = "test send mes";
	Game game;
	while (1)
	{
		printf("%s", game.point);
		send(this->client_sock, password, sizeof(password), 0);

		recv(this->client_sock, &this->server_response, sizeof(this->server_response), 0);
		printf("Tu server: %s\n", server_response);
	}
	close(this->client_sock);
	return 1;
}
