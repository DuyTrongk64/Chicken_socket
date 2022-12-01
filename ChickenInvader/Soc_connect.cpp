#include "Soc_connect.h"

Soc_connect::Soc_connect()
{
	this->con_socket();
}

Soc_connect::~Soc_connect()
{

}

void Soc_connect::con_socket()
{
	// Step 1: Construct a UDP socket
	if ((this->client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		// Call socket() to create a socket
		perror("\nError: ");
		return ;
	}

	// Step 2: Define the address of the server
	//bzero(&server_addr, sizeof(server_addr));
	this->server_addr.sin_family = AF_INET;
	this->server_addr.sin_port = htons(SERV_PORT);
	this->server_addr.sin_addr.s_addr = inet_addr(SERV_IP); // converse ip adderss from string to network ip (int)
	std::cout << "Server IP: " << SERV_IP << " - Port: " << SERV_PORT << "\n";

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, SERV_IP, &this->server_addr.sin_addr) <= 0)
	{
		std::cout << "\nInvalid address/ Address not supported" << "\n";
		return ;
	}

	if (connect(this->client_sock, (struct sockaddr*)&this->server_addr, sizeof(this->server_addr)) < 0)
	{
		std::cout << "Connection Failed" << "\n";
		return ;
	}

	recv(this->client_sock, &this->server_response, sizeof(this->server_response), 0);

	std::cout << "Tu server: " << server_response << "\n";
	char password[BUFF_SIZE] = "test send mes";
	while (1)
	{
		send(this->client_sock, password, sizeof(password), 0);

		recv(this->client_sock, &this->server_response, sizeof(this->server_response), 0);
		std::cout << "Tu server: " << this->server_response << "\n";
	}
	//close(this->client_sock);
	return ;
}
