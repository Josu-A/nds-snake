#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <nds.h>

/*!
 * \brief Irudiak memorian kopiatzeko DMA kanala gordetzen du.
 */
#define DMA_CHANNEL 3

/*!
 * Kuadrante batek neurtzen dituen pixelak.
 */
#define TILE_LENGTH 16

/*!
 * Pantailaren luzera definitzen da kuadranteetan.
 */
#define SCREEN_WIDTH_TILES (SCREEN_WIDTH / TILE_LENGTH)

/*!
 * Pantailaren altuera definitzen da kuadranteetan.
 */
#define SCREEN_HEIGHT_TILES (SCREEN_HEIGHT / TILE_LENGTH)

/*!
 *  \brief Ukimen-pantailari buruz informazioa gordetzen du.
 */
extern touchPosition PANT_DAT;

/******************************************************************************
 * begin { Automata }
******************************************************************************/

/*!
 * \brief Automatak izan ditzakeen egoera posibleak gordetzen ditu.
 */
typedef enum AUTOMATON_STATES {
    AUTOMATON_START, // Jokoa irekitzean du
    AUTOMATON_SELECTION, // Joko mota aukeraketan du
    AUTOMATON_PLAYING, // Jolasean du
    AUTOMATON_PAUSED, // Jokoa pausatuta dagoenean du
    AUTOMATON_ENDING // Jokoa amaituta denean
} AUTOMATON_STATES;

/*!
 * \brief Automataren egoera gordeko duen aldagai globala.
 */
extern AUTOMATON_STATES AUTOMATON_STATE;

/*!
 * \brief Jolasak izan ditzakeen moduak gordetzen ditu.
 */
typedef enum GAME_MODES {
    GAMEMODE_NONE, // Modurik aukeratuta ez dago
    GAMEMODE_LIMITED, // Jolasa bukatu egiten da puntuazio maximoa lortzean
    GAMEMODE_UNLIMITED // Jolasa denbora bukatzean bakarrik amaitzen da
} GAME_MODES;

/*!
 * \brief Aukeratutako jolas modua gordetzen duen aldagai globala.
 */
extern GAME_MODES selectedGameMode;

/*!
 * \brief Jolasa berriz jokatu ahal izateko konfiguratzen ditu aldagaiak.
 */
extern void resetGameConfig();

/******************************************************************************
 * end { Automata }
******************************************************************************/

/******************************************************************************
 * begin { Kontsola }
******************************************************************************/

/*!
 * \brief Goiko pantailako kontsola kontrolatzeko datuak gordetzen ditu.
 */
extern PrintConsole topScreenConsole;

/*!
 * \brief Beheko pantailako kontsola kontrolatzeko datuak gordetzen ditu.
 */
extern PrintConsole bottomScreenConsole;

/******************************************************************************
 * end { Kontsola }
******************************************************************************/

/******************************************************************************
 * begin { Denboragailua }
******************************************************************************/

/*!
 * \brief Timer0-ren zerbitzu errutinak segunduro zenbat eten sortu behar
 * dituen gordetzen du.
 */
#define ETEN_SEGUNDOKO 4

/*!
 * \brief Segundo erdia igarotzeko gertatu behar den eten kopurua da.
 */
#define SEG_0_5 (ETEN_SEGUNDOKO / 2)

/*!
 * \brief Segundo bat igarotzeko gertatu behar den eten kopurua da.
 */
#define SEG_1 (ETEN_SEGUNDOKO * 1)

/*!
 * \brief Jolasa bukatzeko dauden segundu kopurua.
 */
#define MAX_GAME_TIME_NORMALIZED 20

/*!
 * \brief Jolasa bukatzeko dauden eten kopurua.
 */
#define MAX_GAME_TIME (ETEN_SEGUNDOKO * MAX_GAME_TIME_NORMALIZED)

/*!
 * \brief Jokoa amaitzean jolas mota aukeraketara itzultzeko dauden segundu
 * maximoak.
 */
#define MAX_END_TIME_NORMALIZED 10

/*!
 * \brief Jokoa amaitzean jolas mota aukeraketara itzultzeko dauden eten
 * kopuru maximoak.
 */
#define MAX_END_TIME (ETEN_SEGUNDOKO * MAX_END_TIME_NORMALIZED)

/*!
 * \brief Jolasteko gelditzen den denbora etenalditan.
 */
extern int timeLeftToPlay;

/*!
 * \brief Jolasteko gelditzen den denbora segundutan.
 */
extern int timeLeftToPlayNormalized;

/*!
 * \brief Jolasa amaitzean aukeraketa pantailara joateko gelditzen den denbora
 * etenalditan.
 */
extern int endingTimer;

/*!
 * \brief Jolasa amaitzean aukeraketa pantailara joateko gelditzen den denbora
 * segundutan.
 */
extern int endingTimerNormalized;

/******************************************************************************
 * end { Denboragailua }
******************************************************************************/

/*!
 * \brief Joku modu mugatuan irabazteko iritsi behar den puntuazioa.
 */
#define MAX_SCORE 200

/*!
 * \brief Partidako puntuazioa gordetzen du.
 */
extern int score;

#endif