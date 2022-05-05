#ifndef _SPRITES_H_
#define _SPRITES_H_

#include "definitions.h"

#define ANIMATION_SPEED 3
#define FRAMES_PER_ANIMATION 3
#define SNAKE_DIMENSION 32

extern void initSnake(Snake* sprite, u8* gfx);

extern void animateSnake(Snake* sprite);

extern void showSnake();

extern void configureSnake();

extern void displaySnake();

#endif