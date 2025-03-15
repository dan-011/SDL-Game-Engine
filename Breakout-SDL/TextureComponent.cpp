#include "TextureComponent.h"
#include "GameManager.h"

TextureComponent::TextureComponent(const Vector2f& pos, int _layer, bool visibility) : VisualComponent2D(pos, Vector2f(0, 0), _layer, visibility), texture(NULL) {}
TextureComponent::~TextureComponent() {
	if (texture) SDL_DestroyTexture(texture);
}

void TextureComponent::Render() {
	if (!texture) return;

	unsigned int flip = SDL_FLIP_NONE;
	if (GetHFlip()) flip |= SDL_FLIP_HORIZONTAL;
	if (GetVFlip()) flip |= SDL_FLIP_VERTICAL;

	SDL_RenderCopyEx(GameManager::Instance().GetRenderer(), texture, GetSrcRect(), GetDstRect(), GetAngle(), GetCenterPoint(), (SDL_RendererFlip)flip);
}
SDL_Rect* TextureComponent::GetDstRect() { return NULL; }
SDL_Rect* TextureComponent::GetSrcRect() { return NULL; }
double TextureComponent::GetAngle() { return 0.0; }
SDL_Point* TextureComponent::GetCenterPoint() { return NULL; }
bool TextureComponent::GetHFlip() { return false; }
bool TextureComponent::GetVFlip() { return false; }
void TextureComponent::SetNewTexture(SDL_Texture* newTexture) {
	if (texture) SDL_DestroyTexture(texture);
	texture = newTexture;
}