#include <iostream>
#include <cstdlib>

#pragma comment(lib, "Ws2_32.lib")

#include <sdkddkver.h>
#include <conio.h>
#include <stdio.h>

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#define SCK_VERSION2 0x0202;

using namespace std;

class server {

public:

	void initEngine();
	void startServer();
	void startClient();
	string getData();
	void setData(string data);

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
	SOCKET server_socket_connect;
	SOCKET client_socket_connect;

//--------------------------------------------------
// recvDataSize get you the receive data size.
// recvDataString get you the receive data in string.
// buffer is the holder for receive data.
//--------------------------------------------------
	int recvDataSize;
	string recvDataString;
	char buffer[3000];


//------------------------------------------------------------
// dataToSend is obvious.
// dataInChars is constant pointer of dataToSend and
//	it is needed because the method send only accept this type.
// dataSendSize is the the size of the dataToSend size.
//------------------------------------------------------------
	string dataToSend;
	int dataSendSize;
	const char* dataInChars;
	int sent_ok;
	int recv_OK;

//-----------------------------------------------------------
// private helpers methods.
//-----------------------------------------------------------
	void sendData();
	void recvData();


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

	server_socket_connect = socket(AF_INET, SOCK_STREAM, NULL);
	server_socket_listen = socket(AF_INET, SOCK_STREAM, NULL);


	bind(server_socket_listen, (SOCKADDR*)&ip_address, ip_address_size);
	listen(server_socket_listen, SOMAXCONN);

}

void server::startClient() {

	client_socket_connect = socket(AF_INET, SOCK_STREAM, NULL);
	connect(client_socket_connect, (SOCKADDR*)&ip_address, ip_address_size);
}

void server::sendData() {

	dataInChars = dataToSend.c_str();
	dataSendSize = dataToSend.length();
	sent_ok = send(client_socket_connect, dataInChars, dataSendSize, NULL);
	/*
	// There is a bug in the code below. Will fix soon.
	if (sent_ok != -1) cout << "sent successfully" << endl;
		else {
				cout << "Error!!! didn't send sucessfully. Is the server running?" << endl;
				cout << "Press Y to send again or Q to exit" << endl;
				char input;
				cin >> input; 
				input = tolower(input);
				if (input == 'y') {
					cout << "Attempting to send again..... ";
					setData(dataToSend);
				}
				else {
					cout << "Quitting.... " << endl;
					exit(1);
				}
			}
	*/
}
//----------------------------------------------------------------------------------------------------
// this method receives data from the socket. You have to tell it where the message ends
// otherwise it will print out garbage because of the unused buffer size initialized at the beginning.
//----------------------------------------------------------------------------------------------------

void server::recvData() {


	cout << "Waiting for client to connect " << endl;
	if (server_socket_connect = accept(server_socket_listen, (SOCKADDR*)&ip_address, &ip_address_size)) {
		cout << "A connection is found " << endl;
		int sent_ok = recvDataSize = recv(server_socket_connect, buffer, sizeof(buffer) - 1, NULL);
		// will implement error checking later. 
		if (recvDataSize >= 0) buffer[recvDataSize] = '\0';
		recvDataString = buffer;
		dataToSend = "Hello client, I have received your data ";
		dataInChars = dataToSend.c_str();
		dataSendSize = dataToSend.length();
		sent_ok = send(server_socket_connect, dataInChars, dataSendSize, NULL);
	}
}

string server::getData() {
	recvData();
  	return recvDataString;
}

void server::setData(string data) {

	dataToSend = data;
	sendData();
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
string recvData = object.getData();
cout << recvData;
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
