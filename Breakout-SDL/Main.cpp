#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "GameManager.h"
#include "FontManager.h"
#include "Breakout.h"
#include "Scene.h"

int main(int argc, char* argv[])
{
	//SDL_Log("Starting app");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;
	const int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP;
	if (!(IMG_Init(imgFlags) & imgFlags))
		return -1;
	if (TTF_Init() < 0)
		return -1;
	if (!GameManager::Instance().Init()) return -1;
	Breakout scene;
	scene.Play();
	FontManager::Instance().DestroyFonts();
	GameManager::Instance().Free();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}
