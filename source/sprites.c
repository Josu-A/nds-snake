#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "definitions.h"
#include "sprites.h"

#include "snake.h"

Snake snake;

void initSnake(Snake* sprite, u8* gfx)
{
    sprite->spriteGfxMem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
    sprite->frameGfx = (u8*) gfx;
}

void animateSnake(Snake* sprite)
{
    int frame = sprite->animFrame + sprite->state * FRAMES_PER_ANIMATION;
	u8* offset = sprite->frameGfx + frame * SNAKE_DIMENSION*SNAKE_DIMENSION;
    dmaCopyHalfWords(DMA_CHANNEL,
                     offset,
                     sprite->spriteGfxMem,
                     SNAKE_DIMENSION*SNAKE_DIMENSION);
}

void showSnake()
{
    dmaCopyHalfWords(DMA_CHANNEL,
                     snakePal,
                     SPRITE_PALETTE,
                     snakePalLen);
}

void configureSnake(Snake* sprite)
{
    sprite->x = SCREEN_WIDTH / 2;
    sprite->y = SCREEN_HEIGHT / 2;
    initSnake(&snake, (u8*) snakeTiles);
}

void displaySnake(Snake* sprite)
{
    oamSet(&oamMain,
           0,
           sprite->x,
           sprite->y,
           0,
           0,
           SpriteSize_32x32,
           SpriteColorFormat_256Color,
           sprite->spriteGfxMem,
           -1,
           false,
           false,
           false,
           false,
           false);
}