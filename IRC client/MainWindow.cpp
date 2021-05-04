#include "MainWindow.h"

void MainWindow::Connect_OnClick(UI::Element* elem)
{
	root->GetElements()[0]->active = false;
	root->GetElements()[1]->active = true;
	std::string addr = ((UI::Text*)GetElementByName("ServerAddress"))->GetText();
	std::string nick = ((UI::Text*)GetElementByName("Name"))->GetText();
	((IRCClient*)parent)->nick = nick;
	if (((IRCClient*)parent)->Connect(addr)) {
		Message nickmsg, usermsg;
		nickmsg.code = "NICK";
		nickmsg.params.push_back(nick);
		usermsg.code = "USER";
		usermsg.params.push_back(nick);
		usermsg.params.push_back("0");
		usermsg.params.push_back("*");
		usermsg.tail = nick;
		((IRCClient*)parent)->sendMessage(nickmsg);
		((IRCClient*)parent)->sendMessage(usermsg);
		ChangeChannel("SERVER INFO"); 
	}
	else {
		root->GetElements()[0]->active = true;
		root->GetElements()[1]->active = false;
	}
}

void MainWindow::Send_OnClick(UI::Element* elem)
{
	if (!((UI::Text*)GetElementByName("Input"))->GetText().empty()) {
		((IRCClient*)parent)->sendMessage(((IRCClient*)parent)->createMessage(((UI::Text*)GetElementByName("Input"))->GetText(), ((IRCClient*)parent)->curChannel->GetName()));
		((UI::Text*)GetElementByName("Input"))->UpdateText("");
	}
}

void MainWindow::Channel_OnClick(UI::Element* elem)
{
	ChangeChannel(elem->GetName());
}

void MainWindow::ChangeChannel(std::string name) {
	UI::Element* msgbox = GetElementByName("MsgBox");
	Channel* chan = ((IRCClient*)parent)->channels->GetChannel(name);
	if (chan == ((IRCClient*)parent)->curChannel) return;
	msgbox->ClearElements();
	for (size_t i = 0; i < chan->GetMessages().size(); i++) {
		msgbox->AddElement(chan->CreateMessageUI(i, _renderer, this, font));
	}
	((IRCClient*)parent)->curChannel = chan;
}
