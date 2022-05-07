#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia
	
#include "definitions.h"
#include "peripherals.h"
#include "graphics.h"
#include "serviceRutines.h"
#include "backgrounds.h"
#include "gui.h"
#include "objectSnake.h"
#include "sprites.h"


AUTOMATON_STATES AUTOMATON_STATE;
touchPosition PANT_DAT;

GAME_MODES selectedGameMode = GAMEMODE_NONE;

int score;
int endingTimer;
int endingTimerNormalized;
int timeLeftToPlay;
int timeLeftToPlayNormalized;

void resetGameConfig()
{
	score = 0; // Reset score to 0
	endingTimer = MAX_END_TIME; // Reset timer used to get back to the main menu when
	                            // each game finishes to its max value, in interruptions
	endingTimerNormalized = MAX_END_TIME_NORMALIZED; // Reset timer used to get back to the main menu when
													 // each game finishes to its max value, in seconds
	timeLeftToPlay = MAX_GAME_TIME; // Reset the time left to play to its max value, in interruptions
	timeLeftToPlayNormalized = MAX_GAME_TIME_NORMALIZED; // Reset the time left to play to its max value, in seconds
	setPositionSnake(&snake, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	setDefaultRotationSnake(&snake);
}

bool touchedTouchScreen(touchPosition *touchPad)
{
	return (touchPad->px != 0 || touchPad->py != 0);
}

void jokoa01()
{	
	// Hasierako egoera ezarri
	AUTOMATON_STATE = AUTOMATON_START;
	
	setInterruptionServiceRutines(); // Eten zerbitzu-errutinak ezarri
	configureKeys(KEY_SELECT | KEY_START); // SELECT eta START teklak etenen bidez sinkronizatu
	// Timer0 denboragailua konfiguratzen da zehaztutako eten segundurako
	int latch = (1<<16) - (1<<25) / (ETEN_SEGUNDOKO * (1<<10));
	configureTimer0(latch, 0x1<<6 | 0x3<<0);
	allowInterruptsKeys(); // Teklatuaren etenak baimendu
	allowInterruptsTimer0(); // 0 denboragailuaren etenak baimendu eta martxan jarri
	startClockTimer0();

	// Touch botoiak sortu
	createButtons();
	saveIntoMemorySnake();
	
	while(1)
	{
		if (AUTOMATON_STATE == AUTOMATON_START)
		{
			// Nothing for now
		}
		else if (AUTOMATON_STATE == AUTOMATON_SELECTION)
		{
			resetGameConfig();

			showButton(&buttonSelectModeLimited, &bottomScreenConsole);
			showButton(&buttonSelectModeUnlimited, &bottomScreenConsole);

			while (selectedGameMode == GAMEMODE_NONE)
			{
				touchRead(&PANT_DAT);
				if (touchedInsideButton(&buttonSelectModeLimited, &PANT_DAT))
				{
					selectedGameMode = GAMEMODE_LIMITED; // Configure game for Limited mode
				}
				else if (touchedInsideButton(&buttonSelectModeUnlimited, &PANT_DAT))
				{
					selectedGameMode = GAMEMODE_UNLIMITED; // Configure game for Unlimited mode
				}
			}

			hideButton(&buttonSelectModeLimited, &bottomScreenConsole);
			hideButton(&buttonSelectModeUnlimited, &bottomScreenConsole);

			showButton(&buttonGameTime, &topScreenConsole);
			AUTOMATON_STATE = AUTOMATON_PLAYING; // Change automaton state to playing
		}
		else if (AUTOMATON_STATE == AUTOMATON_PLAYING)
		{
			scanKeys();
			rotateSnake(&snake);
			animateSnake(&snake);
			displaySnake(&snake);

			if (selectedGameMode == GAMEMODE_LIMITED && score == MAX_SCORE)
			{
				AUTOMATON_STATE = AUTOMATON_ENDING;
			}
		}
		else if (AUTOMATON_STATE == AUTOMATON_PAUSED)
		{
			touchRead(&PANT_DAT);
			if (touchedTouchScreen(&PANT_DAT) && ~(keysCurrent() & KEY_LID))
			{
				hideButton(&buttonResumeGame, &bottomScreenConsole);
				AUTOMATON_STATE = AUTOMATON_PLAYING;
			}
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