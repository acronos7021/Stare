#include <iostream>
#include <cstdlib>
#include <thread>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include <stdio.h>
#include "CMDparser.h"


struct dataSocket
{
  int descriptor;
  sockaddr_in info;
};

class serverv2 {

public:

	void initEngine(CMDparser *cmd);
	void startServer();
	void listening();

private:
	CMDparser* cmd;
	sockaddr_in info;
        int descriptor;

	char buffer[3000];

	const char* dataInChars;

	void sendData();
	void recvData(int client_socket);

};

