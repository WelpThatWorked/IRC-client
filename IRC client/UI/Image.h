#pragma once
#include <SDL.h>
#include <cmath>
#include "Functions.h"
#include "Element.h"

namespace UI {

	class Image : public Element {
	public:
		Image(const char* file, SDL_Renderer* r, Window* _root, int x, int y, int w, int h) : Element(r, _root, x, y, w, h) {
			auto var = SDL_LoadBMP(file);
			if (var == NULL) {
				throw(SDL_GetError());
			}
			texture = SDL_CreateTextureFromSurface(_renderer, var);
			SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
			SDL_FreeSurface(var);
		}

		~Image() {
			SDL_DestroyTexture(texture);
		}

		void Draw() {
			SDL_Rect src, dest;
			int x = GetTotalX() + clip_x;
			int y = GetTotalY() + clip_y;
			int w = _w - clip_w;
			int h = _h - clip_h;
			float ratio_w = (float)tex_w / _w;
			float ratio_h = (float)tex_h / _h;
			src = { std::ceil(clip_x * ratio_w), std::ceil(clip_y * ratio_h) , tex_w - std::ceil(clip_w * ratio_w) , tex_h - std::ceil(clip_h * ratio_h) };
			ConvertPixels(_renderer, &x, &y);
			ConvertPixels(_renderer, &w, &h);
			dest = { x,y,w,h };
			SDL_RenderCopy(_renderer, texture, &src, &dest);
		}

	private:
		int tex_w, tex_h;
		SDL_Texture* texture;
	};
}