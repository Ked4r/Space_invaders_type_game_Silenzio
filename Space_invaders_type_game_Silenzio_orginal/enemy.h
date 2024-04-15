#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "texture.h"


struct circle
{
    int x, y, r;
};

typedef struct _Enemy
{
    int x, y, w, h;
    int alive;
    int Hp;
    int value;
} Enemy;


void SpawnEnemy();
