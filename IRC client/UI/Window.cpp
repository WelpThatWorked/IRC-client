#include "Window.h"
#include <queue>
#include <vector>
#include "Element.h"
#include "Functions.h"
#include "Application.h"

namespace UI {

	Window::Window(const char* title, int x, int y, int w, int h, Uint32 flags, SDL_Color _color, Application* _parent, Window* _owner) {
		window = SDL_CreateWindow(title, x, y, w, h, flags);
		//"WelpIRC", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE
		_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
		clearColor = _color;
		parent = _parent;
		owner = _owner;
		if (!owner) {
			parent->AddWindow(this);
		}
		else {
			owner->ownedWindows.push_back(this);
		}
		focus = nullptr;
		root = nullptr;
	}

	Window::~Window()
	{
		delete root;
		SDL_DestroyWindow(window);
		for (auto Window : ownedWindows) {
			delete Window;
		}
		if (!owner) parent->RemoveWindow(this);
	}

	void Window::HandleEvent(SDL_Event e)
	{
		switch (e.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT) {
				bool flag = true;
				UI::Element* i = root;
				while (flag) {
					const std::vector<UI::Element*> elements = i->GetElements();
					flag = false;
					for (size_t j = 0; j < elements.size(); j++) {
						if (!elements[j]->active) {
							continue;
						}
						int x = elements[j]->GetTotalX() + elements[j]->GetClipX();
						int w = elements[j]->GetW() - elements[j]->GetClipW();
						int y = elements[j]->GetTotalY() + elements[j]->GetClipY();
						int h = elements[j]->GetH() - elements[j]->GetClipH();
						UI::ConvertPixels(_renderer, &x, &y);
						UI::ConvertPixels(_renderer, &w, &h);
						if ((unsigned)(e.button.x - x) <= (w) && (unsigned)(e.button.y - y) <= (h)) {
							flag = true;
							i = elements[j];
							break;
						}
					}
				}
				while (i) {
					if (i->OnClick()) {
						return;
					}
					else {
						i = i->GetParent();
					}
				}
				focus = nullptr;
			}
			break;
		default:
			if (focus) focus->HandleEvent(e);
			break;
		}
	}

	void Window::Draw() {
		SDL_SetRenderDrawColor(_renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		SDL_RenderClear(_renderer);
		while (!updateQ.empty()) {
			updateQ.front()();
			updateQ.pop();
		}
		std::queue<Element*> q;
		q.push(root);
		while (!q.empty()) {
			Element* i = q.front();
			if (i->active) {
				i->Draw();

				std::vector<Element*> subelements = i->GetElements();
				for (size_t i = 0; i < subelements.size(); i++) {
					q.push(subelements[i]);
				}
			}
			q.pop();
		}
		SDL_RenderPresent(_renderer);
	}

	Element* Window::GetElementByName(std::string name) {
		std::queue<Element*> q;
		q.push(root);
		while (!q.empty()) {
			Element* i = q.front();
			if (i->GetName() == name) return i;
			q.pop();
			std::vector<Element*> subelements = i->GetElements();
			for (size_t i = 0; i < subelements.size(); i++) {
				q.push(subelements[i]);
			}
		}
		return NULL;
	}
}