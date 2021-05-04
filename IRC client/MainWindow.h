#pragma once
#include <iostream>
#include <filesystem>
#include <SDL.h>
#include "UI/Element.h"
#include "UI/Rectangle.h"
#include "UI/Text.h"
#include "UI/Button.h"
#include "UI/Scroller.h"
#include "UI/Window.h"
#include "UI/Application.h"
#include "IRCClient.h"

class MainWindow : public UI::Window {
public:
	MainWindow(IRCClient* _parent) : UI::Window("IRC Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE, SDL_Color{ 255,255,255,255 }, (UI::Application*)_parent, nullptr) {		
		std::cout << std::filesystem::current_path();
		
		font = TTF_OpenFont("res\\Roboto-Regular.ttf", 72);

		root = new UI::Rectangle(SDL_Color{ 0x43,0x62,0x90,0xFF }, _renderer, this, 0, 0, 1980, 1080);

		//Login
		root->AddElement(new UI::Rectangle(SDL_Color{ 0,0,0,0 }, _renderer, this, 0, 0, 1980, 1080));
		root->GetElements()[0]->AddElement(new UI::Text(font, "Server Address:", false, _renderer, this, 358, 300, 512, 81));
		root->GetElements()[0]->AddElement(new UI::Rectangle(SDL_Color{ 0x90,0x90,0x90,0xFF }, _renderer, this, 870, 300, 700, 81));
		root->GetElements()[0]->GetElements()[1]->AddElement(new UI::Text(font, "", true, "ServerAddress", _renderer, this, 0, 0, 700, 81));
		root->GetElements()[0]->AddElement(new UI::Text(font, "Nickname:", false, _renderer, this, 512, 500, 340, 81));
		root->GetElements()[0]->AddElement(new UI::Rectangle(SDL_Color{ 0x90,0x90,0x90,0xFF }, _renderer, this, 852, 500, 700, 81));
		root->GetElements()[0]->GetElements()[3]->AddElement(new UI::Text(font, "", true, "Name", _renderer, this, 0, 0, 700, 81));
		root->GetElements()[0]->AddElement(new UI::Button(std::bind(&MainWindow::Connect_OnClick, this, std::placeholders::_1), _renderer, this, 1612, 949, 268, 81));
		root->GetElements()[0]->GetElements()[4]->AddElement(new UI::Rectangle(SDL_Color{ 0x7f,0x7f,0x7f,0xFF }, _renderer, this, 0, 0, 268, 81));
		root->GetElements()[0]->GetElements()[4]->GetElements()[0]->AddElement(new UI::Text(font, "Connect", false, _renderer, this, 0, 0, 268, 81));

		//Connecting
		root->AddElement(new UI::Rectangle(SDL_Color{ 0,0,0,0 }, _renderer, this, 0, 0, 1980, 1080));
		root->GetElements()[1]->active = false;
		root->GetElements()[1]->AddElement(new UI::Text(font, "Connecting...", false, _renderer, this, 778, 459, 424, 81));

		//Chat
		root->AddElement(new UI::Rectangle(SDL_Color{ 0,0,0,0 }, _renderer, this, 0, 0, 1980, 1080));
		root->GetElements()[2]->active = false;
		root->GetElements()[2]->AddElement(new UI::Scroller("MsgBox", _renderer, this, 0, 0, 1536, 972));
		root->GetElements()[2]->AddElement(new UI::Rectangle(SDL_Color{ 0x33, 0x4B, 0x6E, 0xFF }, _renderer, this, 1536, 0, 384, 972));
		root->GetElements()[2]->GetElements()[1]->AddElement(new UI::Scroller("ChannelBox", _renderer, this, 0, 0, 384, 972));
		root->GetElements()[2]->GetElements()[1]->GetElements()[0]->AddElement(((IRCClient*)parent)->channels->GetChannel("SERVER INFO")->CreateChannelUI(std::bind(&MainWindow::Channel_OnClick, this, std::placeholders::_1), this, _renderer, font));
		root->GetElements()[2]->AddElement(new UI::Rectangle(SDL_Color{ 0x25, 0x36, 0x4F, 0xFF }, _renderer, this, 0, 972, 1920, 108));
		root->GetElements()[2]->GetElements()[2]->AddElement(new UI::Rectangle(SDL_Color{ 0x90,0x90,0x90,0xFF }, _renderer, this, 15, 11, 1604, 81));
		root->GetElements()[2]->GetElements()[2]->GetElements()[0]->AddElement(new UI::Text(font, "", true, "Input", _renderer, this, 0, 0, 1604, 81));
		root->GetElements()[2]->GetElements()[2]->AddElement(new UI::Button(std::bind(&MainWindow::Send_OnClick, this, std::placeholders::_1), _renderer, this, 1737, 11, 168, 81));
		root->GetElements()[2]->GetElements()[2]->GetElements()[1]->AddElement(new UI::Rectangle(SDL_Color{ 0x38, 0x59, 0x8A, 0xFF }, _renderer, this, 0, 0, 168, 81));
		root->GetElements()[2]->GetElements()[2]->GetElements()[1]->GetElements()[0]->AddElement(new UI::Text(font, "Send", false, _renderer, this, 0, 0, 168, 81));
	}

	void Connect_OnClick(UI::Element* elem);
	void Send_OnClick(UI::Element* elem);
	void Channel_OnClick(UI::Element* elem);
	void ChangeChannel(std::string name);
	void ConnectionConfirmed() {
		root->GetElements()[1]->active = false;
		root->GetElements()[2]->active = true;
	}

	SDL_Renderer* GetRenderer(){
		return _renderer;
	}

	void HandleEvent(SDL_Event e) {
		switch (e.type) {
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
				Message msg;
				msg.code = "QUIT";
				msg.params.push_back("Client Closed");
				((IRCClient*)parent)->sendMessage(msg);
			}
			break;
		default:
			Window::HandleEvent(e);
			break;
		}
	}

	TTF_Font* font;
};