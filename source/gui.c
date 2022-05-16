#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "definitions.h"
#include "gui.h"

/*!
 * \brief Modu finitua aukeratzeko gordetzen duen egitura.
 */
Button buttonSelectModeLimited;

/*!
 * \brief Modu etengabea aukeratzeko gordetzen duen egitura.
 */
Button buttonSelectModeUnlimited;

/*!
 * \brief Jokoa pausatzen denean agertzen den testua gordetzen duen egitura.
 */
Button buttonResumeGame;

/*!
 * \brief Jolasi ahala goiko pantailan geratzen den denboraren testua
 * gordetzen duen egitura.
 */
Button buttonGameTime;

/*!
 * \brief Botoi/testu bat sortzeko erabiltzen da, barruko testua, lerrokatze
 * horizontala, eta posizio bertikala.
 * 
 * \param button sortutako botoia gordetzeko jadanik sortutako aldagai baten
 *               erakuslea
 * \param content botoiak idatziko duen testua
 * \param align lerrokatze horizontala zehazten duen (TEXT_ALIGN) motako balioa
 * \param y posizio bertikala zehazten du (min = 0, max = 23)
 */
static void createButton(Button *button, char *content, TEXT_ALIGN align, int y)
{
	/* Emandako balioak gorde */
	button->y = y;
	button->align = align;
	button->content = content;

	button->contentLength = strlen(content); // Gorde karaktere kopurua
	button->width = button->contentLength * 8; // Lodiera kalkulatu, karaktere
	                                           // 1ek 8 lodiera du
	button->height = 8; // Gorde altuera, karaktere 1ek 8 altuera du

	/* Posizio horizontala kalkulatu emandako lerrokatzearen eta karaktere
	   kopuruaren arabera */
	if (align == TEXTALIGN_LEFT)
	{
		button->x = 0;
	}
	else if (align == TEXTALIGN_RIGHT)
	{
		button->x = 31 - button->contentLength + 1;
	} 
	else if (align == TEXTALIGN_CENTER)
	{
		button->x = (31 - button->contentLength + 1) / 2;
	}
}

/*!
 * \brief Pantailaratu behar diren testu/botoi guztiak sortzen ditu.
 */
void createButtons()
{
	createButton(&buttonSelectModeLimited, "PUNTUAZIORA IRITSI", TEXTALIGN_CENTER, 7);
	createButton(&buttonSelectModeUnlimited, "MUGARIK GABEA", TEXTALIGN_CENTER, 15);
	createButton(&buttonResumeGame, "Pantaila ikutu jarraitzeko", TEXTALIGN_CENTER, 11);
	createButton(&buttonGameTime, "Geratzen den denbora: 20 ", TEXTALIGN_RIGHT, 22);
}

/*!
 * \brief Jasotako botoi bat pantailaratzen du jadanik sortutako kontsola
 * egitura bat emanda. Honek zein pantailatan idatzi behar den gordetzen du,
 * besteak beste. 
 * 
 * \param button pantailarazi nahi den botoi/testua
 * \param screenConsole zein pantailan erakutsi nahi den gordetzen duen kontsola
 */
void showButton(Button *button, PrintConsole *screenConsole)
{
	consoleSelect(screenConsole);
	iprintf("\x1b[%d;%dH%s", button->y, button->x, button->content);
}

/*!
 * \brief Jasotako botoi bat ezkutatzen du jadanik sortutako kontsola
 * egitura bat emanda. Honek zein pantailatan idatzi behar den gordetzen du,
 * besteak beste. 
 * 
 * \param button ezkutatu nahi den botoi/testua
 * \param screenConsole zein pantailatik ezkutatu nahi den gordetzen duen kontsola
 */
void hideButton(Button *button, PrintConsole *screenConsole)
{
	consoleSelect(screenConsole);
	iprintf("\x1b[%d;%dH%s", button->y, 0, "                                ");
}

void pressStartAnimate()
{
	static int seg1;
	seg1++;
	if (seg1 == SEG_0_5)
	{
		consoleSelect(&bottomScreenConsole);
		iprintf("\x1b[%d;%dH%s", 11, 4, "Sakatu START jarraitzeko");
	}
	else if (seg1 == SEG_1)
	{
		consoleSelect(&bottomScreenConsole);
		iprintf("\x1b[2J");
		seg1 = 0;
	}
}

void showRealTimeTimer(int interruptTimer, int *secTimer, PrintConsole *console, int x, int y)
{
	*secTimer = interruptTimer / ETEN_SEGUNDUKO;
	if (interruptTimer % SEG_1 == 0)
	{
		consoleSelect(console);
		if (*secTimer / 10 != 0)
		{
			iprintf("\x1b[%d;%dH%d", y, x, *secTimer);
		}
		else
		{
			iprintf("\x1b[%d;%dH %d", y, x, *secTimer);
		}
	}
}

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
void clearConsoleLines(PrintConsole *console, int x1, int x2, int y1, int y2)
{
	consoleSelect(console);
	char str[33] = {0};
	char temp[33] = {0};
	for (int i = x1; i <= x2; i++)
	{
		snprintf(str, sizeof(str), "%s%s", temp, " ");
		snprintf(temp, sizeof(temp), str);
	}
	for (int i = y1; i <= y2; i++)
	{
		iprintf("\x1b[%d;%dH%s", i, x1, str);
	}
}

/*!
 * \brief Ukipena ea botoi baten barruan egin den itzultzen du. true (1) barruan
 * egin bada, false (0) ez bada.
 *
 * \param button begiratu nahi den botoia
 * \param screenData ukipen-pantailari buruz informazioa gordetzen duen erakuslea
 * \return true (1) barruan egin bada ukipena, false (0) bestela
 */
bool touchedInsideButton(Button *button, touchPosition *screenData)
{
	int tX = screenData->px;
	int tY = screenData->py;

	int bX = button->x * 8;
	int bY = button->y * 8;
	int bW = button->width;
	int bH = button->height;
	
	return !(tX < bX || tX > bX + bW || tY < bY || tY > bY + bH);
}