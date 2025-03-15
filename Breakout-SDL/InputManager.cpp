#include "InputManager.h"

#if VERSION == PC
InputManager& InputManager::Instance() {
	static InputManager instance;
	return instance;
}
InputManager::~InputManager() {}

bool InputManager::InputPressed(SDL_Scancode input) {
	if (keyStates[input]) {
		if(!inputStates[input]) inputStates[input] |= PRESSED | PRESSED_ONCE;
	}
	else inputStates[input] = UNPRESSED;
	
	return inputStates[input] & PRESSED;
}
bool InputManager::InputPressedOnce(SDL_Scancode input) {
	InputPressed(input);

	bool res = inputStates[input] & PRESSED_ONCE;
	if (res) inputStates[input] ^= PRESSED_ONCE;

	return res;
}
InputManager::InputManager() : keyStates(NULL) {

}
void InputManager::UpdateInputs() {
	keyStates = const_cast<Uint8*>(SDL_GetKeyboardState(NULL));
}

void InputManager::AddInput(SDL_Scancode code) {
	inputStates.insert({ code, UNPRESSED });
}
#endif

#if VERSION == VITA
InputManager& InputManager::Instance() {
	static InputManager instance;
	return instance;
}
InputManager::~InputManager() {}

bool InputManager::InputPressed(SceCtrlButtons input) {
	if (ctrl.buttons & input) {
		if (!inputStates[input]) inputStates[input] = PRESSED | PRESSED_ONCE;
	}
	else inputStates[input] = UNPRESSED;

	return inputStates[input] & PRESSED;
}
bool InputManager::InputPressedOnce(SceCtrlButtons input) {
	InputPressed(input);

	bool res = inputStates[input] & PRESSED_ONCE;
	if (res) inputStates[input] ^= PRESSED_ONCE;

	return res;
}
InputManager::InputManager() {
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	inputStates.insert({ SCE_CTRL_SELECT, UNPRESSED });
	inputStates.insert({ SCE_CTRL_L3, UNPRESSED });
	inputStates.insert({ SCE_CTRL_R3, UNPRESSED });
	inputStates.insert({ SCE_CTRL_START, UNPRESSED });
	inputStates.insert({ SCE_CTRL_UP, UNPRESSED });
	inputStates.insert({ SCE_CTRL_RIGHT, UNPRESSED });
	inputStates.insert({ SCE_CTRL_DOWN, UNPRESSED });
	inputStates.insert({ SCE_CTRL_LEFT, UNPRESSED });
	inputStates.insert({ SCE_CTRL_LTRIGGER, UNPRESSED });
	inputStates.insert({ SCE_CTRL_L2, UNPRESSED });
	inputStates.insert({ SCE_CTRL_RTRIGGER, UNPRESSED });
	inputStates.insert({ SCE_CTRL_R2, UNPRESSED });
	inputStates.insert({ SCE_CTRL_L1, UNPRESSED });
	inputStates.insert({ SCE_CTRL_R1, UNPRESSED });
	inputStates.insert({ SCE_CTRL_TRIANGLE, UNPRESSED });
	inputStates.insert({ SCE_CTRL_CIRCLE, UNPRESSED });
	inputStates.insert({ SCE_CTRL_CROSS, UNPRESSED });
	inputStates.insert({ SCE_CTRL_SQUARE, UNPRESSED });
	inputStates.insert({ SCE_CTRL_INTERCEPTED, UNPRESSED });
	inputStates.insert({ SCE_CTRL_PSBUTTON, UNPRESSED });
	inputStates.insert({ SCE_CTRL_HEADPHONE, UNPRESSED });
	inputStates.insert({ SCE_CTRL_VOLUP, UNPRESSED });
	inputStates.insert({ SCE_CTRL_VOLDOWN, UNPRESSED });
	inputStates.insert({ SCE_CTRL_POWER, UNPRESSED });
}
void InputManager::UpdateInputs() {
	sceCtrlPeekBufferPositive(0, &ctrl, 1);
}

unsigned char InputManager::GetLX() { return ctrl.lx; }
unsigned char InputManager::GetLY() { return ctrl.ly; }
unsigned char InputManager::GetRX() { return ctrl.rx; }
unsigned char InputManager::GetRY() { return ctrl.ry; }
#endif
