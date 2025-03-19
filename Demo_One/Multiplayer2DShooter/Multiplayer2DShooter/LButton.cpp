#include "LButton.h"
#pragma once






LButton::LButton(SDL_Renderer* renderer,std::string fontName, int fontSize, SDL_Color textColor, std::string text)
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
		if (!texture.loadFromRenderedText(renderer, text, this->textColor, this->font ))
		{
		


			std::cout << "Failed to load text texture";
		}
	}

}

void LButton::SetText(SDL_Renderer* renderer, std::string text)
{

	if (!texture.loadFromRenderedText(renderer, text, textColor, font))
	{

		std::cout << "Failed to load text texture";
	}

}




void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;

		if (x < mPosition.x)
		{
		
			inside = false;
	
		}
		if (x > (mPosition.x + width))
		{
	
			inside = false;

		}
		if (y < mPosition.y)
		{
	
			inside = false;

		}
		if (y > (mPosition.y + height))
		{
	
			inside = false;
		
		}

		if (!inside)
		{
			color = { 255,0,0, 255 };
		
			
		}
		else
		{
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				color = { 0,255,0, 255 };
				break;

			case SDL_MOUSEBUTTONDOWN:
				color = { 0,0,255, 255 };
				break;

			case SDL_MOUSEBUTTONUP:
				color = { 0,0,0, 255 };
				break;



			default:
				break;
			}


		}

	}


}

void LButton::render(SDL_Renderer* renderer)
{


	SDL_Rect fillRect = { mPosition.x, mPosition.y, width, height};
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &fillRect);

	texture.render(renderer,mPosition.x, mPosition.y);

	TTF_CloseFont(font);
	font = NULL;


}
