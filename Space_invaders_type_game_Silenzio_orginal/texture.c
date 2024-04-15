#include "texture.h"




SDL_Texture* LoadTexture(const char* NameOfTexture, SDL_Renderer* renderer)
{
    SDL_Surface* Image;
    Image = IMG_Load(NameOfTexture);
    if (!Image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }

    return SDL_CreateTextureFromSurface(renderer, Image);
}

SDL_Texture** LoadAnimation(const char* NameOfTexture, SDL_Renderer* renderer, int Frames)
{
    SDL_Texture** Animation= (SDL_Texture**)malloc(Frames*sizeof(SDL_Texture*));
    
    for (int i = 0; i < Frames; i++)
    {
        char* a[1000];
        sprintf(a, NameOfTexture, i);   
        Animation[i] = LoadTexture(a, renderer);
    }

    return Animation;
}

