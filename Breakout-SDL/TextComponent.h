#pragma once
#include <string>
#include "TextureComponent.h"
#include "Color.h"

// you can change the text, color, font, or fontsize - but it will require creating a new font texture
class TextComponent : public TextureComponent {
public:
	TextComponent(const std::string& text, const Color& color, const std::string& fontPath, int fontSize, const Vector2f& pos, int _layer = 0, bool visibility = true);
	virtual ~TextComponent();

	std::string GetText();
	void SetText(std::string text);
	Color GetColor();
	void SetColor(const Color& color);
	std::string GetFont();
	void SetFont(std::string fontPath);
	int GetFontSize();
	void SetFontSize(int fontSize);
	virtual SDL_Rect* GetDstRect() override;
	virtual void SetPos(float x, float y) override;
	virtual void SetPos(const Vector2f& pos) override;
	virtual SDL_Texture* CreateTexture() override;

private:
	std::string textStr;
	Color clr;
	std::string fontPth;
	int size;
	SDL_Rect rect; // TODO: may be able to delete this by constructing a new object everytime
};