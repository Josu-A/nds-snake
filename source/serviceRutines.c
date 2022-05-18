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
	int tmp = ((unsigned int) (8 << (sizeof(int) * 7))) - 1; // int balio maximoa lortu
	return tmp - SCORE_INCREMENT; // balio maximoari puntuazio inkrementua kendu eta itzuli
}

/*!
 * \brief Etenen bidez konfiguratutako teklatuek jautiko duten funtzio nagusia.
 */
void interruptKeys()
{
	/* Jokoa irikitzerakoan dagoen egoera. Teklatutik input-a jaso harte emen
	   egongo da. */
	if (AUTOMATON_STATE == AUTOMATON_START)
	{
		/* Etenen bidez START tekla sakatzean */
		if (keysCurrent() & KEY_START) // Teklak detektatu eta konparatu START sakatu den
		{
			clearConsoleLines(&bottomScreenConsole, 0, 31, 0, 23); // Pantaila nagusiko
			                                                       // kontsola garbitu

			/* Jokatzeko modua aukeratzeko bi pantailetako fondoak erakutsi. */
			showSubBgGamemodeSelect();
			showMainBgGamemodeSelectModes();

			AUTOMATON_STATE = AUTOMATON_SELECTION; // Automataren egoera aldatu selekziora
		}
	}
	/* Jokatu bitartean teklatutik SELECT edo START input-a jasoz gero edo
	   pantaila ixten baldin bada, jokoa pausatuko da. */
	else if (AUTOMATON_STATE == AUTOMATON_PLAYING)
	{
		int keys = keysCurrent(); // Teklak detektatu
		/* SELECT/START/Pantaila ixtean, jokoa pausatuko da */
		if (keys & KEY_SELECT || keys & KEY_START || keys & KEY_LID) 
		{
			showButton(&buttonResumeGame, &bottomScreenConsole); // Jokoa pausatu dela adierazi

			AUTOMATON_STATE = AUTOMATON_PAUSED; // Automataren egoera aldatu pausara
		}
	}
	/* Jokoa bukatzean eten bidez SELECT sakatzen bada joku mota berdinean
	   jolasten berriro hasiko da. */
	else if (AUTOMATON_STATE == AUTOMATON_ENDING)
	{
		/* Etenen bidez SELECT tekla sakatzean */
		if (keysCurrent() & KEY_SELECT) // Teklak detektatu eta konparatu SELECT sakatu den
		{
			resetGameConfig(); // Jolasteko aldagaiak berriz hasieratu

			/* Bi kontsolak garbitu */
			clearConsoleLines(&topScreenConsole, 0, 31, 0, 23);
			clearConsoleLines(&bottomScreenConsole, 0, 31, 0, 23);
			
			/* Goiko pantailan jolasaren informazioaren testua ezarri  */
			showButton(&buttonGameTime, &topScreenConsole); // denbora testua
			showButton(&buttonScore, &topScreenConsole); // puntuazio testua

			displayApple(&apple); // Sagarra pantailaratzen da

			AUTOMATON_STATE = AUTOMATON_PLAYING; // Jolastera pasa
		}
	}
}

/*!
 * \brief 0 denboragailuak etenak sortutakoan deituko duen funtzioa.
 */
