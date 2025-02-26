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
		SDL_SetSurfaceBlendMode(loadedSurface, SDL_BLENDMODE_BLEND);

		
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
		SDL_DestroySurface(loadedSurface);

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
void LTexture::render(SDL_Renderer* renderer ,int x, int y) {
	SDL_FRect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderTexture(renderer, mTexture, NULL, &renderQuad);
}