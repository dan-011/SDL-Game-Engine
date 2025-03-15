#include <psp2/kernel/processmgr.h>
#include <SDL2/SDL.h>
#include <vector>
#include "GameManager.h"
#include "RectangleShape.h"
#include "Breakout.h"
#include "Scene.h"

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;
	if (!GameManager::Instance().Init()) return -1;
	Breakout scene;
	scene.Play();
	GameManager::Instance().Free();
	SDL_Quit();
	return 0;
}
