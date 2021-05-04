#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "Element.h"
#include <string>
#include "Functions.h"

namespace UI {

	class Text : public Element {
	public:
		Text(TTF_Font* font, std::string str, bool _editable, SDL_Renderer* r, Window* _root, int x, int y, int w, int h) : Element(r, _root, x, y, w, h) {
			_font = font;
			text = str;
			editable = _editable;
			redraw = true;
			cursorpos = str.length();
		}
		Text(TTF_Font* font, std::string str, bool _editable, std::string name, SDL_Renderer* r, Window* _root, int x, int y, int w, int h) : Element(name, r, _root, x, y, w, h) {
			_font = font;
			text = str;
			editable = _editable;
			redraw = true;
			cursorpos = str.length();
		}
		~Text() {
			SDL_DestroyTexture(texture);
		}
		void UpdateText(std::string str) {
			text = str;
			cursorpos = str.length();
			redraw = true;
		}
		void Draw();

		bool OnClick();

		void HandleEvent(SDL_Event e);
		const std::string& GetText() const {
			return text;
		}
	private:
		std::string text;
		bool editable, redraw;
		SDL_Texture* texture;
		TTF_Font* _font;
		int texture_w, texture_h;
		int cursorpos;
	};
}