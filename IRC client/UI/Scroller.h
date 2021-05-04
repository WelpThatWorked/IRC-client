#pragma once
#include "Element.h"
#include <SDL.h>

namespace UI {

	class Scroller : public Element {
	public:
		Scroller(SDL_Renderer* renderer, Window* _root, int x, int y, int w, int h) : Element(renderer, _root, x, y, w, h) {
			curscrollpos = 0;
		}
		Scroller(std::string name, SDL_Renderer* renderer, Window* _root, int x, int y, int w, int h) : Element(name, renderer, _root, x, y, w, h) {
			curscrollpos = 0;
		}
		void Draw() {

		}
		void AddElement(Element* elem) override;
		void RemoveElement(Element* element) override {
			_subelements.erase(std::find(_subelements.begin(), _subelements.end(), element));
			RecalculatePositions();
		}
		void HandleEvent(SDL_Event e);
		bool OnClick();
	private:
		int TotalHeight() const;
		void RecalculatePositions();
		int curscrollpos;
	};
}