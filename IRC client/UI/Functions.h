#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

namespace UI {
	
	//Convert From UI system measurements, which are mapped on a 1920x1080 grid, to actual window size
	void ConvertPixels(SDL_Renderer* _renderer, int* x, int* y);

	bool Initialize();
	void Shutdown();
}