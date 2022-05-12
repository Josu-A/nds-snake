#include <nds.h>
#include <stdio.h>

#include "serviceRutines.h"
#include "definitions.h"
#include "gui.h"
#include "objectSnake.h"
#include "sprites.h"

/*!
 * \brief Etenen bidez konfiguratutako teklatuek jautiko duten funtzio nagusia.
 */
void interruptKeys()
{
	if (AUTOMATON_STATE == AUTOMATON_START)
	{
		if (keysCurrent() & KEY_START)
		{
			clearConsoleLines(&bottomScreenConsole, 0, 31, 0, 23);
			AUTOMATON_STATE = AUTOMATON_SELECTION;
		}
	}
	else if (AUTOMATON_STATE == AUTOMATON_PLAYING)
	{
		int keys = keysCurrent();
		if (keys & KEY_SELECT || keys & KEY_START || keys & KEY_LID)
		{
			showButton(&buttonResumeGame, &bottomScreenConsole);
			AUTOMATON_STATE = AUTOMATON_PAUSED;
		}
	}
	else if (AUTOMATON_STATE == AUTOMATON_ENDING)
	{
		if (keysCurrent() & KEY_SELECT)
		{
			resetGameConfig();
			clearConsoleLines(&topScreenConsole, 12, 13, 15, 15);
			showButton(&buttonGameTime, &topScreenConsole);
			AUTOMATON_STATE = AUTOMATON_PLAYING;
		}
	}
}

/*!
 * \brief 0 denboragailuak etenak sortutakoan deituko duen funtzioa.
 */
void interruptTimer0()
{
	if (AUTOMATON_STATE == AUTOMATON_START)
	{
		pressStartAnimate();
	}
	else if (AUTOMATON_STATE == AUTOMATON_PLAYING)
	{
		static int speedCounter = 0;
		speedCounter++;
		if (speedCounter >= ANIMATION_SPEED)
		{
			changeAnimationFrameSnake(&snake);
			moveSnake(&snake);
			speedCounter = 0;
		}

		timeLeftToPlay--;
		showRealTimeTimer(timeLeftToPlay, &timeLeftToPlayNormalized, &topScreenConsole, 29, 22);
		if (timeLeftToPlay == 0 || isSnakeDead == SNAKE_DEAD)
		{
			hideButton(&buttonGameTime, &topScreenConsole);
			hideSnake(&snake);
			isSnakeDead = SNAKE_DEAD;
			AUTOMATON_STATE = AUTOMATON_ENDING;
		}
	}
	else if (AUTOMATON_STATE == AUTOMATON_ENDING)
	{
		endingTimer--;
		showRealTimeTimer(endingTimer, &endingTimerNormalized, &topScreenConsole, 12, 15);

		if (endingTimer == 0)
		{
			selectedGameMode = GAMEMODE_NONE;
			clearConsoleLines(&topScreenConsole, 12, 13, 15, 15);
			AUTOMATON_STATE = AUTOMATON_SELECTION;
		}
	}
}

/*!
 * \brief Eten zerbitzu-errutinak zehazten ditu teklatuarentzako eta
 * denboragailuarentzako.
 */
void setInterruptionServiceRutines()
{
	irqSet(IRQ_KEYS, interruptKeys);
	irqSet(IRQ_TIMER0, interruptTimer0);
}