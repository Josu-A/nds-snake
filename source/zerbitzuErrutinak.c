#include <nds.h>
#include <stdio.h>

#include "definitions.h"
#include "peripherals.h"
#include "backgrounds.h"
#include "sprites.h"

static int seg1 = 0;

void pressStartAnimate()
{
	seg1++;
	if (seg1 == SEG_0_5)
	{
		iprintf("\x1b[%d;%dH%s", 11, 4, "Sakatu START jarraitzeko");
	}
	else if (seg1 == SEG_1)
	{
		iprintf("\x1b[2J");
		seg1 = 0;
	}
}

void moveSnake()
{
	int keys = keysHeld();
	if (keys & KEY_RIGHT)
	{
		if (snake.x < SCREEN_WIDTH - SNAKE_DIMENSION)
			snake.x++;
		snake.state = W_RIGHT;
	}
	if (keys & KEY_LEFT)
	{
		if (snake.x > 0)
			snake.x--;
		snake.state = W_LEFT;
	}
	if (keys & KEY_DOWN)
	{
		if (snake.y < SCREEN_HEIGHT - SNAKE_DIMENSION)
			snake.y++;
		snake.state = W_DOWN;
	}
	if (keys & KEY_UP)
	{
		if (snake.y > 0)
			snake.y--;
		snake.state = W_UP;
	}
}

void changeAnimationFrameSnake()
{
	snake.animFrame++;
	if(snake.animFrame >= FRAMES_PER_ANIMATION)
		snake.animFrame = 0;
}

void etenTek()
{
	if (AUTOMATON_STATE == AUTOMATON_START)
	{
		if (keysCurrent() & KEY_START)
			AUTOMATON_STATE = AUTOMATON_SELECTION;
	}
	/*if (AUTOMATON_STATE == AUTOMATON_PLAYING)
	{
		moveSnake();
		changeAnimationFrameSnake();
	}*/
}

void etenTenp()
{
	if (AUTOMATON_STATE == AUTOMATON_START)
	{
		pressStartAnimate();
	}

	if (AUTOMATON_STATE == AUTOMATON_PLAYING)
	{
		int static speedCounter;
		speedCounter++;
		if (speedCounter >= ANIMATION_SPEED)
		{
			changeAnimationFrameSnake();
			speedCounter = 0;
		}

		if (snake.state == W_RIGHT && snake.x <= SCREEN_WIDTH - SNAKE_DIMENSION)
		{
			if (snake.x < SCREEN_WIDTH - SNAKE_DIMENSION)
			{
				snake.x += SNAKE_DIMENSION;
				canSnakeRotate = SNAKE_CAN_ROTATE;
			}
			else
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
		}
		else if (snake.state == W_LEFT && snake.x >= 0)
		{
			if (snake.x > 0) {
				snake.x -= SNAKE_DIMENSION;
				canSnakeRotate = SNAKE_CAN_ROTATE;
			}
			else
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;

		}
		else if (snake.state == W_DOWN && snake.y <= SCREEN_HEIGHT - SNAKE_DIMENSION)
		{
			if (snake.y < SCREEN_HEIGHT - SNAKE_DIMENSION)
			{
				snake.y += SNAKE_DIMENSION;
				canSnakeRotate = SNAKE_CAN_ROTATE;
			}
			else
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
		}
		else if (snake.state == W_UP && snake.y >= 0)
		{
			if (snake.y > 0)
			{
				snake.y -= SNAKE_DIMENSION;
				canSnakeRotate = SNAKE_CAN_ROTATE;
			}
			else
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
		}
	}
}

void setInterruptionServiceRutines()
{
	irqSet(IRQ_KEYS, etenTek);
	irqSet(IRQ_TIMER0, etenTenp);
}