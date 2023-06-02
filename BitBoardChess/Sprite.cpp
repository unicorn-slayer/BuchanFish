
#include "Sprite.h"
#include "Graphics.h"

Sprite::Sprite()
{

}

Sprite::Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) :
	_x(posX),
	_y(posY),
	_visible(true)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;

	SDL_Surface* loadedSurface = graphics.loadImage(filePath);

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), loadedSurface);
	if (this->_spriteSheet == NULL) {
		printf("\nError: Unable to load image\n");
	}
}

Sprite::~Sprite()
{
}

void Sprite::draw(Graphics& graphics, int x, int y)
{

	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w, this->_sourceRect.h };
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);

}
