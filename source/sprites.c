#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "definitions.h"
#include "sprites.h"
#include "objectSnake.h"

#include "snake.h"

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

void saveIntoMemorySnake()
{
    dmaCopyHalfWords(DMA_CHANNEL,
                     snakePal,
                     SPRITE_PALETTE,
                     snakePalLen);
}

void displaySnake(Snake *sprite)
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

void hideSnake(Snake *sprite)
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
           true,
           false,
           false,
           false);
}

void initSprites()
{
    oamInit(&oamMain, SpriteMapping_1D_128, false);   // Jadanik oamUpdate(&oamMain) exekutatzen du
    oamInit(&oamSub, SpriteMapping_1D_128, false);    // Jadanik oamUpdate(&oamSub) exekutatzen du

    initSnake(&snake, (u8*) snakeTiles);
}