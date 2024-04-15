#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>



SDL_Texture* LoadTexture(const char* NameOfTexture, SDL_Renderer* renderer);

SDL_Texture** LoadAnimation(const char* NameOfTexture, SDL_Renderer* renderer, int Frames);