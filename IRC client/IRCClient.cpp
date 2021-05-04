#include "IRCClient.h"

#include <iostream>
#include <WS2tcpip.h>
#include "UI/Window.h"
#include "MainWindow.h"

/*void IRCClient::Run() {
	quit = false;
	MainWindow= 
	mainWindow = SDL_CreateWindow("WelpIRC", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	TTF_Init();
	font = TTF_OpenFont("arial.ttf", 72);

	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
			{
				Message msg;
				msg.code = "QUIT";
				connectionManager->sendMessage(msg);
				break;
			}
			default:
				screens[curscreen]->HandleEvent(e);
				break;
			}
		}
		screens[curscreen]->Draw();
	}
}*/

void IRCClient::OnStartup() {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup Failure" << std::endl;
		quit = true;
		return;
	};
	connected = false;
	new MainWindow(this);
}

void IRCClient::recvLoop()
{
	char buff[1024]{};
	unsigned int i = 0;
	std::string msg;
	int recved = 0;
	while ((recved = recv(sock, &buff[i], sizeof(buff) - i, 0)) >= 0) {
		i += recved;
		while (true) {
			char* pos = std::find(buff, buff + i, '\n');
			if (pos != buff + i && *(pos - 1) == '\r') {
				msg.assign(buff, pos+1);
				std::cout << msg.substr(0, msg.length() - 2) << std::endl;
				MessageRecieved(splitMessage(msg));
				std::move(pos + 1, buff + i, buff);
				i -= pos - buff + 1;
				ZeroMemory(buff + i, sizeof(buff) - i);
			}
			else {
				break;
			}
		}
	}
}

void IRCClient::MessageRecieved(Message msg) {
	if (msg.code == "PRIVMSG") {
		channels->AddMessage(msg.head.substr(0, msg.head.find('!')), "<" + msg.head.substr(0, msg.head.find('!')) + "> " + msg.tail);
		return;
	}
	//JoinSuccess
	if (msg.code == "001") {
		channels->AddMessage("SERVER INFO", msg.tail);
		mainWindow->AddUpdate([this]() {
			((MainWindow*)mainWindow)->ConnectionConfirmed();
			});
		return;
	}
	if(msg.code == "JOIN"){
		/*if (msg.head.substr(0, msg.head.find('!')) == nick){
			channelManager->AddChannel(msg.params[0]);
		}*/
		channels->AddMessage(msg.params[0], msg.head.substr(0, msg.head.find('!')) + " joined");
		return;
	}
	if (msg.code == "PART") {
		if (msg.head.substr(0, msg.head.find('!')) == nick) {
			channels->RemoveChannel(msg.params[0]);
		}
		else {
			channels->AddMessage(msg.params[0], msg.head.substr(0, msg.head.find('!')) + " left");
		}
		return;
	}
	//Topic Message
	if (msg.code == "332" || msg.code == "331") {
		channels->AddMessage(msg.params[1], msg.tail);
		return;
	}
	//Namreply
	if (msg.code == "353") {
		channels->AddMessage(msg.params[2], msg.tail);
		return;
	}
	//End of Names
	if (msg.code == "366") {
		channels->AddMessage(msg.params[1], msg.tail);
		return;
	}
	if (msg.code == "PING") {
		msg.code = "PONG";
		sendMessage(msg);
		return;
	}
	if (msg.code == "QUIT") {
		if (msg.head.substr(0, msg.head.find('!')) == nick) {
			quit = true;
		}
		else {
			channels->AddMessage("SERVER INFO", msg.head.substr(0, msg.head.find('!')) + " quit " + msg.tail);
		}
		return;
	}
	//Nick in use
	if (msg.code == "433") {
		Disconnect();
		return;
	}
	channels->AddMessage("SERVER INFO", msg.tail);
}

bool IRCClient::Connect(std::string address) {
	if (connected) return false;
	ADDRINFO hint;
	ADDRINFO* result;
	ZeroMemory(&hint, sizeof(hint));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	getaddrinfo(address.c_str(), "6667", &hint, &result);
	if (result == NULL) return false;
	for (ADDRINFO* p = result; p != NULL; p = p->ai_next) {
		char ipstr[INET6_ADDRSTRLEN];
		inet_ntop(p->ai_family, p->ai_addr->sa_data, ipstr, sizeof(ipstr));
		std::cout << ipstr << std::endl;
	}
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	//bind(ressocket, result->ai_addr, result->ai_addrlen);
	if (connect(sock, result->ai_addr, result->ai_addrlen) == 0) {
		netRecv = std::thread(&IRCClient::recvLoop, this);
		connected = true;
	}
	freeaddrinfo(result);
	return connected;
}

void IRCClient::sendMessage(Message msg)
{
	std::string _msg;
	if (!msg.head.empty()) {
		_msg.append(":" + msg.head + " ");
	}
	_msg.append(msg.code);
	for (std::string param : msg.params) {
		_msg.append(" " + param);
	}
	if (!msg.tail.empty()) {
		_msg.append(" :" + msg.tail);
	}
	_msg.append("\r\n");
	send(sock, _msg.c_str(), _msg.length(), 0);
}

Message IRCClient::createMessage(std::string input, std::string channel)
{
	Message msg;
	//if (input.empty()) return msg;
	if (input[0] == '/') {
		input.erase(0, 1);
		size_t offset = input.find(' ');
		if (input.substr(0, offset) == "join") {
			msg.code = "JOIN";
			input.erase(0, offset + 1);
			while (offset != std::string::npos) {
				offset = input.find(' ');
				msg.params.push_back(input.substr(0, offset));
			}
		}
		else
			if (input.substr(0, offset) == "quit") {
				msg.code = "QUI::T";
				if (offset == std::string::npos) {
					input.erase(0, offset);
				}
				else {
					input.erase(0, offset + 1);
					msg.tail = input;
				}
			}
			else
				if (input.substr(0, offset) == "part") {
					msg.code = "PART";
					msg.params.push_back(channel);
				}
	}
	else {
		msg.code = "PRIVMSG";
		msg.params.push_back(channel);
		msg.tail = input;
		channels->AddMessage(channel, "<" + nick + ">(You) " + input);
	}
	return msg;
}

Message IRCClient::splitMessage(std::string message)
{
	Message split;
	if (message[0] == ':') {
		message.erase(0, 1);
		split.head = message.substr(0, message.find(' '));
		message.erase(0, message.find(' ') + 1);
	}
	split.code = message.substr(0, message.find(' '));
	message.erase(0, message.find(' ') + 1);
	while (message[0] != '\r' && message[1] != '\n') {
		if (message[0] == ':') {
			message.erase(0, 1);
			split.tail = message.substr(0, message.find("\r\n"));
			break;
		}
		if (message.find(' ') != std::string::npos) {
			split.params.push_back(message.substr(0, message.find(' ')));
			message.erase(0, message.find(' ') + 1);
		}
		else {
			split.params.push_back(message.substr(0, message.find("\r\n")));
			message.erase(0, message.find("\r\n"));
		}
	}
	return split;
}
