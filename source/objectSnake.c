#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "objectSnake.h"

/*!
 * \brief Suge objektua gordetzen du.
 */
Snake snake;

/*!
 * \brief Jokuak jokalariari aukera eman edo kendu egiten dio sugearen
 * norabidea aldatzeko. Hasieran beti izango du aukera.
 */
SNAKE_ROTATE_PERMISSION canSnakeRotate = SNAKE_CAN_ROTATE;

/*!
 * \brief Sugearen erakuslea jasota honen posizioa ezartzen da pantailan
 * emandako x eta y balioekin.
 * 
 * \param snake suge objektuaren erakuslea
 * \param x posizio horizontala, 0tik SCREEN_WIDTH (256)-ra
 * \param y posizio bertikala,  0tik SCREEN_HEIGHT (192)-ra
 */
void setPositionSnake(Snake *snake, int x, int y)
{
	snake->x = x;
	snake->y = y;
}

/*!
 * \brief Sugearen erakuslea jasota honen hasierako norabide egoera
 * hasieratzen du.
 * 
 * \param snake suge objektuaren erakuslea
 */
void setDefaultRotationSnake(Snake *sprite)
{
	sprite->state = W_RIGHT;
}

/*!
 * \brief Sugearen erakuslea jaso eta mugitu egingo du norabide egokian baldin
 * eta soilik baldin pantaila barruko esparruan badago. Hurrengo mugimenduan
 * sugeak eremutik aterako balitz, erabiltzaileari kontrola kentzen dio eta 
 * mugitzen gelditzen da.
 * 
 * \param snake suge objektuaren erakuslea
 */
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

/*!
 * \brief Sugearen erakuslea jaso eta honi egoera berria ezarriko dio teklatutik
 * jasotako norabidera, baldin eta soilik baldin aukera badu norabidez aldatzeko.
 * 
 * \param snake suge objektuaren erakuslea
 */
void updateRotationStateSnake(Snake* sprite)
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

/*!
 * \brief Sugearen norabide berdineko erakutsi beharreko sub-spritearen
 * posizioa handitzen edo reseteatzen du jolasak spritea aldatzeko (animatzeko)
 * jasotako sugearen erakuslea jasota.
 * 
 * \param snake suge objektuaren erakuslea
 */
void changeAnimationFrameSnake(Snake *snake)
{
	snake->animFrame++;
	if(snake->animFrame >= FRAMES_PER_ANIMATION)
		snake->animFrame = 0;
}