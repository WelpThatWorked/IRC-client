#include "Channel.h"



Channel::Channel(std::string name) {
	_name = name;
}

void Channel::AddMessage(std::string message) {
	messages.push_back(message);
}

UI::Element* Channel::CreateMessageUI(size_t messageIndex, SDL_Renderer* renderer, UI::Window* _root, TTF_Font* font) {
	if (messageIndex == -1) messageIndex = messages.size() - 1;
	UI::Element* root = new UI::Rectangle(SDL_Color{ 0,0,0,0 }, renderer, _root, 0, 0, 1536, 37);
	root->AddElement(new UI::Text(font, messages[messageIndex], false, renderer, _root, 10, 5, 1506, 27));
	return root;
}

const std::vector<std::string>& Channel::GetMessages() const {
	return messages;
}

const std::string& Channel::GetName() const {
	return _name;
}