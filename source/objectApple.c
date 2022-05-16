#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia
#include <time.h>

#include "objectApple.h"
#include "definitions.h"
#include "objectSnake.h"
#include "sprites.h"

/*!
 * \brief Sagar objektua gordetzen du.
 */
Apple apple;

/*!
 * \brief Sagarraren erakuslea jasota honen posizioa ezartzen da pantailan
 * emandako x eta y balioekin.
 * 
 * \param apple sagar objektuaren erakuslea
 * \param x posizio horizontala, 0tik 15ra (SCREEN_WIDTH_TILES - 1)
 * \param y posizio bertikala,  0tik 11ra (SCREEN_HEIGHT_TILES - 1)
 */
void setPositionApple(Apple *apple, int x, int y)
{
	apple->x = x * APPLE_DIMENSION;
	apple->y = y * APPLE_DIMENSION;
}

/*!
 * \brief Zoriz [0, max) tarteko balio bat itzultzen du, max kanpo.
 * 
 * \param max tarteko goi bornea, kanpo
 * \return long zoriz lortutako tarteko balioa
 */
static long randomMaxValue(long max)
{
    // random() funtzioak duen zenbaki tarteko zenbaki kopurua kalkulatu
    unsigned long numRandomVal = (unsigned long) RAND_MAX + 1; 

    /* random() funtzioak ematen duen zenbaki tartea zatitzen du jasotako max
       zenbakiarekin, tartetik distribuzio bat sortzeko zatit kopuru honekin. */
    unsigned long disSize = numRandomVal / (unsigned long) max;

    /* Gainezka ez egiteko eta distribuzio uniformea akastu ez dadin deskartatu
       behar diren balio posibleak kalkulatu. */
    unsigned long modulus = numRandomVal % (unsigned long) max;

    long x; // zorizko aldagaia gordetzeko
    srand(time(NULL));
    do
    {
        x = random(); // 0tik 2^31-1ra zorizko balioa lortu
    }
    while (x >= numRandomVal - modulus); // iterazioa jarraitu distribuzio uniformetik
                                      // kanpo dugun balio bat lortu ahala
    return x / disSize; // distribuzio uniformea emandako balio maximoarekin
                             // zatitu [0, max) tarteko balio bat lortzeko
}

/*!
 * \brief X eta Y bidez zehaztutako kuadrantean parametro bidez jasotako
 * sugearen zatiren bat badago true itzultzen du. Kuadrantea hutsik badago false.
 * 
 * \param snake suge objektuaren erakuslea
 * \param x x posizio horizontala, 0tik 15ra (SCREEN_WIDTH_TILES - 1)
 * \param y posizio bertikala,  0tik 11ra (SCREEN_HEIGHT_TILES - 1)
 * \return true kuadrantean sugea badago, false libre badago
 */
static bool tileContainsSnake(Snake *snake, int x, int y)
{
    // Jasotako kuadrantearen pixelak lortu
    x *= TILE_LENGTH;
    y *= TILE_LENGTH;

    return (snake->snakeHead.x == x && snake->snakeHead.y == y)
        || (snake->snakeBody[0].x == x && snake->snakeBody[0].y == y)
        || (snake->snakeTail.x == x && snake->snakeTail.y == y);
}

/*!
 * \brief Parametroz jasotako sagarrarentzat hutsik dagoen posizio berri
 * bat kalkulatzen eta zehazten du.
 * 
 * \param apple sagar objektuaren erakuslea
 */
void newRandomPositionApple(Apple *apple)
{
    // Sagarrarentzako kuadrante berria
    int newTileX;
    int newTileY;

    do
    {
        // Kuadrante berria lortu
        newTileX = (int) randomMaxValue(SCREEN_WIDTH_TILES);
        newTileY = (int) randomMaxValue(SCREEN_HEIGHT_TILES);
    }
    // Kuadrante berria lortu kuadrantea libre dagoen arte
    while (tileContainsSnake(&snake, newTileX, newTileY));
    
    // Sagarraren posizio berria ezarri pixeletan
    apple->x = newTileX * TILE_LENGTH;
    apple->y = newTileY * TILE_LENGTH;
}

/*!
 * \brief Sugea eta sagarra txokatu diren itzultzen du. Horretarako, suge
 * burua dagoen tile-a eta sagarra dagoen tile-a konparatzen ditu.
 * 
 * \param apple sagar objektuaren erakuslea
 * \param snake suge objektuaren erakuslea
 * \return true suge burua sagarraren tile berdinean badago, false bestela
 */
bool appleCollidesSnake(Apple *apple, Snake *snake)
{
    return (apple->x / TILE_LENGTH == snake->snakeBody->x / TILE_LENGTH) 
           && (apple->y / TILE_LENGTH == snake->snakeBody->y / TILE_LENGTH);
}

/*!
 * \brief Sagarra berrezartzen du.
 * 
 * \param apple sagar objektuaren erakuslea
 */
void resetApple(Apple *apple)
{
    hideApple(apple);
    newRandomPositionApple(apple);
    displayApple(apple); // Sagarra pantailaratzen da
}