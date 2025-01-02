#include "GameManager.h"
#include <iostream>
using namespace std;

GameManager::GameManager() : gameWindow(NULL), gameRenderer(NULL) {}

GameManager::~GameManager() {
	if (!(gameWindow && gameRenderer)) Free();
}

GameManager& GameManager::Instance() {
	static GameManager instance;
	return instance;
}

bool GameManager::Init() {
	Free();
	if ((gameWindow = SDL_CreateWindow("Bear Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) == NULL)
		return false;

	/*if ((gameRenderer = SDL_CreateRenderer(gameWindow, -1, 0)) == NULL)
		return false;*/
	if ((gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
		cout << "Renderer failed" << endl;
		return false;
	}
	return true;
}

bool GameManager::Free() {
	if (gameRenderer) {
		SDL_DestroyRenderer(gameRenderer);
		gameRenderer = NULL;
	}
	if (gameWindow) {
		SDL_DestroyWindow(gameWindow);
		gameWindow = NULL;
	}

	return true;
}

SDL_Window* GameManager::GetWindow() {
	return gameWindow;
}
SDL_Renderer* GameManager::GetRenderer() {
	return gameRenderer;
}
