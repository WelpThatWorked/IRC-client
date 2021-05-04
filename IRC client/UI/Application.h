#pragma once
#include <vector>
#include <iostream>
#include <SDL.h>
#include "Window.h"
#include "Functions.h"


namespace UI {
	class Application {
	public:
		Application(){
			mainWindow = nullptr;
			max_framerate = 30;
			min_frametime = 1000 / max_framerate;
		}
		void Run() {
			if (!Initialize()) {
				std::cout << "SDL startup Failed: " << SDL_GetError() << std::endl;
				return;
			}
			//No error return, up to implementing app to clean mess of failed startup and set quit
			OnStartup();

			SDL_Event e;

			/*frame counting
			int frames = 0;
			int lasttime = SDL_GetTicks();
			*/

			while (!quit) {
				unsigned int framestart = SDL_GetTicks();
				while (SDL_PollEvent(&e)) {
					HandleEvent(e);
				}
				std::queue<Window*> q;
				for(auto Window : windows)
				{
					Window->Draw();
				}
				
				/*frame counting
				frames++;
				if (lasttime <= (SDL_GetTicks() - 1000)) {
					lasttime = SDL_GetTicks();
					std::cout << frames << std::endl;
					frames = 0;
				}*/

				//Frame limiting
				int delay = min_frametime - (SDL_GetTicks() - framestart);
				if (delay > 0) {
					SDL_Delay(delay);
				}
			}
			for (auto Window : windows) {
				delete Window;
			}
			OnShutdown();
			Shutdown();
		}

		void AddWindow(Window* win) {
			if (!mainWindow) {
				mainWindow = win;
			}
			windows.push_back(win);
		}
		void RemoveWindow(Window* win) {
			if (win == mainWindow) {
				quit = true;
			}
			std::erase(windows, win);
		}

		virtual void OnStartup() = 0;
		virtual void OnShutdown() = 0;

		virtual void HandleEvent(SDL_Event e) {
			switch (e.type) {
			case SDL_QUIT:
			{
				quit = true;
				break;
			}
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
			case SDL_TEXTEDITING:
			case SDL_TEXTINPUT:
			case SDL_USEREVENT:
			case SDL_WINDOWEVENT:
				for (auto Window : windows) {
					if (SDL_GetWindowID(Window->window) == e.key.windowID) {
						Window->HandleEvent(e);
						break;
					}
				}
				break;
			default:
				break;
			}
		}

		//Change max Framerate. Default is 30.
		void SetFramerateLimit(unsigned int fps) {
			max_framerate = fps;
			min_frametime = 1000 / fps;
		}

		std::vector<Window*> windows;
		Window* mainWindow;
	protected:
		bool quit = false;
		unsigned int max_framerate;
		unsigned int min_frametime;
	};

}