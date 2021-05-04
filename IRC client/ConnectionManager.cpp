/*
*
*  UNUSED
*
*/
#include "ConnectionManager.h"

#include <WS2tcpip.h>
#include <thread>
#include <iostream>
#include "IRCClient.h"

ConnectionManager::ConnectionManager(Context* _parent) {
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	connected = false;
	parent = _parent;
}

bool ConnectionManager::Connect(std::string address) {
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
		netRecv = std::thread(&ConnectionManager::recvLoop, this);
		connected = true;
	}
	freeaddrinfo(result);
	return connected;
}

void ConnectionManager::recvLoop() {
	char buff[512]{};
	int i = 0;
	std::string msg;
	while ((recv(sock, &buff[i], 1, 0)) >= 0) {
		if (buff[i] == '\n' && buff[i - 1] == '\r') {
			msg.assign(buff, i + 1);
			std::cout << buff;
			parent->parent->MessageRecieved(splitMessage(msg));
			ZeroMemory(buff, i + 1);
			i = 0;
		}
		else {
			i++;
		}
	}
}

Message ConnectionManager::createMessage(std::string input, std::string channel) {
	Message msg;
	//if (input.empty()) return msg;
	if (input[0] == '/') {
		input.erase(0, 1);
		size_t offset = input.find(' ');
		if (input.substr(0, offset) == "join") {
			msg.code = "JOIN";
			input.erase(0, offset+1);
			while (offset != std::string::npos) {
				offset = input.find(' ');
				msg.params.push_back(input.substr(0, offset));
			}
		}else
		if (input.substr(0, offset) == "quit") {
			msg.code = "QUI::T";
			if (offset == std::string::npos) {
				input.erase(0, offset);
			}
			else {
				input.erase(0, offset + 1);
				msg.tail = input;
			}
		}else
		if (input.substr(0, offset) == "part") {
			msg.code = "PART";
			msg.params.push_back(channel);
		}
	}else {
		msg.code = "PRIVMSG";
		msg.params.push_back(channel);
		msg.tail = input;
		parent->channelManager->AddMessage(channel, "<" + nick + ">(You) " + input);
	}
	return msg;
}

void ConnectionManager::sendMessage(Message msg) {
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

Message ConnectionManager::splitMessage(std::string message) {
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
