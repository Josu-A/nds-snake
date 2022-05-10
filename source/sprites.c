#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "definitions.h"
#include "sprites.h"
#include "objectSnake.h"

#include "snake.h"

/*!
 * \brief Sugearen spritearentzako memoria alokatzen da, eta alokatutako
 * helbidea parametroz jasotako snake-aren erakuslearen spriteGfxMem aldagaian
 * gordetzen da. Horrez gain, spriteak dituen tile-en informazioaren erakuslea
 * gordetzen da frameGfx-en.
 * 
 * \param sprite suge egituraren erakuslea
 * \param gfx sugearen tile-en erakuslea
 */
static void initSnake(Snake *sprite, u8 *gfx)
{
    sprite->spriteGfxMem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
    sprite->frameGfx = (u8*) gfx;
}

/*!
 * \brief Sugearen egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du. Indize honen bitartez memorian gordeta dagoen tile-en
 * erakusletik nahi dugun frame-aren helbidea lortzen da, eta azkenik bertako
 * framea memoriatik spritea irakurtzen den lekuan kopiatzen da spritea eguneratu
 * ahal izateko.
 *
 * \param sprite suge egituraren erakuslea
 */
void animateSnake(Snake* sprite)
{
    int frame = sprite->animFrame + sprite->state * FRAMES_PER_ANIMATION;
	u8* offset = sprite->frameGfx + frame * SNAKE_DIMENSION*SNAKE_DIMENSION;
    dmaCopyHalfWords(DMA_CHANNEL,
                     offset,
                     sprite->spriteGfxMem,
                     SNAKE_DIMENSION*SNAKE_DIMENSION);
}

/*!
 * \brief spriteSnake-ren paleta kopiatzen du pantaila nagusiko paletara.
 */
void saveIntoMemorySnake()
{
    dmaCopyHalfWords(DMA_CHANNEL,
                     snakePal,
                     SPRITE_PALETTE,
                     snakePalLen);
}

/*!
 * \brief OAM sarrera zehazten du sugea ikusarazteko zehaztutako balioekin.
 *
 * \param sprite suge egitura duen erakuslea
 */
void displaySnake(Snake *sprite)
{
    oamSet(&oamMain, // oam
           0, // id
           sprite->x, // x
           sprite->y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_32x32, // tamaina 
           SpriteColorFormat_256Color, // kolore formatua
           sprite->spriteGfxMem, // spritea agertzen den helbidea memorian
           -1, // indize afinea
           false, // indize afinea bada tamaina bikoiztu errotazioan
           false, // spritea ezkutatu nahi den, ezin da afinea izan
           false, // bertikalki bolkatu spritea
           false, // horizontalki bolkatu spritea
           false); // mosaicoa aplikatu nahi den
}

/*!
 * \brief OAM sarrera zehazten du sugea ezkutatzeko zehaztutako balioekin.
 *
 * \param sprite suge egitura duen erakuslea
 */
void hideSnake(Snake *sprite)
{
    oamSet(&oamMain, // oam
           0, // id
           sprite->x, // x
           sprite->y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_32x32, // tamaina
           SpriteColorFormat_256Color, // kolore formatua
           sprite->spriteGfxMem, // spritea agertzen den helbidea memorian
           -1, // indize afinea
           false, // indize afinea bada tamaina bikoiztu errotazioan
           true, // spritea ezkutatu nahi den, ezin da afinea izan
           false, // bertikalki bolkatu spritea
           false, // horizontalki bolkatu spritea
           false); // mosaicoa aplikatu nahi den
}

/*!
 * \brief 2D sprite motoreak hasieratzen ditu bi pantailetan tiledun eta bitmapdun
 * spriteak ageri daitezen. Baita ere sugearentzat memoria alokatzen du.
 */
void initSprites()
{
    oamInit(&oamMain, SpriteMapping_1D_128, false);   // Jadanik oamUpdate(&oamMain) exekutatzen du
    oamInit(&oamSub, SpriteMapping_1D_128, false);    // Jadanik oamUpdate(&oamSub) exekutatzen du

    initSnake(&snake, (u8*) snakeTiles); // memoria alokatu
}