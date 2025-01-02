#include <SDL.h>
#include <vector>
#include "GameManager.h"
#include "RectangleShape.h"
#include "DEMO.h"
#include "Scene.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;
	if (!GameManager::Instance().Init()) return -1;
	RedRectangleScene scene;
	scene.Play();
	GameManager::Instance().Free();
	SDL_Quit();
	return 0;
}
