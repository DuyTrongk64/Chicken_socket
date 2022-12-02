#ifndef SERVER_H
#define SERVER_H



#include<string>
#include<sstream>         /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include<string>
#include<sstream>
#include<bits/stdc++.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include<iostream>





class Server
{
private:
	int server_sock; /* file descriptors */
	char buff[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size;
	char server_mesg[256] = "Server connected successfully!\n";

	
public:
	Server();
	virtual~Server();
	void initServer();

};

#endif // !SERVER_H