#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <WinSock2.h>
#include <thread>
#include "UI/Application.h"
#include "ChannelManager.h"
#include "MessageStruct.h"

class IRCClient : public UI::Application{
public:
	IRCClient() {
		wsaData = {};
		channels = new ChannelManager(this);
		sock = INVALID_SOCKET;
		connected = false;
		curChannel = nullptr;
	}

	void OnStartup();
	void OnShutdown() {
		Disconnect();
		WSACleanup();
	}

	void recvLoop();
	void MessageRecieved(Message msg);
	bool Connect(std::string address);
	void Disconnect() {
		if (connected) {
			closesocket(sock);
			netRecv.join();
			sock = INVALID_SOCKET;
			connected = false;
		}
	}

	void sendMessage(Message msg);
	Message createMessage(std::string input, std::string channel);

	UI::Window* GetMainWindow() {
		return mainWindow;
	}

	ChannelManager* channels;
	Channel* curChannel;
	std::string nick;
private:
	Message splitMessage(std::string message);
	//SDL_Renderer* renderer;
	bool connected;
	SOCKET sock;
	WSADATA wsaData;
	std::thread netRecv;
};