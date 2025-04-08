#pragma once
#include <SDL2/SDL.h>
#include "Texture.h"
#include <SDL2/SDL_ttf.h>


class Text
{
public:
	Text(SDL_Renderer* renderer, std::string fontName, int fontSize, SDL_Color textColor, std::string text);
	~Text();


	void setPosition(int x, int y) { mPosition.x = x; mPosition.y = y; }

	void render(SDL_Renderer* renderer);

	void SetDimensions(int width, int height) { this->width = width; this->height = height; }

	int getTextWidth() { return texture.getWidth(); }
	int getTextHeight() { return texture.getHeight(); }

	int getWidth() { return width; }
	int getHeight() { return height; }

	void SetBackgroundColor(SDL_Color backgroundColor) { this->backgroundColor = backgroundColor; }

private:

	SDL_Point mPosition;

	Texture texture;

	int width = 300;
	int height = 100;

	SDL_Color backgroundColor = { 0,0,0,0 };

	TTF_Font* font;
	SDL_Color textColor;

};

Text::Text(SDL_Renderer* renderer, std::string fontName, int fontSize, SDL_Color textColor, std::string text)
{

	mPosition.x = 0;
	mPosition.y = 0;

	this->textColor = textColor;

	font = TTF_OpenFont(fontName.c_str(), fontSize);
	if (font == NULL)
	{
		std::cout << "failed to load font";
	}
	else
	{
		if (!texture.loadFromRenderedText(renderer, text, this->textColor, this->font))
		{



			std::cout << "Failed to load text texture";
		}
	}


}



void Text::render(SDL_Renderer* renderer)
{


	SDL_Rect fillRect = { mPosition.x + texture.getWidth() / 2 - width / 2, mPosition.y + texture.getHeight() / 2 - height / 2, width, height };
	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	SDL_RenderFillRect(renderer, &fillRect);

	texture.render(renderer, mPosition.x, mPosition.y);




}




Text::~Text()
{
	TTF_CloseFont(font);
	font = NULL;
}