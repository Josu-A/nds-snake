#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "objectSnake.h"

Snake snake;
SNAKE_ROTATE_PERMISSION canSnakeRotate = SNAKE_CAN_ROTATE;

void setPositionSnake(Snake *snake, int x, int y)
{
	snake->x = x;
	snake->y = y;
}

void setDefaultRotationSnake(Snake *sprite)
{
	sprite->state = W_RIGHT;
}

void moveSnake(Snake *snake)
{
	if (snake->state == W_RIGHT && snake->x <= SCREEN_WIDTH - SNAKE_DIMENSION)
	{
		if (snake->x < SCREEN_WIDTH - SNAKE_DIMENSION)
		{
			snake->x += SNAKE_DIMENSION;
			canSnakeRotate = SNAKE_CAN_ROTATE;
		}
		else
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
	}
	else if (snake->state == W_LEFT && snake->x >= 0)
	{
		if (snake->x > 0) {
			snake->x -= SNAKE_DIMENSION;
			canSnakeRotate = SNAKE_CAN_ROTATE;
		}
		else
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
	}
	else if (snake->state == W_DOWN && snake->y <= SCREEN_HEIGHT - SNAKE_DIMENSION)
	{
		if (snake->y < SCREEN_HEIGHT - SNAKE_DIMENSION)
		{
			snake->y += SNAKE_DIMENSION;
			canSnakeRotate = SNAKE_CAN_ROTATE;
		}
		else
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
	}
	else if (snake->state == W_UP && snake->y >= 0)
	{
		if (snake->y > 0)
		{
			snake->y -= SNAKE_DIMENSION;
			canSnakeRotate = SNAKE_CAN_ROTATE;
		}
		else
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
	}
}

void rotateSnake(Snake* sprite)
{
	int keys = keysDown();
	if (keys && canSnakeRotate == SNAKE_CAN_ROTATE)
	{
		if (keys & KEY_RIGHT && sprite->state != W_LEFT)
		{
			sprite->state = W_RIGHT;
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
		}	
		else if (keys & KEY_LEFT && sprite->state != W_RIGHT)
		{
			sprite->state = W_LEFT;
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
		}
		else if (keys & KEY_DOWN && sprite->state != W_UP)
		{
			sprite->state = W_DOWN;
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
		}
		else if (keys & KEY_UP && sprite->state != W_DOWN)
		{
			sprite->state = W_UP;
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
		}
	}
}

void changeAnimationFrameSnake(Snake *snake)
{
	snake->animFrame++;
	if(snake->animFrame >= FRAMES_PER_ANIMATION)
		snake->animFrame = 0;
}