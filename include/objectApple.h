#ifndef _OBJECTAPPLE_H_
#define _OBJECTAPPLE_H_

#include "objectSnake.h"

/*!
 * \brief Sagar spritearen luzera/altuera
 */
#define APPLE_DIMENSION 16

/*!
 * \brief Sagar objektu bat definitzen du.
 */
typedef struct Apple
{
    int x; // Objektuaren x posizioa gordetzen du
    int y; // Objektuaren y posizioa gordetzen du

    u16 *spriteGfxMem; // Sprite-a gordeko den memoriaren helbidea gordetzen du

    int spriteId; // OAMen izango duen identifikatzailea
} Apple;

/*!
 * \brief Sagar objektua gordetzen du.
 */
extern Apple apple;

/*!
 * \brief Sagarraren erakuslea jasota honen posizioa ezartzen da pantailan
 * emandako x eta y balioekin.
 * 
 * \param apple sagar objektuaren erakuslea
 * \param x posizio horizontala, 0tik 15ra (SCREEN_WIDTH / APPLE_DIMENSION - 1)
 * \param y posizio bertikala,  0tik 11ra (SCREEN_HEIGHT / APPLE_DIMENSION - 1)
 */
extern void setPositionApple(Apple *apple, int x, int y);

/*!
 * \brief Parametroz jasotako sagarrarentzat hutsik dagoen posizio berri
 * bat kalkulatzen eta zehazten du.
 * 
 * \param apple sagar objektuaren erakuslea
 */
extern void newRandomPositionApple(Apple *apple);

/*!
 * \brief Sugea eta sagarra txokatu diren itzultzen du. Horretarako, suge
 * burua dagoen tile-a eta sagarra dagoen tile-a konparatzen ditu.
 * 
 * \param apple sagar objektuaren erakuslea
 * \param snake suge objektuaren erakuslea
 * \return true suge burua sagarraren tile berdinean badago, false bestela
 */
extern bool appleCollidesSnake(Apple *apple, Snake *snake);

/*!
 * \brief Sagarra berrezartzen du.
 * 
 * \param apple sagar objektuaren erakuslea
 */
extern void resetApple(Apple *apple);

#endif