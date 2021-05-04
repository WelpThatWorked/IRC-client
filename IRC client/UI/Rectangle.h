#pragma once
#include <SDL.h>
#include "Element.h"

namespace UI {

	class Rectangle : public Element {
	public:
		Rectangle(SDL_Color c, SDL_Renderer* r, Window* _root, int x, int y, int w, int h) : Element(r, _root, x, y, w, h) {
			_color = c;
		}
		void Draw();
		SDL_Color _color;
	};
}