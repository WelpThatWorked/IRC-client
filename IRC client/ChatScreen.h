/*
*
*  UNUSED
*
*/
#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "UI/Window.h"
#include "UI/Element.h"
#include "IRCClient.h"
#include "Channels.h"

class ChatWindow : public UI::Window {
public:
	ChatWindow(SDL_Renderer* renderer, IRCClient* _parent, TTF_Font* font);
private:

	void changeChannel(std::string channel);

	void Send_OnClick(UI::Element* elem);

	void Channel_OnClick(UI::Element* elem);

	void ChannelEventHdlr(std::string chan, ChannelEvent evt);

	std::string curchannel;
	TTF_Font* _font;
};