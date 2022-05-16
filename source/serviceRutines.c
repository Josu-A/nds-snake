#include <nds.h>
#include <stdio.h>

#include "serviceRutines.h"
#include "definitions.h"
#include "gui.h"
#include "objectSnake.h"
#include "objectApple.h"
#include "sprites.h"
#include "backgrounds.h"

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
			showSubBgGamemodeSelect();
			showMainBgGamemodeSelectModes();
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
			clearConsoleLines(&bottomScreenConsole, 0, 31, 0, 23);
			showButton(&buttonGameTime, &topScreenConsole);
			displayApple(&apple);
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
		timeLeftToPlay--;

		static int speedCounter = 0;
		speedCounter++;
		if (speedCounter >= ANIMATION_SPEED)
		{
			changeAnimationFrameSnake(&snake);
			moveSnake(&snake);
			displaySnake(&snake); // Sugearen sprite eguneratua pantailatzen da
			updateRotationStateSnakeBody(&snake);
			speedCounter = 0;

			// Sugeak sagarra jaten badu
			if (appleCollidesSnake(&apple, &snake))
			{
				score += SCORE_INCREMENT; // Puntuazioa inkrementatuko da

				/* Denbora extra gehituko da, baldin eta soilik baldin 99 segundu
				 baino gutxiago falta badira jolasa bukatzeko, eta gehienez 99
				 segundura iritsiko da gelditzen den denbora */
				if (timeLeftToPlayNormalized <= 99 - TIME_BONUS_NORMALIZED)
				{
					timeLeftToPlay += TIME_BONUS; // Denbora extra irabazi
				}
				else if (timeLeftToPlayNormalized < 99) {
					timeLeftToPlay = 99 * ETEN_SEGUNDUKO;
				}

				resetApple(&apple); // Sagarra ezkutatu eta posizio berria ezarriko zaio

				// Bukaera modua duen jolasa aukeratu ezean eta irabazi bada
				if (selectedGameMode == GAMEMODE_LIMITED && score >= MAX_SCORE)
				{
					hideButton(&buttonGameTime, &topScreenConsole);
					hideSnake(&snake);
					hideApple(&apple);
					AUTOMATON_STATE = AUTOMATON_ENDING;
				}
			}
		}
		
		if (timeLeftToPlay == 0 || isSnakeDead == SNAKE_DEAD)
		{
			hideButton(&buttonGameTime, &topScreenConsole);
			hideSnake(&snake);
			hideApple(&apple);
			isSnakeDead = SNAKE_DEAD;
			AUTOMATON_STATE = AUTOMATON_ENDING;
		}
	}
	else if (AUTOMATON_STATE == AUTOMATON_ENDING)
	{
		endingTimer--;
		showRealTimeTimer(endingTimer, &endingTimerNormalized, &topScreenConsole, 12, 15);

		if (isSnakeDead == SNAKE_ALIVE)
		{
			consoleSelect(&bottomScreenConsole);
			iprintf("\x1b[%d;%dH%s", 6, 2, "You won, apparently!");
		}
		else
		{
			consoleSelect(&bottomScreenConsole);
			iprintf("\x1b[%d;%dH%s", 6, 2, "You lost, looser!");
		}

		if (endingTimer == 0)
		{
			selectedGameMode = GAMEMODE_NONE;
			clearConsoleLines(&topScreenConsole, 12, 13, 15, 15);
			clearConsoleLines(&bottomScreenConsole, 0, 31, 0, 23);
			showSubBgGamemodeSelect();
			showMainBgGamemodeSelectModes();
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