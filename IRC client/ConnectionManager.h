/*
*
*  UNUSED
*
*/
#pragma once
#include <string>
#include <WinSock2.h>
#include <thread>
#include "MessageStruct.h"
#include "Context.h"


class ConnectionManager {
public:
	ConnectionManager(Context* _parent);

	~ConnectionManager()
	{
		WSACleanup();
	}

	bool Connect(std::string address);
	void Disconnect() {
		closesocket(sock);
		sock = SOCKET_ERROR;
	}
	void recvLoop();

	Message createMessage(std::string input, std::string channel);

	void sendMessage(Message msg);
private:
	Message splitMessage(std::string message);
	bool connected;
	SOCKET sock;
	WSADATA wsaData;
	std::thread netRecv;
	Context* parent;
	std::string nick;
};