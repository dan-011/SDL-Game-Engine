#pragma once
#include "TextureComponent.h"
#include <string>

// TODO: implement optional color keying

class ImageComponent : public TextureComponent { // might change to SpriteComponent
public:
	ImageComponent(const std::string& path, const Vector2f& pos, double _angle = 0.0, bool horizontalFlip = false, bool verticalFlip = false, SDL_Point* centerPoint = NULL, int _layer = 0, bool visibility = true);
	virtual ~ImageComponent();

	virtual SDL_Rect* GetDstRect() override; // subclass defines this
	virtual void SetPos(float x, float y) override;
	virtual void SetPos(const Vector2f& pos) override;
	// virtual SDL_Rect* GetSrcRect() override;
	virtual double GetAngle() override;
	void SetAngle(double _angle);
	// virtual SDL_Point* GetCenterPoint() override; TODO
	virtual bool GetHFlip() override;
	void SetHFlip(bool horizontalFlip);
	virtual bool GetVFlip() override;
	void SetVFlip(bool verticalFlip);
	virtual SDL_Texture* CreateTexture() override;

private:
	std::string imgPath;
	SDL_Rect dstRect;  // TODO: may be able to delete this by constructing a new object everytime
	double angle;
	bool hFlip;
	bool vFlip;
};
