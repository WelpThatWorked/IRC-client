#include "Text.h"

#include "Window.h"

namespace UI {

	void Text::Draw() {
		if (redraw) {
			SDL_DestroyTexture(texture);
			SDL_Surface* var = TTF_RenderText_Solid(_font, text.c_str(), SDL_Color{ 0,0,0 });
			texture = SDL_CreateTextureFromSurface(_renderer, var);
			SDL_FreeSurface(var);
			SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);
			redraw = false;
		}
		int x = GetTotalX();
		int y = GetTotalY();
		int w = _w;
		int h = _h;
		float scale = (float)_h / texture_h;
		int scaled_tex_w = scale * texture_w;
		int cx = clip_x;
		int cy = clip_y;
		int cw = clip_w;
		int ch = clip_h;
		UI::ConvertPixels(_renderer, &x, &y);
		UI::ConvertPixels(_renderer, &w, &h);
		UI::ConvertPixels(_renderer, &scaled_tex_w, NULL);
		UI::ConvertPixels(_renderer, &cx, &cy);
		UI::ConvertPixels(_renderer, &cw, &ch);
		SDL_Rect srcrect{ clip_x / scale, clip_y / scale, texture_w - (clip_w / scale), texture_h - (clip_h / scale) };
		SDL_Rect rect{ x + cx,y + cy,w - (cw + cx),h - (ch + cy) };
		if (scaled_tex_w > (w - cw)) {
			srcrect.w = (_w - clip_w) / scale;
		}
		else {
			rect.w = scaled_tex_w;
		}
		SDL_RenderCopy(_renderer, texture, &srcrect, &rect);
		if (editable) {
			Element* i = _parent;
			while (i->GetParent() != NULL) i = i->GetParent();
			if (root->GetFocus() == this) {
				int sub_w;
				TTF_SizeText(_font, text.substr(0, cursorpos).c_str(), &sub_w, NULL);
				sub_w *= scale;
				if (sub_w <= _w) {
					UI::ConvertPixels(_renderer, &sub_w, NULL);
					SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
					SDL_RenderDrawLine(_renderer, x + sub_w, y, x + sub_w, y + h);
				}
			}
		}
	}
	bool Text::OnClick() {
		if (editable) {
			root->SetFocus(this);
			SDL_StartTextInput();
			return true;
		}
		else {
			Element::OnClick();
		}
	}
	void Text::HandleEvent(SDL_Event e) {
		switch (e.type) {
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_BACKSPACE && cursorpos > 0) {
				text.erase(cursorpos - 1);
				cursorpos--;
				redraw = true;
			}
			else if (e.key.keysym.sym == SDLK_v && e.key.keysym.mod & KMOD_CTRL) {
				text.insert(cursorpos, SDL_GetClipboardText());
				cursorpos += strlen(SDL_GetClipboardText());
				redraw = true;
			}
			break;
		case SDL_TEXTINPUT:
			if (!(SDL_GetModState() & KMOD_CTRL)) {
				text.insert(cursorpos, e.text.text);
				cursorpos++;
				redraw = true;
			}
			break;
		}
	}
}