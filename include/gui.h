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

extern void showRealTimeTimer(int interruptTimer, int *secTimer, PrintConsole *console, int x, int y);
extern void pressStartAnimate();

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