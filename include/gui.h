#ifndef _GUI_H_
#define _GUI_H_

#include <nds.h>

/*!
 * \brief Botoiak pantailaratzean horizontalki lerrokatzeko aukerak.
 */
typedef enum TEXT_ALIGN {
    TEXTALIGN_LEFT, // Ezkerrean lerrokatzen da
    TEXTALIGN_CENTER, // Erdian lerrokatzen da
    TEXTALIGN_RIGHT // Eskubidan lerrokatzen da
} TEXT_ALIGN;

/*!
 * \brief Testua pantailaratu ahal izateko errezten duen egitura. Gordetzen duen informazioa:
 * testua idazten hasi den x posizioa, testua idazten hasi den y posizioa, testuaren lodiera
 * eta altuera pixeletan, lerrokatze horizontala, testuaren String-a eta karaktere kopurua.
 */
typedef struct Button
{
    int x; // 0tik 31rako int
    int y; // 0tik 23rako int
    int width;
    int height;
    TEXT_ALIGN align;
    char *content; // Botoiak pantailaratuko duen String-a
    int contentLength; // String-aren luzera
} Button;

/*!
 * \brief Modu finitua aukeratzeko gordetzen duen egitura.
 */
extern Button buttonSelectModeLimited;

/*!
 * \brief Modu etengabea aukeratzeko gordetzen duen egitura.
 */
extern Button buttonSelectModeUnlimited;

/*!
 * \brief Jokoa pausatzen denean agertzen den testua gordetzen duen egitura.
 */
extern Button buttonResumeGame;

/*!
 * \brief Jolasi ahala goiko pantailan geratzen den denboraren testua
 * gordetzen duen egitura.
 */
extern Button buttonGameTime;

/*!
 * \brief Jolasa bukatzean selekzio pantailara itzultzeko testua.
 */
extern Button buttonEndGameTime;

/*!
 * \brief Berriz jolasteko esaten duen testua gordetzen duen botoia.
 */
extern Button buttonPressToRestart;

/*!
 * \brief Partida bukatzean agertzen den testua.
 */
extern Button buttonGameOver;

/*!
 * \brief Partida irabaztean agertzen den testua.
 */
extern Button buttonGameOverWin;

/*!
 * \brief Jokalariak daraman puntuazioa erakusteko testua.
 */
extern Button buttonScore;

/*!
 * \brief Pantailaratu behar diren testu/botoi guztiak sortzen ditu.
 */
extern void createButtons();

/*!
 * \brief Jasotako botoi bat pantailaratzen du jadanik sortutako kontsola
 * egitura bat emanda. Honek zein pantailatan idatzi behar den gordetzen du,
 * besteak beste. 
 * 
 * \param button pantailarazi nahi den botoi/testua
 * \param screenConsole zein pantailan erakutsi nahi den gordetzen duen kontsola
 */
extern void showButton(Button *button, PrintConsole *screenConsole);

/*!
 * \brief Jasotako botoi bat ezkutatzen du jadanik sortutako kontsola
 * egitura bat emanda. Honek zein pantailatan idatzi behar den gordetzen du,
 * besteak beste. 
 * 
 * \param button ezkutatu nahi den botoi/testua
 * \param screenConsole zein pantailatik ezkutatu nahi den gordetzen duen kontsola
 */
extern void hideButton(Button *button, PrintConsole *screenConsole);

/*!
 * \brief Segundu erdiro START sakatzeko pantailaratuko eta ezabatuko du
 * zehaztutako posizioan eta kontsolan.
 * 
 * \param screenConsole zein pantailako kontsolan pantailarazi nahi den
 * \param x x posizioa [0, 31] tartean
 * \param y y posizioa [0, 23] tartean
 */
extern void pressStartAnimate(PrintConsole *screenConsole, int x, int y);

/*!
 * \brief Adierazten den kontsolan jasotako etenetan dagoen denboragailuaren
 * balioarekin segundutan dagoen denboragailua kalkulatzen eta pantailaratzen du
 * eskubian lerrokatuta, zehaztutako posizioan.
 * 
 * \param interruptTimer etenetan dagoen denboragailua
 * \param secTimer segundutan egongo den denboragailuaren erakuslea
 * \param console zein pantailan adieraziko den kontsola
 * \param x x posizioa [0, 31] tartean
 * \param y y posizioa [0, 23] tartean
 */
extern void showRealTimeTimer(int interruptTimer, int *secTimer, PrintConsole *console, int x, int y);

/*!
 * \brief Emandako kontsolatik zehaztutako borneen arteko karaktereak garbitzen
 * ditu. x1 <= x2 eta y1 >= y2 izan behar dira.
 *
 * \param console garbitu nahi den kontsola
 * \param x1 ezker bornea (0tik 31ra)
 * \param x2 eskuin bornea (0tik 31ra)
 * \param y1 goi bornea (0tik 23ra)
 * \param y2 behe bornea (0tik 23ra)
 */
extern void clearConsoleLines(PrintConsole *console, int x1, int x2, int y1, int y2);

/*!
 * \brief Ukipena ea botoi baten barruan egin den itzultzen du. true (1) barruan
 * egin bada, false (0) ez bada.
 *
 * \param button begiratu nahi den botoia
 * \param screenData ukipen-pantailari buruz informazioa gordetzen duen erakuslea
 * \return true (1) barruan egin bada ukipena, false (0) bestela
 */
extern bool touchedInsideButton(Button *button, touchPosition *screenData);

#endif