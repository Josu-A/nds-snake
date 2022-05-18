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
 * \brief Integer overflow-a gertatu ez dadin score balioak hartu dezakeen
 * balio maximoa ematen du, beste inkrementu bat egitean overflow-a ez gertatzeko.
 * 
 * \return score balioak izan dezakeen balio fisiko maximoa
 */
static int maxIntScoreAllowedToAdd()
{
	int tmp = ((unsigned int) (8 << (sizeof(int) * 7))) - 1;
	return tmp - SCORE_INCREMENT;
}

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
			resetGameConfig(); // Jolasteko aldagaiak berriz hasieratu

			/* Bi kontsolak garbitu */
			clearConsoleLines(&topScreenConsole, 0, 31, 0, 23);
			clearConsoleLines(&bottomScreenConsole, 0, 31, 0, 23);

			showButton(&buttonGameTime, &topScreenConsole); // Goiko pantailan geratzen den denbora testua ezarri
			showButton(&buttonScore, &topScreenConsole); // Goiko pantailan jokalariak daraman puntuazio testua ezarri
			displayApple(&apple); // Sagarra erakutsi

			AUTOMATON_STATE = AUTOMATON_PLAYING; // Jolastera pasa
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
			updateRotationStateSnakeTail(&snake);
			speedCounter = 0;

			// Sugeak sagarra jaten badu
			if (appleCollidesSnake(&apple, &snake))
			{
				/* Puntuazioa inkrementatu eta puntuazio eguneratua pantailaratu */
				if (score <= maxIntScoreAllowedToAdd()) // integer overflow gertatu ez dadin check-a
					score += SCORE_INCREMENT;
				iprintf("\x1b[%d;%dH%d", buttonScore.y,
				        buttonScore.x + buttonScore.contentLength - 1,
						score);

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
					hideSnake(&snake);
					hideApple(&apple);
					AUTOMATON_STATE = AUTOMATON_ENDING;
				}
			}
		}
		
		if (timeLeftToPlay == 0 || isSnakeDead == SNAKE_DEAD)
		{
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
			clearConsoleLines(&topScreenConsole, 0, 31, 0, 23);
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