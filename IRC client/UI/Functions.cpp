#include "Functions.h"
#include <filesystem>
#include <Windows.h>

//Convert From UI system measurements, which are mapped on a 1920x1080 grid, to actual window size
void UI::ConvertPixels(SDL_Renderer* _renderer, int* x, int* y) {
	int w, h;
	SDL_GetRendererOutputSize(_renderer, &w, &h);
	if (x) *x = *x * w / 1920;
	if (y) *y = *y * h / 1080;
}

bool UI::Initialize() {
	//ensure working directory is on executable
	wchar_t buff[MAX_PATH];
	ZeroMemory(buff, sizeof(buff));
	GetModuleFileName(NULL, buff, sizeof(buff) / sizeof(wchar_t));
	auto lastchar = wcsrchr(buff, '\\');
	*(lastchar + 1) = '\0';
	std::filesystem::current_path(buff);

	if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) return false;
	return true;
}

void UI::Shutdown() {
	TTF_Quit();
	SDL_Quit();
}
