#ifndef _SPRITES_H_
#define _SPRITES_H_

#include "objectSnake.h"
#include "objectApple.h"

#define COLORS_PER_SPRITE 16

/*!
 * \brief Suge osoaren egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du atal bakoitzarentzat. Indize honen bitartez memorian
 * gordeta dagoen tile-en erakusletik nahi dugun frame-aren helbidea lortzen da,
 * eta azkenik bertako framea memoriatik spritea irakurtzen den lekuan kopiatzen
 * da spritea eguneratu ahal izateko.
 *
 * \param snake suge egituraren erakuslea
 */
extern void animateSnake(Snake *snake);

/*!
 * \brief 2D sprite motoreak hasieratzen ditu bi pantailetan tiledun eta bitmapdun
 * spriteak ageri daitezen. Baita ere, sugearentzat eta sagarrarentzat memoria
 * alokatzen du.
 */
extern void initSprites();

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
 * \brief OAM sarrera zehazten du sagarra ikusarazteko zehaztutako balioekin.
 *
 * \param apple sagar egitura duen erakuslea
 */
extern void displayApple(Apple *apple);

/*!
 * \brief OAM sarrera zehazten du sagarra ezkutatzeko zehaztutako balioekin.
 *
 * \param apple sagar egitura duen erakuslea
 */
extern void hideApple(Apple *apple);

#endif