#include "Rectangle.h"
#include "Functions.h"

namespace UI {

	void Rectangle::Draw() {
		int x = GetTotalX() + clip_x;
		int y = GetTotalY() + clip_y;
		int w = _w - clip_w;
		int h = _h - clip_h;
		ConvertPixels(_renderer, &x, &y);
		ConvertPixels(_renderer, &w, &h);
		SDL_Rect rect = {
			x,
			y,
			w,
			h
		};
		SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
		SDL_RenderFillRect(_renderer, &rect);
	}
}