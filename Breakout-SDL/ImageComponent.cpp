#include "ImageComponent.h"
#include "GameManager.h"
#include <SDL_image.h>

ImageComponent::ImageComponent(const std::string& path, const Vector2f& pos, double _angle, bool horizontalFlip, bool verticalFlip, SDL_Point* centerPoint, int _layer, bool visibility) : imgPath(path), TextureComponent(pos, _layer, visibility), dstRect({ (int)pos.x, (int)pos.y, 0, 0 }), angle(_angle), hFlip(horizontalFlip), vFlip(verticalFlip) {
	SetNewTexture(CreateTexture()); // TODO: see how to abstract this out;
}
ImageComponent::~ImageComponent() {}

SDL_Rect* ImageComponent::GetDstRect() {
	return &dstRect;
}
double ImageComponent::GetAngle() {
	return angle;
}
void ImageComponent::SetAngle(double _angle) { angle = _angle; }
void ImageComponent::SetPos(float x, float y) {
	dstRect.x = (int)x;
	dstRect.y = (int)y;
	TextureComponent::SetPos(x, y);
}
void ImageComponent::SetPos(const Vector2f& pos) {
	dstRect.x = (int)pos.x;
	dstRect.y = (int)pos.y;
	TextureComponent::SetPos(pos);
}
// virtual SDL_Point* GetCenterPoint() override; TODO
bool ImageComponent::GetHFlip() { return hFlip; }
void ImageComponent::SetHFlip(bool horizontalFlip) { hFlip = horizontalFlip; }
bool ImageComponent::GetVFlip() { return vFlip; }
void ImageComponent::SetVFlip(bool verticalFlip) { vFlip = verticalFlip; }
SDL_Texture* ImageComponent::CreateTexture() {
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(imgPath.c_str());
	texture = SDL_CreateTextureFromSurface(GameManager::Instance().GetRenderer(), surface);
	SetDims((float)surface->w, (float)surface->h);
	dstRect = { (int)GetPos().x, (int)GetPos().y, surface->w, surface->h };
	SDL_FreeSurface(surface);

	return texture;
}