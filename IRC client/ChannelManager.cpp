#include "ChannelManager.h"
#include "IRCClient.h"
#include "MainWindow.h"

Channel* ChannelManager::GetChannel(std::string name) {
	for (Channel* chan : channels) {
		if (chan->GetName() == name) return chan;
	}
	AddChannel(name);
	return channels.back();
}

void ChannelManager::AddMessage(std::string channame, std::string msg) {
	Channel* chan = GetChannel(channame);
	chan->AddMessage(msg);
	if (chan == parent->curChannel) {
		auto elem = chan->CreateMessageUI(chan->GetMessages().size() - 1, ((MainWindow*)parent->GetMainWindow())->GetRenderer(), parent->GetMainWindow(), ((MainWindow*)parent->GetMainWindow())->font);
		parent->GetMainWindow()->AddUpdate([this, elem]() {
			parent->GetMainWindow()->GetElementByName("MsgBox")->AddElement(elem);
		});
	}
}

void ChannelManager::AddChannel(std::string name) {
	channels.push_back(new Channel(name));
	auto elem = channels[channels.size() - 1]->CreateChannelUI(std::bind(&MainWindow::Channel_OnClick, static_cast<MainWindow*>(parent->mainWindow), std::placeholders::_1) , ((MainWindow*)parent->GetMainWindow()), ((MainWindow*)parent->GetMainWindow())->GetRenderer(), ((MainWindow*)parent->GetMainWindow())->font);
	parent->GetMainWindow()->AddUpdate([this, elem]() {
		parent->GetMainWindow()->GetElementByName("ChannelBox")->AddElement(elem);
	});
}

void ChannelManager::RemoveChannel(std::string name) {
	auto elem = parent->GetMainWindow()->GetElementByName(name);
	std::string curChannelName = parent->curChannel->GetName();
	parent->GetMainWindow()->AddUpdate([this, elem, name, curChannelName]() {
		elem->GetParent()->RemoveElement(elem);
		if (curChannelName == name) ((MainWindow*)parent->GetMainWindow())->ChangeChannel("SERVER INFO");
	});
	auto chaniter = std::remove_if(channels.begin(), channels.end(), [name](Channel* x) {return x->GetName() == name; });
	delete *chaniter;
	channels.erase(chaniter);
}