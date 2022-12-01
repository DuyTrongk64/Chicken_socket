#ifndef SOC_CONNECT
#define SOC_CONNECT

#include<string>
#include<sstream>
#include<bits/stdc++.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define SERV_PORT 5550
#define SERV_IP "127.0.0.1"
#define BUFF_SIZE 1024
using namespace std;

class Soc_connect
{
private:
	//Socket

	int bytes_sent, bytes_received, sin_size;
	int client_sock;
	struct sockaddr_in server_addr;
	char buff[BUFF_SIZE];
	char server_response[256];
public:
	Soc_connect();
	virtual ~Soc_connect();

	//Socket connect
	void con_socket();
};

#endif // !SOC_CONNECT