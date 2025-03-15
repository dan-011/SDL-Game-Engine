#include "FontManager.h"

FontManager::FontManager() {}
FontManager::~FontManager() {}


FontManager& FontManager::Instance() {
	static FontManager instance;
	return instance;
}


TTF_Font* FontManager::GetFont(std::string path, int fontSize) {
	std::pair<std::string, int> key(path, fontSize);
	
	if (fonts.find(key) == fonts.end()) {
		std::string msg = "Attempting to create font " + path + " with size " + std::to_string(fontSize);
		//SDL_Log(msg.c_str());
		TTF_Font* font = NULL;
		font = TTF_OpenFont(path.c_str(), fontSize);
		if(font == NULL) {
			msg = "Failure to create font " + path + " with size " + std::to_string(fontSize);
			//SDL_Log(msg.c_str());
		}
		else {
			msg = "Successfully created font " + path + " with size " + std::to_string(fontSize);
			//SDL_Log(msg.c_str());
		}
		fonts.insert({ key, font });
	}
	return fonts[key];
}

void FontManager::DestroyFonts() {
	for (auto& font : fonts) TTF_CloseFont(font.second);
}