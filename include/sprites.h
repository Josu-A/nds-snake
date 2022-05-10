#ifndef _SPRITES_H_
#define _SPRITES_H_

#include "objectSnake.h"

/*!
 * \brief Sugearen egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du. Indize honen bitartez memorian gordeta dagoen tile-en
 * erakusletik nahi dugun frame-aren helbidea lortzen da, eta azkenik bertako
 * framea memoriatik spritea irakurtzen den lekuan kopiatzen da spritea eguneratu
 * ahal izateko.
 *
 * \param sprite suge egituraren erakuslea
 */
extern void animateSnake(Snake* sprite);

/*!
 * \brief spriteSnake-ren paleta kopiatzen du pantaila nagusiko paletara.
 */
extern void saveIntoMemorySnake();

/*!
 * \brief OAM sarrera zehazten du sugea ikusarazteko zehaztutako balioekin.
 *
 * \param sprite suge egitura duen erakuslea
 */
extern void displaySnake(Snake *sprite);

/*!
 * \brief OAM sarrera zehazten du sugea ezkutatzeko zehaztutako balioekin.
 *
 * \param sprite suge egitura duen erakuslea
 */
extern void hideSnake(Snake *sprite);

/*!
 * \brief 2D sprite motoreak hasieratzen ditu bi pantailetan tiledun eta bitmapdun
 * spriteak ageri daitezen. Baita ere sugearentzat memoria alokatzen du.
 */
extern void initSprites();

#endif