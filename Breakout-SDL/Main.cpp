#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "GameManager.h"
#include "RectangleShape.h"
#include "DEMO.h"
#include "Scene.h"

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;
	if (TTF_Init() < 0)
		return -1;
	if (!GameManager::Instance().Init()) return -1;
	RedRectangleScene scene;
	scene.Play();
	scene.Free();
	GameManager::Instance().Free();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
