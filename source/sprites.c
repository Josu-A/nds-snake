#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "definitions.h"
#include "sprites.h"
#include "objectSnake.h"

#include "spriteSnakeHead.h"
#include "spriteSnakeBody.h"
#include "spriteSnakeTail.h"

/*!
 * \brief Sugea buruaren spritearentzako memoria alokatzen da, eta alokatutako
 * helbidea parametroz jasotako snake-aren erakuslearen buruaren spriteGfxMem
 * aldagaian gordetzen da. Horrez gain, spriteak dituen tile-en informazioaren
 * erakuslea gordetzen da buruko frameGfx-en.
 * 
 * \param snake suge egituraren erakuslea
 * \param gfx suge buruaren tile-en erakuslea
 */
static void initSnakeHead(Snake *snake, u8 *gfx)
{
    snake->snakeHead.spriteGfxMem = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
    snake->snakeHead.frameGfx = (u8*) gfx;
    snake->snakeHead.spriteId = 0;
}

/*!
 * \brief Suge buruaren egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du. Indize honen bitartez memorian gordeta dagoen tile-en
 * erakusletik nahi dugun frame-aren helbidea lortzen da, eta azkenik bertako
 * framea memoriatik spritea irakurtzen den lekuan kopiatzen da spritea eguneratu
 * ahal izateko.
 *
 * \param snake suge egituraren erakuslea
 */
void animateSnakeHead(Snake *snake)
{
    int frame = snake->snakeHead.animFrame + snake->snakeHead.state * HEAD_FRAMES_PER_ANIMATION;
	u8 *offset = snake->snakeHead.frameGfx + frame * SNAKE_DIMENSION * SNAKE_DIMENSION / 2;
    dmaCopyHalfWords(DMA_CHANNEL,
                     offset,
                     snake->snakeHead.spriteGfxMem,
                     SNAKE_DIMENSION * SNAKE_DIMENSION);
}

/*!
 * \brief spriteSnakeHead-ren paleta kopiatzen du pantaila nagusiko paletara.
 *
 * \param snake suge egituraren erakuslea
 */
void saveIntoMemorySnakeHead(Snake *snake)
{
    dmaCopyHalfWords(DMA_CHANNEL,
                     spriteSnakeHeadPal,
                     SPRITE_PALETTE,
                     spriteSnakeHeadPalLen);
}

/*!
 * \brief Sugea gorputzaren spritearentzako memoria alokatzen da, eta alokatutako
 * helbidea parametroz jasotako snake-aren erakuslearen gorputzaren spriteGfxMem
 * aldagaian gordetzen da. Horrez gain, spriteak dituen tile-en informazioaren
 * erakuslea gordetzen da gorputzeko frameGfx-en.
 * 
 * \param snake suge egituraren erakuslea
 * \param gfx suge gorputzaren tile-en erakuslea
 */
static void initSnakeBody(Snake *snake, u8 *gfx)
{
    snake->snakeBody[0].spriteGfxMem = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
    snake->snakeBody[0].frameGfx = (u8*) gfx;
    snake->numSnakeBody = MIN_SNAKE_BODY_PARTS;
    snake->snakeBody[0].spriteId = 1;
}

/*!
 * \brief Suge gorputzaren egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du. Indize honen bitartez memorian gordeta dagoen tile-en
 * erakusletik nahi dugun frame-aren helbidea lortzen da, eta azkenik bertako
 * framea memoriatik spritea irakurtzen den lekuan kopiatzen da spritea eguneratu
 * ahal izateko.
 *
 * \param snake suge egituraren erakuslea
 */
void animateSnakeBody(Snake *snake)
{
    int frame = snake->snakeBody[0].animFrame + snake->snakeBody[0].state * BODY_FRAMES_PER_ANIMATION;
	u8 *offset = snake->snakeBody[0].frameGfx + frame * SNAKE_DIMENSION * SNAKE_DIMENSION / 2;
    dmaCopyHalfWords(DMA_CHANNEL,
                     offset,
                     snake->snakeBody[0].spriteGfxMem,
                     SNAKE_DIMENSION * SNAKE_DIMENSION);
}

/*!
 * \brief spriteSnakeBody-ren paleta kopiatzen du pantaila nagusiko paletara.
 *
 * \param snake suge egituraren erakuslea
 */
void saveIntoMemorySnakeBody(Snake *snake)
{
    dmaCopyHalfWords(DMA_CHANNEL,
                     spriteSnakeBodyPal,
                     &SPRITE_PALETTE[16],
                     spriteSnakeBodyPalLen);
}

/*!
 * \brief Sugea buruaren spritearentzako memoria alokatzen da, eta alokatutako
 * helbidea parametroz jasotako snake-aren erakuslearen buruaren spriteGfxMem
 * aldagaian gordetzen da. Horrez gain, spriteak dituen tile-en informazioaren
 * erakuslea gordetzen da buruko frameGfx-en.
 * 
 * \param snake suge egituraren erakuslea
 * \param gfx sugea buruaren tile-en erakuslea
 */
static void initSnakeTail(Snake *snake, u8 *gfx)
{
    snake->snakeTail.spriteGfxMem = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
    snake->snakeTail.frameGfx = (u8*) gfx;
    snake->snakeTail.spriteId = 2;
}

