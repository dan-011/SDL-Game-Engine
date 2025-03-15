#pragma once
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>
#include "MathHelper.h"

class FontManager {
public:
	static FontManager& Instance();
	~FontManager();

	TTF_Font* GetFont(std::string path, int fontSize);
	void DestroyFonts();

private:
	FontManager();

	std::unordered_map<std::pair<std::string, int>, TTF_Font*, PairHash> fonts;
};
