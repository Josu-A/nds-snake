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
 * \brief Sugea bizirik dagoen adiJokuak jokalariari aukera eman edo kendu egiten dio sugearen
 * norabidea aldatzeko.
 */
SNAKE_DEATH_STATE isSnakeDead = SNAKE_ALIVE;


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
	snake->snakeHead.x = x;
	snake->snakeHead.y = y;

	snake->snakeBody[0].x = snake->snakeHead.x - SNAKE_DIMENSION;
	snake->snakeBody[0].y = snake->snakeHead.y;

	snake->snakeTail.x = snake->snakeBody[0].x - SNAKE_DIMENSION;
	snake->snakeTail.y = snake->snakeBody[0].y;
}

/*!
 * \brief Sugearen erakuslea jasota honen hasierako norabide egoera
 * hasieratzen du.
 * 
 * \param snake suge objektuaren erakuslea
 */
void setDefaultRotationSnake(Snake *snake)
{
	snake->snakeHead.state = W_HEAD_RIGHT;
	snake->snakeBody[0].state = W_BODY_HORIZONTAL;
	snake->snakeTail.state = W_TAIL_RIGHT;
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
	if (snake->snakeHead.state == W_HEAD_RIGHT && snake->snakeHead.x <= SCREEN_WIDTH - SNAKE_DIMENSION)
	{
		if (snake->snakeHead.x < SCREEN_WIDTH - SNAKE_DIMENSION)
		{
			snake->snakeTail.x = snake->snakeBody[0].x;
			snake->snakeBody[0].x = snake->snakeHead.x;

			snake->snakeTail.y = snake->snakeBody[0].y;
			snake->snakeBody[0].y = snake->snakeHead.y;

			snake->snakeHead.x += SNAKE_DIMENSION;
			canSnakeRotate = SNAKE_CAN_ROTATE;
		}
		else
		{
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			isSnakeDead = SNAKE_DEAD;
		}
			
	}
	else if (snake->snakeHead.state == W_HEAD_LEFT && snake->snakeHead.x >= 0)
	{
		if (snake->snakeHead.x > 0) {
			snake->snakeTail.x = snake->snakeBody[0].x;
			snake->snakeBody[0].x = snake->snakeHead.x;

			snake->snakeTail.y = snake->snakeBody[0].y;
			snake->snakeBody[0].y = snake->snakeHead.y;

			snake->snakeHead.x -= SNAKE_DIMENSION;
			canSnakeRotate = SNAKE_CAN_ROTATE;
		}
		else
		{
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			isSnakeDead = SNAKE_DEAD;
		}
	}
	else if (snake->snakeHead.state == W_HEAD_DOWN && snake->snakeHead.y <= SCREEN_HEIGHT - SNAKE_DIMENSION)
	{
		if (snake->snakeHead.y < SCREEN_HEIGHT - SNAKE_DIMENSION)
		{
			snake->snakeTail.x = snake->snakeBody[0].x;
			snake->snakeBody[0].x = snake->snakeHead.x;

			snake->snakeTail.y = snake->snakeBody[0].y;
			snake->snakeBody[0].y = snake->snakeHead.y;

			snake->snakeHead.y += SNAKE_DIMENSION;
			canSnakeRotate = SNAKE_CAN_ROTATE;
		}
		else
		{
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			isSnakeDead = SNAKE_DEAD;
		}
	}
	else if (snake->snakeHead.state == W_HEAD_UP && snake->snakeHead.y >= 0)
	{
		if (snake->snakeHead.y > 0)
		{
			snake->snakeTail.x = snake->snakeBody[0].x;
			snake->snakeBody[0].x = snake->snakeHead.x;

			snake->snakeTail.y = snake->snakeBody[0].y;
			snake->snakeBody[0].y = snake->snakeHead.y;
			
			snake->snakeHead.y -= SNAKE_DIMENSION;
			canSnakeRotate = SNAKE_CAN_ROTATE;
		}
		else
		{
			canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			isSnakeDead = SNAKE_DEAD;
		}
	}
}

/*!
 * \brief Sugearen erakuslea jaso eta honi egoera berria ezarriko dio teklatutik
 * jasotako norabidera, baldin eta soilik baldin aukera badu norabidez aldatzeko.
 * 
 * \param snake suge objektuaren erakuslea
 */
void updateRotationStateSnake(Snake* snake)
{
	if (canSnakeRotate == SNAKE_CAN_ROTATE)
	{
		// Burua
		int keys = keysDown();
		if (keys)
		{
			if (keys & KEY_RIGHT && snake->snakeHead.state != W_HEAD_LEFT)
			{
				snake->snakeHead.state = W_HEAD_RIGHT;
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			}	
			else if (keys & KEY_LEFT && snake->snakeHead.state != W_HEAD_RIGHT)
			{
				snake->snakeHead.state = W_HEAD_LEFT;
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			}
			else if (keys & KEY_DOWN && snake->snakeHead.state != W_HEAD_UP)
			{
				snake->snakeHead.state = W_HEAD_DOWN;
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			}
			else if (keys & KEY_UP && snake->snakeHead.state != W_HEAD_DOWN)
			{
				snake->snakeHead.state = W_HEAD_UP;
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			}
		}
		
		// Gorputza
		if (snake->snakeBody[0].state == W_BODY_HORIZONTAL && snake->snakeHead.y != snake->snakeBody[0].y)
		{
			snake->snakeBody[0].state = W_BODY_VERTICAL;
		}
		else if (snake->snakeBody[0].state == W_BODY_VERTICAL && snake->snakeHead.x != snake->snakeBody[0].x)
		{
			snake->snakeBody[0].state = W_BODY_HORIZONTAL;
		}
		
		// Isatsa
		if (snake->snakeTail.state == W_TAIL_RIGHT || snake->snakeTail.state == W_TAIL_LEFT)
		{
			if (snake->snakeBody[0].y > snake->snakeTail.y)
			{
				snake->snakeTail.state = W_TAIL_DOWN;
			}
			else if (snake->snakeBody[0].y < snake->snakeTail.y)
			{
				snake->snakeTail.state = W_TAIL_UP;
			}
		}
		else if (snake->snakeTail.state == W_TAIL_UP || snake->snakeTail.state == W_TAIL_DOWN)
		{
			if (snake->snakeBody[0].x > snake->snakeTail.x)
			{
				snake->snakeTail.state = W_TAIL_RIGHT;
			}
			else if (snake->snakeBody[0].x < snake->snakeTail.x)
			{
				snake->snakeTail.state = W_TAIL_LEFT;
			}
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
	snake->snakeHead.animFrame++;
	if(snake->snakeHead.animFrame >= HEAD_FRAMES_PER_ANIMATION)
		snake->snakeHead.animFrame = 0;
	snake->snakeBody[0].animFrame++;
	if (snake->snakeBody[0].animFrame >= BODY_FRAMES_PER_ANIMATION)
		snake->snakeBody[0].animFrame = 0;
	snake->snakeTail.animFrame++;
	if(snake->snakeTail.animFrame >= TAIL_FRAMES_PER_ANIMATION)
		snake->snakeTail.animFrame = 0;
}