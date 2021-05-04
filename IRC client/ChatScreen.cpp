/*
*
*  UNUSED
*
*/
#include "ChatWindow.h"

#include "UI/Rectangle.h"
#include "UI/Scroller.h"
#include "UI/Button.h"
#include "UI/Text.h"
#include "Channel.h"

ChatWindow::ChatWindow(SDL_Renderer* renderer, IRCClient* _parent, TTF_Font* font) : UI::Window(renderer, _parent) {
	root = new UI::Rectangle(SDL_Color{ 0x43,0x62,0x90,0xFF }, renderer, 0, 0, 1536, 972));
	root->GetElements()[2]->GetElements()[0]->AddElement(new UI::Scroller("MsgBox", renderer, 0, 0, 1536, 972));
	root->GetElements()[2]->AddElement(new UI::Rectangle(SDL_Color{ 0x33, 0x4B, 0x6E, 0xFF }, renderer, 1536, 0, 384, 972));
	root->GetElements()[2]->GetElements()[1]->AddElement(new UI::Scroller("ChannelBox", renderer, 0, 0, 384, 972));
	root->GetElements()[2]->GetElements()[1]->GetElements()[0]->AddElement(parent->channelManager->GetChannel("SERVER INFO")->CreateChannelUI(&ChatWindow::Channel_OnClick, this, renderer, font));
	root->GetElements()[2]->AddElement(new UI::Rectangle(SDL_Color{ 0x25, 0x36, 0x4F, 0xFF }, renderer, 0, 972, 1920, 108));
	root->GetElements()[2]->GetElements()[2]->AddElement(new UI::Rectangle(SDL_Color{ 0x90,0x90,0x90,0xFF }, renderer, 15, 11, 1604, 81));
	root->GetElements()[2]->GetElements()[2]->GetElements()[0]->AddElement(new UI::Text(font, "", true, "Input", renderer, 0, 0, 1604, 81));
	root->GetElements()[2]->GetElements()[2]->AddElement(new UI::Button(&ChatWindow::Send_OnClick, this, renderer, 1737, 11, 168, 81));
	root->GetElements()[2]->GetElements()[2]->GetElements()[1]->AddElement(new UI::Rectangle(SDL_Color{ 0x38, 0x59, 0x8A, 0xFF }, renderer, 0, 0, 168, 81));
	root->GetElements()[2]->GetElements()[2]->GetElements()[1]->GetElements()[0]->AddElement(new UI::Text(font, "Send", false, renderer, 0, 0, 168, 81));

	curchannel = "SERVER INFO";
	
	_font = font;

	parent->channelManager->SetWatcher(&ChatWindow::ChannelEventHdlr, this);
}

inline void ChatWindow::changeChannel(std::string channel) {
	UI::Element* msgbox = root->GetElementByName("MsgBox");
	msgbox->ClearElements();
	Channel* chan = parent->channelManager->GetChannel(channel);
	for (size_t i = 0; i < chan->GetMessages().size(); i++) {
		msgbox->AddElement(chan->CreateMessageUI(i, _renderer, this, _font));
	}
	curchannel = channel;
}

inline void ChatWindow::Send_OnClick(UI::Element* elem) {
	parent->connectionManager->sendMessage(parent->connectionManager->createMessage(((UI::Text*)root->GetElementByName("Input"))->GetText(), curchannel));
	((UI::Text*)root->GetElementByName("Input"))->UpdateText("");
}

void ChatWindow::Channel_OnClick(UI::Element* elem) {
	changeChannel(elem->GetName());
}

void ChatWindow::ChannelEventHdlr(std::string chan, ChannelEvent evt)
{
		switch (evt) {
		case ChannelEvent::MessageAdded:
			if (chan == curchannel) {
				auto elem = parent->channelManager->GetChannel(chan)->CreateMessageUI(parent->channelManager->GetChannel(chan)->GetMessages().size() - 1, _renderer, _font);
				updateQ.push([elem, this]() {root->GetElementByName("MsgBox")->AddElement(elem); });
			}
			break;
		case ChannelEvent::ChannelAdded: {
			auto elem = parent->channelManager->GetChannel(chan)->CreateChannelUI(&ChatWindow::Channel_OnClick, this, _renderer, _font);
			updateQ.push([chan, elem, this]() {
				root->GetElementByName("ChannelBox")->AddElement(elem);
				changeChannel(chan);
				});
			break;
		}
		case ChannelEvent::ChannelRemoved:
			updateQ.push([chan, this]() {
				root->GetElementByName("ChannelBox")->RemoveElement(root->GetElementByName(chan));
				if (chan == curchannel) changeChannel("SERVER INFO");
			});
			break;
		}
}
