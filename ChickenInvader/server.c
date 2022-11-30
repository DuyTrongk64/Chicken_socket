/*UDP Echo Server*/
#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 5550  /* Port that will be opened */ 
#define BUFF_SIZE 1024

int main()
{
 
	int server_sock; /* file descriptors */
	char buff[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size;
	char server_mesg[256] = "Server connected successfully!\n";

	/* :Construct a UDP socket
		-Make IPv4 Internet protocols.
		-Supports datagrams (data truyền theo từng gói một).
		-Only a single protocol exists to support a particular
		socket type within a given protocol family.
  */
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == -1) {
		// Call socket() to create a socket
		perror("\nError: ");
		return 0;
	}

	//Define the address of the server

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;

	bind(server_sock, (struct sockaddr*)&server, sizeof(server));
	printf("\nServer is running on port %d\n", PORT);

	int client_socket;
	client_socket = accept(server_sock, NULL, NULL);
	send(client_socket, server_mesg, sizeof(server_mesg), 0);
	
	do
	{
		listen(server_sock, 5);
		read(client_socket, buff, 1024);
		printf("%s\n", buff);
		return 0;
	} while (1);
	
	return 0;
}
