#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "UI/Element.h"
#include "UI/Button.h"
#include "UI/Rectangle.h"
#include "UI/Text.h"

class Channel {
public:
	Channel(std::string name);
	void AddMessage(std::string message);
	UI::Element* CreateMessageUI(size_t messageIndex, SDL_Renderer* renderer, UI::Window* _root, TTF_Font* font);
	UI::Element* CreateChannelUI(std::function<void(UI::Element*)> callback, UI::Window* _root, SDL_Renderer* renderer, TTF_Font* font) {
		UI::Element* root = new UI::Button(callback, _name, renderer, _root, 0, 0, 384, 54);
		root->AddElement(new UI::Text(font, _name, false, renderer, _root, 0, 0, 384, 54));
		return root;
	}
	const std::vector<std::string>& GetMessages() const;
	const std::string& GetName() const;
private:
	std::vector<std::string> messages;
	std::string _name;
};