#pragma once
#include <SDL.h>
#include <string>
#include <functional>
#include <concepts>
#include "Element.h"

namespace UI {

	class Button : public Element {
	public:
		Button(std::function<void(Element*)> _callback, SDL_Renderer* renderer, Window* _root, int x, int y, int w, int h) : Element(renderer, _root, x, y, w, h) {
			callback = _callback;
		}
		Button(std::function<void(Element*)> _callback, std::string name, SDL_Renderer* renderer, Window* _root, int x, int y, int w, int h) : Element(name, renderer, _root, x, y, w, h) {
			callback = _callback;
		}
		void Draw() {}
		bool OnClick() {
			callback(this);
			return true;
		}
	private:
		std::function<void(Element*)> callback;
		//void(*callback)(Element*) ;
	};
}