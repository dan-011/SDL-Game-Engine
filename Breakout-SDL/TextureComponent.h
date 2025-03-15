#pragma once
#include "VisualComponent.h"
#include <SDL.h>

class TextureComponent : public VisualComponent2D {
public:
	TextureComponent(const Vector2f& pos, int _layer = 0, bool visibility = true);
	virtual ~TextureComponent();

	virtual void Render() override;
	virtual SDL_Rect* GetDstRect(); // subclass defines this
	virtual SDL_Rect* GetSrcRect();
	virtual double GetAngle();
	virtual SDL_Point* GetCenterPoint();
	virtual bool GetHFlip();
	virtual bool GetVFlip();
	// void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
	virtual SDL_Texture* CreateTexture() = 0;

protected:
	void SetNewTexture(SDL_Texture* newTexture);

private:
	SDL_Texture* texture;
};