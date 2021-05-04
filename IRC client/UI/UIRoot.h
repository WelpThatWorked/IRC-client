/*
*
*  UNUSED
*
*/
#pragma once
#include <SDL.h>
#include <queue>
#include "UIElement.h"

class Root : public Element {
public:
	Root(SDL_Color color, SDL_Renderer* renderer) : Element(renderer, 0, 0, 1920, 1080) {
		_color = color;
		focused_element = NULL;
	}
	void Draw() {
		SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
		SDL_RenderClear(_renderer);
	}
	void OnClick(){
		focused_element = NULL;
		SDL_StopTextInput();
	}
	void SetFocus(Element* elem);
	Element* GetFocus() {
		return focused_element;
	}
	Element* GetElementByName(std::string name);
private:
	Element* focused_element;
	SDL_Color _color;
};