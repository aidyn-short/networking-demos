#include "LTexture.h"


//Initializes variables
LTexture::LTexture() 
{
	mTexture = NULL;
	mHeight = 0;
	mWidth = 0;
}

//Deallocates memory
LTexture::~LTexture() {
	free();
}

bool LTexture::loadFromRenderedText(SDL_Renderer* renderer, std::string textureText, SDL_Color textColor, TTF_Font* font) {
	free();




	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		std::cout << "Unable to render text Surface";
	}
	else
	{
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mTexture == NULL)
		{
			std::cout << "Failed to create texture from textSurface";
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return mTexture != NULL;

}


void LTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue) 
{

	SDL_SetTextureColorMod(mTexture, red, green, blue);

}


void LTexture::setBlendMode(SDL_BlendMode blending)
{

	SDL_SetTextureBlendMode(mTexture, blending);

}

void LTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);

}


//Loads image at specified path
bool LTexture::loadFromFile(SDL_Renderer* renderer, std::string path) {
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());



	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load img";
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		
		
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << "Unable to create texture from loadedSurface";
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	

	}

	mTexture = newTexture;
	return mTexture != NULL;



}

//Deallocates texture
void LTexture::free() {
	if (mTexture !=NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

//Renders texture at given point
void LTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{

SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);

	//SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
}