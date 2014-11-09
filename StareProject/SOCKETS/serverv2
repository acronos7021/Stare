#include <iostream>
#include <cstdlib>
#include <thread>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#define SCK_VERSION2 0x0202;

class server {

public:

	void initEngine();
	void startServer();
	void listening();

private:

	//---------------------------------------------
	// This is windows specific stuffs. 
	//---------------------------------------------
	WSAData windowsSocketData;
	WORD socketVersion;

	SOCKADDR_IN ip_address;
	int ip_address_size;

	//---------------------------------------------
	// instances for server and client to be used.
	//---------------------------------------------
	SOCKET server_socket_listen;

	//--------------------------------------------------
	// recvDataSize get you the receive data size.
	// recvDataString get you the receive data in string.
	// buffer is the holder for receive data.
	//--------------------------------------------------
	char buffer[3000];

	//------------------------------------------------------------
	// dataToSend is obvious.
	// dataInChars is constant pointer of dataToSend and
	//	it is needed because the method send only accept this type.
	// dataSendSize is the the size of the dataToSend size.
	//------------------------------------------------------------
	const char* dataInChars;

	//-----------------------------------------------------------
	// private helpers methods.
	//-----------------------------------------------------------
	void sendData();
	void recvData(SOCKET S);


};

void server::initEngine()  {

	socketVersion = MAKEWORD(2, 1);
	WSAStartup(socketVersion, &windowsSocketData);
	ip_address_size = sizeof(ip_address);

	ip_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	ip_address.sin_family = AF_INET;
	ip_address.sin_port = htons(444);

}

void server::startServer() {

	server_socket_listen = socket(AF_INET, SOCK_STREAM, NULL);

	bind(server_socket_listen, (SOCKADDR*)&ip_address, ip_address_size);
	listen(server_socket_listen, SOMAXCONN);

}


//----------------------------------------------------------------------------------------------------
// this method receives data from the socket. You have to tell it where the message ends
// otherwise it will print out garbage because of the unused buffer size initialized at the beginning.
//----------------------------------------------------------------------------------------------------

void server::recvData(SOCKET client_socket) {

	   int recvDataSize = recv(client_socket, buffer, sizeof(buffer) - 1, NULL);
		// will implement error checking later. 
		if (recvDataSize >= 0) buffer[recvDataSize] = '\0';
		std::string recvDataString = buffer;
///     Get data from the algorithm/database in here. the two lines below are for testing purpose.
		std::cout << recvDataString << std::endl;
		std::string dataToSend = "Hello client, I have received your data ";
		dataInChars = dataToSend.c_str();
		int dataSendSize = dataToSend.length();
		int sent_ok = send(client_socket, dataInChars, dataSendSize, NULL);

	}

void server::listening(){
	
	while (true) {
	std::cout << "Waiting for client to connect " << std::endl;
	if (SOCKET client_socket = accept(server_socket_listen, (SOCKADDR*)&ip_address, &ip_address_size)) {
		std::cout << "A connection is found " << std::endl;
		std::thread *tb = new std::thread(&server::recvData, this, client_socket);
	    }
	}
}

int main() {

	server object;

	//---------------------------------------------
	// To setup windows sockets information
	//---------------------------------------------

	object.initEngine();
	//----------------------------------------------------------------------------------------
	// wrap the method getData() around a while loop if you want the server to run forever.
	//---------------------------------------------------------------------------------------

	object.startServer();
    object.listening();
	//std::cout<<recvData;
	system("pause");
	//---------------------------------------------
	// To send data to the clients 
	//---------------------------------------------

	//object.startClient();
	//string data = "Hello Server !!!!";
	//bject.setData(data);
	//system("pause");
	//exit(1);
}
