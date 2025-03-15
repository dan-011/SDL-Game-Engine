#include "FontManager.h"

FontManager::FontManager() {}
FontManager::~FontManager() {}


FontManager& FontManager::Instance() {
	static FontManager instance;
	return instance;
}


TTF_Font* FontManager::GetFont(std::string path, int fontSize) {
	std::pair<std::string, int> key(path, fontSize);
	if (fonts.find(key) == fonts.end()) fonts.insert({ key, TTF_OpenFont(path.c_str(), fontSize) });
	return fonts[key];
}

void FontManager::DestroyFonts() {
	for (auto& font : fonts) TTF_CloseFont(font.second);
}