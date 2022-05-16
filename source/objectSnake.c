#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "objectSnake.h"
#include "definitions.h"

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
 * \param x posizio horizontala, 0tik 15ra (SCREEN_WIDTH / SNAKE_DIMENSION - 1)
 * \param y posizio bertikala,  0tik 11ra (SCREEN_HEIGHT / SNAKE_DIMENSION - 1)
 */
static void setPositionSnake(Snake *snake, int x, int y)
{
	snake->snakeHead.x = x * SNAKE_DIMENSION;
	snake->snakeHead.y = y * SNAKE_DIMENSION;

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
static void setDefaultRotationSnake(Snake *snake)
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

void updateRotationStateSnakeBody(Snake *snake)
{
	// Sugeak biratzeko aukera badu
	if (canSnakeRotate == SNAKE_CAN_ROTATE)
	{
		// if-elseif sententzia honek sugearen gorputzaren biraketa kontrolatuko du
		// Sprite horizontalera bira
		if ((snake->snakeBody[0].state == W_BODY_HORIZONTAL && snake->snakeHead.y == snake->snakeBody[0].y) // Horizontalean doa
			|| (snake->snakeBody[0].state == W_BODY_NE && snake->snakeHead.y == snake->snakeBody[0].y) // Goi-behetik eskuinera biratu eta eskuinera doa
			|| (snake->snakeBody[0].state == W_BODY_ES && snake->snakeHead.y == snake->snakeBody[0].y) // Behe-gotik eskuinera biratu eta eskuinera doa
		    || (snake->snakeBody[0].state == W_BODY_SW && snake->snakeHead.y == snake->snakeBody[0].y) // Behe-gotik ezkerrera biratu eta ezkerrera doa
		   	|| (snake->snakeBody[0].state == W_BODY_WN && snake->snakeHead.y == snake->snakeBody[0].y) // Goi-behetik ezkerrera biratu eta ezkerrera doa
		   )
		{
			snake->snakeBody[0].state = W_BODY_HORIZONTAL;
		}
		// Sprite bertikalera bira
		else if ((snake->snakeBody[0].state == W_BODY_VERTICAL && snake->snakeHead.x == snake->snakeBody[0].x) // Bertikalean doa
				 || (snake->snakeBody[0].state == W_BODY_NE && snake->snakeHead.x == snake->snakeBody[0].x) // Eskubi-ezkerretik gora biratu eta gorantz doa
				 || (snake->snakeBody[0].state == W_BODY_ES && snake->snakeHead.x == snake->snakeBody[0].x) // Eskubi-ezkerretik behera biratu eta beherantz doa
				 || (snake->snakeBody[0].state == W_BODY_SW && snake->snakeHead.x == snake->snakeBody[0].x) // Ezker-eskubitik behera biratu eta beherantz doa
				 || (snake->snakeBody[0].state == W_BODY_WN && snake->snakeHead.x == snake->snakeBody[0].x) // Ezker-eskubitil gora biratu eta gorantz doa
				)
		{
			snake->snakeBody[0].state = W_BODY_VERTICAL;
		}
		// Sprite ipar-ekira bira
		else if ((snake->snakeBody[0].state == W_BODY_HORIZONTAL && snake->snakeHead.x < snake->snakeTail.x && snake->snakeHead.y < snake->snakeBody[0].y) // Horizontal ezkerretik gora biratu eta gorantz doa 
				 || (snake->snakeBody[0].state == W_BODY_VERTICAL && snake->snakeHead.x > snake->snakeBody[0].x && snake->snakeHead.y > snake->snakeTail.y) // Bertikal behetik eskubira biratu eta eskubirantz doa
				 || (snake->snakeBody[0].state == W_BODY_ES && snake->snakeHead.x > snake->snakeBody[0].x && snake->snakeHead.y == snake->snakeBody[0].y) // Eskubi-ezkerretik behera biratu eta behera eskubira doa 
				 || (snake->snakeBody[0].state == W_BODY_SW && snake->snakeHead.x == snake->snakeBody[0].x && snake->snakeHead.y < snake->snakeBody[0].y) // Ezker-eskubitik behera biratu eta behera eskubira doa
				 || (snake->snakeBody[0].state == W_BODY_SW && snake->snakeHead.x > snake->snakeBody[0].x && snake->snakeHead.y == snake->snakeBody[0].y) // Behe-goitik ezkerrera biratu eta ezker gora doa
				 || (snake->snakeBody[0].state == W_BODY_WN && snake->snakeHead.x == snake->snakeBody[0].x && snake->snakeHead.y < snake->snakeBody[0].y) // Goi-behetik ezkerrera biratu eta ezker gora doa
				)
		{
			snake->snakeBody[0].state = W_BODY_NE;
		}
		// Sprite eki-hegora bira
		else if ((snake->snakeBody[0].state == W_BODY_HORIZONTAL && snake->snakeHead.x < snake->snakeTail.x && snake->snakeHead.y > snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_VERTICAL && snake->snakeHead.x > snake->snakeBody[0].x && snake->snakeHead.y < snake->snakeTail.y) // 
				 || (snake->snakeBody[0].state == W_BODY_NE && snake->snakeHead.x > snake->snakeBody[0].x && snake->snakeHead.y == snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_SW && snake->snakeHead.x == snake->snakeBody[0].x && snake->snakeHead.y > snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_WN && snake->snakeHead.x == snake->snakeBody[0].x && snake->snakeHead.y > snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_WN && snake->snakeHead.x > snake->snakeBody[0].x && snake->snakeHead.y == snake->snakeBody[0].y) // 
				)
		{
			snake->snakeBody[0].state = W_BODY_ES;
		}
		// Sprite hego-mendera bira
		else if ((snake->snakeBody[0].state == W_BODY_HORIZONTAL && snake->snakeHead.x > snake->snakeTail.x && snake->snakeHead.y > snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_VERTICAL && snake->snakeHead.x < snake->snakeBody[0].x && snake->snakeHead.y < snake->snakeTail.y) // 
				 || (snake->snakeBody[0].state == W_BODY_NE && snake->snakeHead.x == snake->snakeBody[0].x && snake->snakeHead.y > snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_NE && snake->snakeHead.x < snake->snakeBody[0].x && snake->snakeHead.y == snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_ES && snake->snakeHead.x == snake->snakeBody[0].x && snake->snakeHead.y > snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_WN && snake->snakeHead.x < snake->snakeBody[0].x && snake->snakeHead.y == snake->snakeBody[0].y) // 
				)
		{
			snake->snakeBody[0].state = W_BODY_SW;
		}
		// Sprite mende-iparrera bira
		else if ((snake->snakeBody[0].state == W_BODY_HORIZONTAL && snake->snakeHead.x > snake->snakeTail.x && snake->snakeHead.y < snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_VERTICAL && snake->snakeHead.x < snake->snakeBody[0].x && snake->snakeHead.y > snake->snakeTail.y) // 
				 || (snake->snakeBody[0].state == W_BODY_NE && snake->snakeHead.x == snake->snakeBody[0].x && snake->snakeHead.y < snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_ES && snake->snakeHead.x == snake->snakeBody[0].x && snake->snakeHead.y < snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_ES && snake->snakeHead.x < snake->snakeBody[0].x && snake->snakeHead.y == snake->snakeBody[0].y) // 
				 || (snake->snakeBody[0].state == W_BODY_SW && snake->snakeHead.x < snake->snakeBody[0].x && snake->snakeHead.y == snake->snakeBody[0].y) // 
				)
		{
			snake->snakeBody[0].state = W_BODY_WN;
		}
		
		// if-elseif sententzia honek sugearen isatsaren biraketa kontrolatuko du
		// Isatsa eskubirantz edo ezkerrerantz mugitzen ari bada
		if (snake->snakeTail.state == W_TAIL_RIGHT || snake->snakeTail.state == W_TAIL_LEFT)
		{
			/* eta bere aurreko gorputz atala, gorputza kasu honetan, y ardatzean
			   gorago badago, isatsaren egoera beheraka ezarri. */
			if (snake->snakeBody[0].y > snake->snakeTail.y)
			{
				snake->snakeTail.state = W_TAIL_DOWN;
			}

			/* eta bere aurreko gorputz atala, gorputza kasu honetan, y ardatzean
			   beherago badago, isatsaren egoera goraka ezarri. */
			else if (snake->snakeBody[0].y < snake->snakeTail.y)
			{
				snake->snakeTail.state = W_TAIL_UP;
			}
		}
		// Isatsa gorantz edo beherantz mugitzen ari bada
		else if (snake->snakeTail.state == W_TAIL_UP || snake->snakeTail.state == W_TAIL_DOWN)
		{
			/* eta bere aurreko gorputz atala, gorputza kasu honetan, x ardatzean
			   gorago badago, isatsaren egoera eskubirantz ezarri. */
			if (snake->snakeBody[0].x > snake->snakeTail.x)
			{
				snake->snakeTail.state = W_TAIL_RIGHT;
			}

			/* eta bere aurreko gorputz atala, gorputza kasu honetan, x ardatzean
			   beherago badago, isatsaren egoera ezkerrerantz ezarri. */
			else if (snake->snakeBody[0].x < snake->snakeTail.x)
			{
				snake->snakeTail.state = W_TAIL_LEFT;
			}
		}
	}
}

/*!
 * \brief Sugearen erakuslea jaso eta honi egoera berria ezarriko dio teklatutik
 * jasotako norabidera, baldin eta soilik baldin aukera badu norabidez aldatzeko.
 * 
 * \param snake suge objektuaren erakuslea
 */
void updateRotationStateSnakeHead(Snake *snake)
{
	// Sugeak biratzeko aukera badu
	if (canSnakeRotate == SNAKE_CAN_ROTATE)
	{
		int keys = keysDown(); // Teklak detektatu

		// If sententzia honek sugearen buruaren biraketa kontrolatuko du
		if (keys) // Teklarik sakatu bada
		{
			/* Eskubi tekla sakatu bada eta sugea ezkerreratz ez badoa,
			sugearen egoera eskubirantz ezarri eta biratzeko aukera kendu. */
			if (keys & KEY_RIGHT && snake->snakeHead.state != W_HEAD_LEFT)
			{
				snake->snakeHead.state = W_HEAD_RIGHT;
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			}
			/* Ezker tekla sakatu bada eta sugea eskubirantz ez badoa,
			sugearen egoera ezkerrerantz ezarri eta biratzeko aukera kendu. */
			else if (keys & KEY_LEFT && snake->snakeHead.state != W_HEAD_RIGHT)
			{
				snake->snakeHead.state = W_HEAD_LEFT;
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			}
			/* Behera tekla sakatu bada eta sugea gorantz ez badoa,
			sugearen egoera beherantz ezarri eta biratzeko aukera kendu. */
			else if (keys & KEY_DOWN && snake->snakeHead.state != W_HEAD_UP)
			{
				snake->snakeHead.state = W_HEAD_DOWN;
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
			}
			/* Gora tekla sakatu bada eta sugea beherantz ez badoa,
			sugearen egoera gorantz ezarri eta biratzeko aukera kendu. */
			else if (keys & KEY_UP && snake->snakeHead.state != W_HEAD_DOWN)
			{
				snake->snakeHead.state = W_HEAD_UP;
				canSnakeRotate = SNAKE_CAN_NOT_ROTATE;
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
	snake->snakeHead.animFrame++; // Buruaren hurrengo framearen indizea gorde
	// Buruaren azkeneko framearen indizean badago, lehenengoa ezarri
	if(snake->snakeHead.animFrame >= HEAD_FRAMES_PER_ANIMATION)
		snake->snakeHead.animFrame = 0;
	
	snake->snakeBody[0].animFrame++;  // Gorputzaren hurrengo framearen indizea gorde
	// Gorputzaren azkeneko framearen indizean badago, lehenengoa ezarri
	if (snake->snakeBody[0].animFrame >= BODY_FRAMES_PER_ANIMATION)
		snake->snakeBody[0].animFrame = 0;
	
	snake->snakeTail.animFrame++;  // Isatsaren hurrengo framearen indizea gorde
	// Isatsaren azkeneko framearen indizean badago, lehenengoa ezarri
	if(snake->snakeTail.animFrame >= TAIL_FRAMES_PER_ANIMATION)
		snake->snakeTail.animFrame = 0;
}

/*!
 * \brief Sugea berrezartzen du.
 * 
 * \param snake suge objektuaren erakuslea
 */
void resetSnake(Snake *snake)
{
	isSnakeDead = SNAKE_ALIVE;
	setPositionSnake(snake, SCREEN_WIDTH_TILES / 2, SCREEN_HEIGHT_TILES / 2);
	setDefaultRotationSnake(snake);
}