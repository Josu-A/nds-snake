#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "definitions.h"
#include "gui.h"

Button buttonSelectModeLimited;
Button buttonSelectModeUnlimited;
Button buttonResumeGame;
Button buttonGameTime;

void createButtons()
{
	createButton(&buttonSelectModeLimited, "PUNTUAZIORA IRITSI", TEXTALIGN_CENTER, 7);
	createButton(&buttonSelectModeUnlimited, "MUGARIK GABEA", TEXTALIGN_CENTER, 15);
	createButton(&buttonResumeGame, "Pantaila ikutu jarraitzeko", TEXTALIGN_CENTER, 11);
	createButton(&buttonGameTime, "Geratzen den denbora: 20 ", TEXTALIGN_RIGHT, 22);
}

void createButton(Button *button, char *content, TEXT_ALIGN align, int y)
{
	button->y = y;
	button->align = align;
	button->content = content;

	button->contentLength = strlen(content);
	button->width = button->contentLength * 8;
	button->height = 8;

	if (align == TEXTALIGN_LEFT)
	{
		button->x = 0;
	} else if (align == TEXTALIGN_RIGHT)
	{
		button->x = 31 - button->contentLength + 1;
	} 
	else if (align == TEXTALIGN_CENTER)
	{
		button->x = (31 - button->contentLength + 1) / 2;
	}
}

void showButton(Button *button, PrintConsole *screenConsole)
{
	consoleSelect(screenConsole);
	iprintf("\x1b[%d;%dH%s", button->y, button->x, button->content);
}

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
	if (interruptTimer % SEG_1 == 0)
	{
		*secTimer -= 1;
		int temp = *secTimer;
		consoleSelect(console);
		if (temp / 10 != 0)
		{
			iprintf("\x1b[%d;%dH%d", y, x, temp);
		}
		else
		{
			iprintf("\x1b[%d;%dH %d", y, x, temp);
		}
	}
}

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