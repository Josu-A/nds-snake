#ifndef _SPRITES_H_
#define _SPRITES_H_

#include "objectSnake.h"

#define COLORS_PER_SPRITE 16

/*!
 * \brief Suge buruaren egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du. Indize honen bitartez memorian gordeta dagoen tile-en
 * erakusletik nahi dugun frame-aren helbidea lortzen da, eta azkenik bertako
 * framea memoriatik spritea irakurtzen den lekuan kopiatzen da spritea eguneratu
 * ahal izateko.
 *
 * \param snake suge egituraren erakuslea
 */
extern void animateSnakeHead(Snake *snake);

/*!
 * \brief spriteSnake-ren paleta kopiatzen du pantaila nagusiko paletara.
 *
 * \param snake suge egituraren erakuslea
 */
extern void saveIntoMemorySnakeHead(Snake *snake);

/*!
 * \brief Suge gorputzaren egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du. Indize honen bitartez memorian gordeta dagoen tile-en
 * erakusletik nahi dugun frame-aren helbidea lortzen da, eta azkenik bertako
 * framea memoriatik spritea irakurtzen den lekuan kopiatzen da spritea eguneratu
 * ahal izateko.
 *
 * \param snake suge egituraren erakuslea
 */
void animateSnakeBody(Snake *snake);

/*!
 * \brief spriteSnakeBody-ren paleta kopiatzen du pantaila nagusiko paletara.
 *
 * \param snake suge egituraren erakuslea
 */
extern void saveIntoMemorySnakeBody(Snake *snake);

/*!
 * \brief Suge isatsaren egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du. Indize honen bitartez memorian gordeta dagoen tile-en
 * erakusletik nahi dugun frame-aren helbidea lortzen da, eta azkenik bertako
 * framea memoriatik spritea irakurtzen den lekuan kopiatzen da spritea eguneratu
 * ahal izateko.
 *
 * \param snake suge egituraren erakuslea
 */
extern void animateSnakeTail(Snake *snake);

/*!
 * \brief spriteSnakeTail-ren paleta kopiatzen du pantaila nagusiko paletara.
 *
 * \param snake suge egituraren erakuslea
 */
extern void saveIntoMemorySnakeTail(Snake *snake);

/*!
 * \brief OAM sarrera zehazten du sugea ikusarazteko zehaztutako balioekin.
 *
 * \param snake suge egitura duen erakuslea
 */
extern void displaySnake(Snake *snake);

/*!
 * \brief OAM sarrera zehazten du sugea ezkutatzeko zehaztutako balioekin.
 *
 * \param snake suge egitura duen erakuslea
 */
extern void hideSnake(Snake *snake);

/*!
 * \brief 2D sprite motoreak hasieratzen ditu bi pantailetan tiledun eta bitmapdun
 * spriteak ageri daitezen. Baita ere sugearentzat memoria alokatzen du.
 */
extern void initSprites();

#endif