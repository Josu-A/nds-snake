#ifndef _GUI_H_
#define _GUI_H_

#include <nds.h>

extern void pressStartAnimate();

typedef enum TEXT_ALIGN {
    TEXTALIGN_LEFT,
    TEXTALIGN_CENTER,
    TEXTALIGN_RIGHT
} TEXT_ALIGN;

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

extern Button buttonSelectModeLimited;
extern Button buttonSelectModeUnlimited;
extern Button buttonResumeGame;
extern Button buttonGameTime;

extern void showButton(Button *button, PrintConsole *screenConsole);
extern void hideButton(Button *button, PrintConsole *screenConsole);

extern void createButtons();
extern void createButton(Button *button, char *content, TEXT_ALIGN align, int y);
extern void showButton(Button *button, PrintConsole *screenConsole);
extern void hideButton(Button *button, PrintConsole *screenConsole);

extern void showRealTimeTimer(int interruptTimer, int *secTimer, PrintConsole *console, int x, int y);
extern void pressStartAnimate();

extern void clearConsoleLines(PrintConsole *console, int x1, int x2, int y1, int y2);

extern bool touchedInsideButton(Button *button, touchPosition *screenData);

#endif