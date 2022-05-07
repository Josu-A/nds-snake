#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <nds.h>		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

/*!
 * \brief Irudiak memorian kopiatzeko DMA kanala aukeratu
 */
#define DMA_CHANNEL 3

/******************************************************************************
 * begin { TouchScreen }
******************************************************************************/

/*!
 *  \brief Ukimen-pantailaren erregistroa
 */
extern touchPosition PANT_DAT;

/******************************************************************************
 * end { TouchScreen }
******************************************************************************/

/******************************************************************************
 * begin { Automata }
******************************************************************************/

/*!
 * \brief Automataren egoera posibleak gordetzen ditu.
 */
typedef enum AUTOMATON_STATES {
    AUTOMATON_START, // Jokoa irekitzean
    AUTOMATON_SELECTION, // Joko mota aukeratzen
    AUTOMATON_PLAYING, // Jolasten
    AUTOMATON_PAUSED, // Jokoa pausatuta
    AUTOMATON_ENDING // Jokoa amaituta
} AUTOMATON_STATES;

/*!
 * \brief Automataren egoera gordeko duen aldagaia deklaratzen da c
 * fitxategi batean.
 */
extern AUTOMATON_STATES AUTOMATON_STATE;

typedef enum GAME_MODES {
    GAMEMODE_NONE,
    GAMEMODE_LIMITED,
    GAMEMODE_UNLIMITED
} GAME_MODES;

extern GAME_MODES selectedGameMode;
extern void resetGameConfig();

/******************************************************************************
 * end { Automata }
******************************************************************************/

extern PrintConsole topScreenConsole;
extern PrintConsole bottomScreenConsole;

/******************************************************************************
 * begin { Denboragailua }
******************************************************************************/

#define ETEN_SEGUNDOKO 4
#define SEG_0_5 (ETEN_SEGUNDOKO / 2)
#define SEG_1 (ETEN_SEGUNDOKO * 1)
#define MAX_GAME_TIME_NORMALIZED 20
#define MAX_GAME_TIME (ETEN_SEGUNDOKO * MAX_GAME_TIME_NORMALIZED)
#define MAX_END_TIME_NORMALIZED 10
#define MAX_END_TIME (ETEN_SEGUNDOKO * MAX_END_TIME_NORMALIZED)
extern int timeLeftToPlay;
extern int timeLeftToPlayNormalized;
extern int endingTimer;
extern int endingTimerNormalized;

/******************************************************************************
 * end { Denboragailua }
******************************************************************************/

extern int score;
#define MAX_SCORE 200

#endif