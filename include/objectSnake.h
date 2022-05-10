#ifndef _OBJECTSNAKE_H_
#define _OBJECTSNAKE_H_

/*!
 * \brief Suge spritearen luzera/altuera
 */
#define SNAKE_DIMENSION 32

/*!
 * \brief Sugearen mugimendua azkartzen edo moteltzen du. 1 azkarrena, IntSize motelena.
 */
#define ANIMATION_SPEED 3

/*!
 * \brief Sugearen spritearen irudiak egoera bakoitzeko zenbat sub-sprite horizontal dituen.
 */
#define FRAMES_PER_ANIMATION 3

/*!
 * \brief Suge objektuak izan ditzakeen egoerak definitzen dira
 */
typedef enum SnakeSpriteState {
    W_UP, // Sugea gorantz doa
    W_RIGHT, // Sugea eskuinerantz doa
    W_DOWN, // Sugea beherantz doa
    W_LEFT // Sugea ezkerrerantz doa
} SnakeSpriteState;

/*!
 * \brief Sugea objektu bat definitzen du.
 */
typedef struct Snake
{
    int x; // Objektuaren x posizioa gordetzen du
    int y; // Objektuaren y posizioa gordetzen du

    u16* spriteGfxMem; // Sprite-a gordeko den memoriaren helbidea gordetzen du
    u8* frameGfx; // Sprite-ak dituen tile kopurua gordetzen du. Tile bat = 4 pixel dira.

    SnakeSpriteState state; // Objektuaren egoera gordetzen du
    int animFrame; // Objektuaren egoeraren zenbatgarren fotograman dagoen adierazten du
} Snake;

/*!
 * \brief Suge objektua gordetzen du.
 */
extern Snake snake;

/*!
 * \brief Sugeak norabidea aldatzeko aukerak dira
 */
typedef enum SNAKE_ROTATE_PERMISSION {
    SNAKE_CAN_NOT_ROTATE,
    SNAKE_CAN_ROTATE
} SNAKE_ROTATE_PERMISSION;

/*!
 * \brief Jokuak jokalariari aukera eman edo kendu egiten dio sugearen
 * norabidea aldatzeko.
 */
extern SNAKE_ROTATE_PERMISSION canSnakeRotate;

/*!
 * \brief Sugearen erakuslea jaso eta mugitu egingo du norabide egokian baldin
 * eta soilik baldin pantaila barruko esparruan badago. Hurrengo mugimenduan
 * sugeak eremutik aterako balitz, erabiltzaileari kontrola kentzen dio eta 
 * mugitzen gelditzen da.
 * 
 * \param snake suge objektuaren erakuslea
 */
extern void moveSnake(Snake *snake);

/*!
 * \brief Sugearen erakuslea jaso eta honi egoera berria ezarriko dio teklatutik
 * jasotako norabidera, baldin eta soilik baldin aukera badu norabidez aldatzeko.
 * 
 * \param snake suge objektuaren erakuslea
 */
extern void updateRotationStateSnake(Snake *snake);

/*!
 * \brief Sugearen norabide berdineko erakutsi beharreko sub-spritearen
 * posizioa handitzen edo reseteatzen du jolasak spritea aldatzeko (animatzeko)
 * jasotako sugearen erakuslea jasota.
 * 
 * \param snake suge objektuaren erakuslea
 */
extern void changeAnimationFrameSnake(Snake *snake);

/*!
 * \brief Sugearen erakuslea jasota honen posizioa ezartzen da pantailan
 * emandako x eta y balioekin.
 * 
 * \param snake suge objektuaren erakuslea
 * \param x posizio horizontala, 0tik SCREEN_WIDTH (256)-ra
 * \param y posizio bertikala,  0tik SCREEN_HEIGHT (192)-ra
 */
extern void setPositionSnake(Snake *snake, int x, int y);

/*!
 * \brief Sugearen erakuslea jasota honen hasierako norabide egoera
 * hasieratzen du.
 * 
 * \param snake suge objektuaren erakuslea
 */
extern void setDefaultRotationSnake(Snake *sprite);

#endif