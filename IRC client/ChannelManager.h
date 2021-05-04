#pragma once
#include <vector>
#include "Channel.h"
#include <functional>

enum class ChannelEvent {
	MessageAdded,
	ChannelAdded,
	ChannelRemoved
};

class IRCClient;

class ChannelManager {
public:
	ChannelManager(IRCClient* _parent) {
		parent = _parent;
		channels.push_back(new Channel("SERVER INFO"));
	}
	Channel* GetChannel(std::string name);
	void AddMessage(std::string channame, std::string msg);
	void AddChannel(std::string name);
	void RemoveChannel(std::string name);
private:
	std::vector<Channel*> channels;
	IRCClient* parent;
};