/*!
 * \brief Suge isatsaren egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du. Indize honen bitartez memorian gordeta dagoen tile-en
 * erakusletik nahi dugun frame-aren helbidea lortzen da, eta azkenik bertako
 * framea memoriatik spritea irakurtzen den lekuan kopiatzen da spritea eguneratu
 * ahal izateko.
 *
 * \param snake suge egituraren erakuslea
 */
void animateSnakeTail(Snake *snake)
{
    int frame = snake->snakeTail.animFrame + snake->snakeTail.state * TAIL_FRAMES_PER_ANIMATION;
	u8 *offset = snake->snakeTail.frameGfx + frame * SNAKE_DIMENSION * SNAKE_DIMENSION / 2;
    dmaCopyHalfWords(DMA_CHANNEL,
                     offset,
                     snake->snakeTail.spriteGfxMem,
                     SNAKE_DIMENSION * SNAKE_DIMENSION);
}

/*!
 * \brief spriteSnakeTail-ren paleta kopiatzen du pantaila nagusiko paletara.
 *
 * \param snake suge egituraren erakuslea
 */
void saveIntoMemorySnakeTail(Snake *snake)
{
    dmaCopyHalfWords(DMA_CHANNEL,
                     spriteSnakeTailPal,
                     &SPRITE_PALETTE[32],
                     spriteSnakeTailPalLen);
}

/*!
 * \brief OAM sarrera zehazten du sugea ikusarazteko zehaztutako balioekin.
 *
 * \param snake suge egitura duen erakuslea
 */
void displaySnake(Snake *snake)
{
    oamSet(&oamMain, // oam
           snake->snakeHead.spriteId, // id
           snake->snakeHead.x, // x
           snake->snakeHead.y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_16x16, // tamaina 
           SpriteColorFormat_16Color, // kolore formatua
           snake->snakeHead.spriteGfxMem, // spritea agertzen den helbidea memorian
           -1, // indize afinea
           false, // indize afinea bada tamaina bikoiztu errotazioan
           false, // spritea ezkutatu nahi den, ezin da afinea izan
           false, // bertikalki bolkatu spritea
           false, // horizontalki bolkatu spritea
           false); // mosaicoa aplikatu nahi den
    oamSet(&oamMain, // oam
           snake->snakeBody[0].spriteId, // id
           snake->snakeBody[0].x, // x
           snake->snakeBody[0].y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_16x16, // tamaina 
           SpriteColorFormat_16Color, // kolore formatua
           snake->snakeBody[0].spriteGfxMem, // spritea agertzen den helbidea memorian
           -1, // indize afinea
           false, // indize afinea bada tamaina bikoiztu errotazioan
           false, // spritea ezkutatu nahi den, ezin da afinea izan
           false, // bertikalki bolkatu spritea
           false, // horizontalki bolkatu spritea
           false); // mosaicoa aplikatu nahi den
    oamSet(&oamMain, // oam
           snake->snakeTail.spriteId, // id
           snake->snakeTail.x, // x
           snake->snakeTail.y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_16x16, // tamaina 
           SpriteColorFormat_16Color, // kolore formatua
           snake->snakeTail.spriteGfxMem, // spritea agertzen den helbidea memorian
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
 * \param snake suge egitura duen erakuslea
 */
void hideSnake(Snake *snake)
{
    oamSet(&oamMain, // oam
           snake->snakeHead.spriteId, // id
           snake->snakeHead.x, // x
           snake->snakeHead.y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_16x16, // tamaina
           SpriteColorFormat_16Color, // kolore formatua
           snake->snakeHead.spriteGfxMem, // spritea agertzen den helbidea memorian
           -1, // indize afinea
           false, // indize afinea bada tamaina bikoiztu errotazioan
           true, // spritea ezkutatu nahi den, ezin da afinea izan
           false, // bertikalki bolkatu spritea
           false, // horizontalki bolkatu spritea
           false); // mosaicoa aplikatu nahi den
    oamSet(&oamMain, // oam
           snake->snakeBody[0].spriteId, // id
           snake->snakeBody[0].x, // x
           snake->snakeBody[0].y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_16x16, // tamaina 
           SpriteColorFormat_16Color, // kolore formatua
           snake->snakeBody[0].spriteGfxMem, // spritea agertzen den helbidea memorian
           -1, // indize afinea
           false, // indize afinea bada tamaina bikoiztu errotazioan
           true, // spritea ezkutatu nahi den, ezin da afinea izan
           false, // bertikalki bolkatu spritea
           false, // horizontalki bolkatu spritea
           false); // mosaicoa aplikatu nahi den
    oamSet(&oamMain, // oam
           snake->snakeTail.spriteId, // id
           snake->snakeTail.x, // x
           snake->snakeTail.y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_16x16, // tamaina 
           SpriteColorFormat_16Color, // kolore formatua
           snake->snakeTail.spriteGfxMem, // spritea agertzen den helbidea memorian
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

    initSnakeHead(&snake, (u8*) spriteSnakeHeadTiles); // memoria alokatu
    initSnakeBody(&snake, (u8*) spriteSnakeBodyTiles);
    initSnakeTail(&snake, (u8*) spriteSnakeTailTiles);
}