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

/*!
 * \brief Automataren egoera gordeko duen aldagai globala.
 */
AUTOMATON_STATES AUTOMATON_STATE;

/*!
 *  \brief Ukimen-pantailari buruz informazioa gordetzen du.
 */
touchPosition PANT_DAT;

/*!
 * \brief Aukeratutako jolas modua gordetzen duen aldagai globala.
 */
GAME_MODES selectedGameMode = GAMEMODE_NONE;

/*!
 * \brief Partidako puntuazioa gordetzen du.
 */
int score;

/*!
 * \brief Jolasa amaitzean aukeraketa pantailara joateko gelditzen den denbora
 * etenalditan.
 */
int endingTimer;

/*!
 * \brief Jolasa amaitzean aukeraketa pantailara joateko gelditzen den denbora
 * segundutan.
 */
int endingTimerNormalized;

/*!
 * \brief Jolasteko gelditzen den denbora etenalditan.
 */
int timeLeftToPlay;

/*!
 * \brief Jolasteko gelditzen den denbora segundutan.
 */
int timeLeftToPlayNormalized;

/*!
 * \brief Jolasa berriz jokatu ahal izateko konfiguratzen ditu aldagaiak.
 */
void resetGameConfig()
{
	score = 0; // Reset score to 0
	endingTimer = MAX_END_TIME; // Reset timer used to get back to the main menu when
	                            // each game finishes to its max value, in interruptions
	endingTimerNormalized = MAX_END_TIME_NORMALIZED; // Reset timer used to get back to the main menu when
													 // each game finishes to its max value, in seconds
	timeLeftToPlay = MAX_GAME_TIME; // Reset the time left to play to its max value, in interruptions
	timeLeftToPlayNormalized = MAX_GAME_TIME_NORMALIZED; // Reset the time left to play to its max value, in seconds
	resetSnake(&snake);
	resetApple(&apple);
}

/*!
 * \brief Ukimen pantaila ukitu den itzultzen du, true (1) gisa. Ez bada ukitu,
 * false (0) itzultzen du.
 */
static bool touchedTouchScreen(touchPosition *touchPad)
{
	return (touchPad->px != 0 || touchPad->py != 0);
}

/*!
 * \brief Jokoaren logika zehazten du.
 */
void jokoa01()
{	
	AUTOMATON_STATE = AUTOMATON_START; // Hasierako egoera ezarri
	
	setInterruptionServiceRutines(); // Eten zerbitzu-errutinak ezarri
	configureKeys(KEY_SELECT | KEY_START); // SELECT eta START teklak etenen bidez sinkronizatu
	int latch = (1<<16) - (1<<25) / (ETEN_SEGUNDOKO * (1<<10)); // Latch-a lortu
	configureTimer0(latch, 0x1<<6 | 0x3<<0); // 0 denoragailua konfiguratu
	allowInterruptsKeys(); // Teklatuaren etenak baimendu
	allowInterruptsTimer0(); // 0 denboragailuaren etenak baimendu
	startClockTimer0(); // 0 denboragailua martxan jarri

	createButtons(); // Pantailaratzeko botoiak sortu

	// Loop nagusia
	while(1)
	{
		// Jolas mota aukeratzen denerako egoera
		if (AUTOMATON_STATE == AUTOMATON_SELECTION) 
		{
			resetGameConfig(); // Jokoa ondo hasieratzeko aldagaiak hasieratu

			// Azpiko pantailan bi jolas moduen botoiak erakutsi
			showButton(&buttonSelectModeLimited, &bottomScreenConsole);
			showButton(&buttonSelectModeUnlimited, &bottomScreenConsole);

			// Joku mota aukeratzeko begizta
			while (selectedGameMode == GAMEMODE_NONE)
			{
				touchRead(&PANT_DAT); // Ukimen pantaila eguneratu

				// Bukatzen den moduaren testuaren gainean ukitu ezkero
				if (touchedInsideButton(&buttonSelectModeLimited, &PANT_DAT))
				{
					selectedGameMode = GAMEMODE_LIMITED; // Bukatzen den modua gorde
				}
				// Bukatzen ez den moduaren testuaren gainean ukitu ezkero
				else if (touchedInsideButton(&buttonSelectModeUnlimited, &PANT_DAT))
				{
					selectedGameMode = GAMEMODE_UNLIMITED; // Bukatzen ez den modua gorde
				}
			}

			// Azpiko pantailako jolas moduen botoiak ezkutatu
			hideButton(&buttonSelectModeLimited, &bottomScreenConsole);
			hideButton(&buttonSelectModeUnlimited, &bottomScreenConsole);
			
			// Goiko pantailan geratzen den denbora testua ezarri
			showButton(&buttonGameTime, &topScreenConsole);

			displayApple(&apple);
			
			AUTOMATON_STATE = AUTOMATON_PLAYING; // Egoera jolasten ezarri
		}
		else if (AUTOMATON_STATE == AUTOMATON_PLAYING) // Jolasean ibiltzeko egoera
		{
			scanKeys(); // Teklatuaren egoera eguneratzen du libnds-ko funtzioak erabiltzeko
			updateRotationStateSnake(&snake); // Teklatutik norabide berria lortzen du
			animateSnake(&snake); // Eguneratutako sub-sprite berria memorian ezartzen du erakusteko
			displaySnake(&snake); // Sugearen sprite eguneratua pantailatzen da

			// Bukatzen den modua aukeratuz gero eta puntuazio maximora iristerakoan, jokoa amaitu.
			if (selectedGameMode == GAMEMODE_LIMITED && score == MAX_SCORE)
			{
				AUTOMATON_STATE = AUTOMATON_ENDING;
			}
		}
		else if (AUTOMATON_STATE == AUTOMATON_PAUSED) // Jolasa pausatzerako egoera
		{
			touchRead(&PANT_DAT);
			if (touchedTouchScreen(&PANT_DAT) && ~(keysCurrent() & KEY_LID))
			{
				hideButton(&buttonResumeGame, &bottomScreenConsole);
				AUTOMATON_STATE = AUTOMATON_PLAYING;
			}
		}
		else if (AUTOMATON_STATE == AUTOMATON_ENDING) // Jolasa bukatzerakoak aukera egiteko egoera
		{
			// Nothing for now
		}

		swiWaitForVBlank(); // Pantailak guztiz errefrekatu arte itxaroten du

		// Bi pantailen OAMak eguneratzen ditu momentuan memorian gordetako spritekin
		oamUpdate(&oamMain);
 		oamUpdate(&oamSub);
	}

	/* Loop nagusia bukatzean egin beharko litzateken eragiketak, gure kasuan
	   loop hori inoiz ez denez amaituko, ez dira beharrezkoak. */
	denyInterruptsKeys(); // Teklatuko etenak galarazten ditu.
	denyInterruptsTimer0(); // 0 denboragailuaren etenak galarazten ditu.
}