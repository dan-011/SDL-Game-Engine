#pragma once
#include <unordered_map>
#include <SDL.h>

#define VERSION 0
#define PC 0
#define VITA 1

enum InputState {
	UNPRESSED = 0x0,
	PRESSED_ONCE = 0x1,
	PRESSED = 0x2,
};

#if VERSION == PC
class InputManager {
public:
	static InputManager& Instance();
	~InputManager();

	bool InputPressed(SDL_Scancode input);
	bool InputPressedOnce(SDL_Scancode input);

	void UpdateInputs();
	void AddInput(SDL_Scancode code);

private:
	InputManager();
	std::unordered_map<SDL_Scancode, Uint8> inputStates;
	Uint8* keyStates;
};
#endif

#if VERSION == VITA
class InputManager {
public:
	static InputManager& Instance();
	~InputManager();

	bool InputPressed(SDL_Scancode input);
	bool InputPressedOnce(SDL_Scancode input);

	void UpdateInputs();
	unsigned char GetLX();
	unsigned char GetLY();
	unsigned char GetRX();
	unsigned char GetRY();
private:
	InputManager();
	std::unordered_map<SceCtrlButtons, Uint8> inputStates;
	SceCtrlData ctrl;
};
#endif