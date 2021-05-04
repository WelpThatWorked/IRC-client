#pragma once
#pragma message ("Scrreeeen")
#include <SDL.h>
#include <queue>
#include <functional>
#include "Element.h"

namespace UI {
	class Application;

	class Window {
	public:
		Window(const char* title, int x, int y, int w, int h, Uint32 flags, SDL_Color _color, Application* _parent, Window* _owner);
		~Window();
		virtual void HandleEvent(SDL_Event e);
		void Draw();
		void AddUpdate(std::function<void()> func) {
			updateQ.push(func);
		}
		Element* GetElementByName(std::string name);
		void SetFocus(Element* p) {
			focus = p;
		}
		Element* GetFocus() {
			return focus;
		}
		SDL_Window* window;
		Window* owner;
		std::vector<Window*> ownedWindows;
		Application* parent;
		Element* root, * focus;
		SDL_Color clearColor;
		SDL_Renderer* _renderer;
		std::queue<std::function<void()>> updateQ;
	};
}