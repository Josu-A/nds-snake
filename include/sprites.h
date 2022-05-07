#ifndef _SPRITES_H_
#define _SPRITES_H_

#include "objectSnake.h"

extern void initSnake(Snake* sprite, u8* gfx);
extern void animateSnake(Snake* sprite);
extern void saveIntoMemorySnake();
extern void displaySnake(Snake *sprite);
extern void hideSnake(Snake *sprite);

extern void initSprites();

#endif