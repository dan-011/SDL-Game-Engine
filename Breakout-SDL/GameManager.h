#pragma once
#include <SDL.h>

enum {
	SCREEN_WIDTH = 960,
	SCREEN_HEIGHT = 544
};

class GameManager {
public:
	static GameManager& Instance();
	~GameManager();
	bool Init();

	bool Free();

	SDL_Window* GetWindow();
	SDL_Renderer* GetRenderer();
	

private:
	GameManager();

	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;

};