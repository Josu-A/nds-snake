#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia
	
#include "definitions.h"
#include "peripherals.h"
#include "graphics.h"
#include "zerbitzuErrutinak.h"
#include "backgrounds.h"
#include "sprites.h"

int AUTOMATON_STATE;

SNAKE_ROTATE_PERMISSION canSnakeRotate = SNAKE_CAN_ROTATE;
touchPosition PANT_DAT;

void rotateSnake(Snake* sprite)
{
	int keys = keysHeld();
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

void jokoa01()
{	
	// Hasierako egoera ezarri
	AUTOMATON_STATE = AUTOMATON_START;
	
	// Eten zerbitzu-errutinak ezarri
	setInterruptionServiceRutines();
	// SELECT eta START teklak etenen bidez sinkronizatu
	configureKeys(KEY_SELECT | KEY_START);
	// Timer0 denboragailua konfiguratzen da zehaztutako eten segundurako
	int latch = (2^16) - (2^25) / (ETEN_SEGUNDOKO * (2^10));
	configureTimer0(latch, 0x1<<6 | 0x3<<0);
	// Teklatuaren etenak baimendu
	allowInterruptsKeys();
	// 0 denboragailuaren etenak baimendu eta martxan jarri
	allowInterruptsTimer0();
	startClockTimer0();
	
	while(1)
	{
		if (AUTOMATON_STATE == AUTOMATON_START)
		{
			// Nothing for now		
		}
		else if (AUTOMATON_STATE == AUTOMATON_SELECTION)
		{
			// Nothing for now
		}
		else if (AUTOMATON_STATE == AUTOMATON_PLAYING)
		{
			scanKeys();
			rotateSnake(&snake);
			animateSnake(&snake);
			displaySnake(&snake);
		}
		else if (AUTOMATON_STATE == AUTOMATON_PAUSED)
		{
			// Nothing for now
		}
		else if (AUTOMATON_STATE == AUTOMATON_ENDING)
		{
			// Nothing for now
		}

		swiWaitForVBlank();

		oamUpdate(&oamMain);
 		oamUpdate(&oamSub);
	}

	// Teklatuaren etenak galarazi
	denyInterruptsKeys();
	// 0 denboragailuaren etenak galarazi
	denyInterruptsTimer0();
}