void interruptTimer0()
{
	/* Jokoa irikitzerakoan dagoen egoera. START tekla sakatu behar den mezua
	   animatzen du. */
	if (AUTOMATON_STATE == AUTOMATON_START)
	{
		pressStartAnimate(); // Segundu erdiko tartearekin fade-in fade-out animazioa
		                     // egiten du, jarraitzeko sakatu behar den botoia adieraziz
	}
	/* Jokatu bitartean sugearen eta sagarraren fisikak kontrolatzen ditu */
	else if (AUTOMATON_STATE == AUTOMATON_PLAYING)
	{
		timeLeftToPlay--; // Etenaldietan adierazitako geratzen den jokatzeko
		                  // denborari bat kendu

		/* Etenez gain fisikak kontrolatzeko itxaron behar den extra.
		   Honen ezker */
		static int speedCounter = 0;
		speedCounter++;
		if (speedCounter >= ANIMATION_SPEED)
		{
			speedCounter = 0; // Kontagailua 0ra ezarri

			/* Funtzio hauek sugearen mugimendua osatzen dute. */
			changeAnimationFrameSnake(&snake); // Norabide bereko hurrengo spritearen
			                                   // indizeak lortu
			moveSnake(&snake); // Sugea mugitu
			displaySnake(&snake); // Suge osoa pantailaratu
			updateRotationStateSnakeBody(&snake); // Suge gorputzaren biratze egoera
			                                      // eguneratu
			updateRotationStateSnakeTail(&snake); // Suge isatsaren biratze egoera
			                                      // eguneratu

			/* Hemen sugeak ea sagarra jan duen zehazten da. */
			if (appleCollidesSnake(&apple, &snake)) // Sagarra jan badu
			{
				/* Puntuazioa inkrementatu */
				if (score <= maxIntScoreAllowedToAdd()) // integer overflow gertatu ez dadin check-a
					score += SCORE_INCREMENT;
				/* Puntuazio eguneratua pantailaratu */
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
				else if (timeLeftToPlayNormalized < 99)
				{
					timeLeftToPlay = 99 * ETEN_SEGUNDUKO; // Geratzen den denbora 99
					                                      // segunduan ezarri
				}

				// Sagarra posizioz mugitu
				resetApple(&apple);

				/* Bukaera modua duen jolasa aukeratu ezean eta irabazi bada */
				if (selectedGameMode == GAMEMODE_LIMITED && score >= MAX_SCORE)
				{
					/* Sugea eta sagarra ezkutatu */
					hideSnake(&snake);
					hideApple(&apple);

					AUTOMATON_STATE = AUTOMATON_ENDING; // Jolas bukaera egoerara igaro
				}
			}
		}
		/* Jolasteko denbora amaitu bada edo sugea hil bada talka eginez */
		if (timeLeftToPlay == 0 || isSnakeDead == SNAKE_DEAD)
		{
			/* Sugea eta sagarra ezkutatu */
			hideSnake(&snake);
			hideApple(&apple);

			isSnakeDead = SNAKE_DEAD; // Sugea hil dela gorde

			AUTOMATON_STATE = AUTOMATON_ENDING; // Jolas bukaera egoerara igaro
		}
	}
	/* Jokoa amaitu denean partidaren informazioa adieraziko du eta adierazitako
	   denbora igarotzen bada modu aukeraketa pantailara mugituko da jokoa*/
	else if (AUTOMATON_STATE == AUTOMATON_ENDING)
	{
		endingTimer--; // Etenaldietan adierazitako aukeraketa pantailarako joateko
		               // geratzen den denborari bat kendu
		// Aukera egiteko geratzen diren segunduak pantailaratu
		showRealTimeTimer(endingTimer, &endingTimerNormalized, &topScreenConsole, 12, 15);

		// Sugea bizirik badago, jokoa irabazi dela esan nahi du
		if (isSnakeDead == SNAKE_ALIVE)
		{
			consoleSelect(&bottomScreenConsole);
			iprintf("\x1b[%d;%dH%s", 6, 2, "You won, apparently!");
		}
		// Sugea hilik badago, jokoa galdu dela esan nahi du
		else
		{
			consoleSelect(&bottomScreenConsole);
			iprintf("\x1b[%d;%dH%s", 6, 2, "You lost, looser!");
		}

		if (endingTimer == 0)
		{
			selectedGameMode = GAMEMODE_NONE; // Aukeratutako joko modua reseteatu

			/* Bi kontsolak garbitu */
			clearConsoleLines(&topScreenConsole, 0, 31, 0, 23);
			clearConsoleLines(&bottomScreenConsole, 0, 31, 0, 23);

			/* Jokatzeko modua aukeratzeko bi pantailetako fondoak erakutsi. */
			showSubBgGamemodeSelect();
			showMainBgGamemodeSelectModes();

			AUTOMATON_STATE = AUTOMATON_SELECTION; // Automataren egoera aldatu selekziora
		}
	}
}

/*!
 * \brief Eten zerbitzu-errutinak zehazten ditu teklatuarentzako eta
 * denboragailuarentzako.
 */
void setInterruptionServiceRutines()
{
	irqSet(IRQ_KEYS, interruptKeys); // Teklatuaren eten zerbitzu-errutina ezarri
	irqSet(IRQ_TIMER0, interruptTimer0); // Denboragailuaren eten zerbitzu-errutina ezarri
}