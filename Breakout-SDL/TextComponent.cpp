#include "TextComponent.h"
#include "FontManager.h"
#include "GameManager.h"

TextComponent::TextComponent(const std::string& text, const Color& color, const std::string& fontPath, int fontSize, const Vector2f& pos, int _layer, bool visibility) : textStr(text), clr(color), fontPth(fontPath), size(fontSize), TextureComponent(pos, _layer, visibility), rect({(int)pos.x, (int)pos.y, 0, 0}) {
	SetNewTexture(CreateTexture()); // TODO: see how to abstract this out;
}
TextComponent::~TextComponent() {}

std::string TextComponent::GetText() {
	return textStr;
}
void TextComponent::SetText(std::string text) {
	textStr = text;
	SetNewTexture(CreateTexture());
}
Color TextComponent::GetColor() {
	return clr;
}
void TextComponent::SetColor(const Color& color) {
	clr = color;
	SetNewTexture(CreateTexture());
}
std::string TextComponent::GetFont() {
	return fontPth;
}
void TextComponent::SetFont(std::string fontPath) {
	fontPth = fontPth;
	SetNewTexture(CreateTexture());
}
int TextComponent::GetFontSize() {
	return size;
}
void TextComponent::SetFontSize(int fontSize) {
	size = fontSize;
	SetNewTexture(CreateTexture());
}
SDL_Rect* TextComponent::GetDstRect() {
	return &rect;
}
void TextComponent::SetPos(float x, float y) {
	rect.x = (int)x;
	rect.y = (int)y;
	TextureComponent::SetPos(x, y);
}
void TextComponent::SetPos(const Vector2f& pos) {
	rect.x = (int)pos.x;
	rect.y = (int)pos.y;
	TextureComponent::SetPos(pos);
}
SDL_Texture* TextComponent::CreateTexture() {
	if(!FontManager::Instance().GetFont(fontPth, size)) return NULL;
	SDL_Surface* surface = TTF_RenderText_Solid(FontManager::Instance().GetFont(fontPth, size), textStr.c_str(), { clr.R, clr.G, clr.B, clr.A });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(GameManager::Instance().GetRenderer(), surface);
	SetDims((float)surface->w, (float)surface->h);
	rect = { (int)GetPos().x, (int)GetPos().y, surface->w, surface->h };
	SDL_FreeSurface(surface);

	return texture;
}