#ifndef _OBJECTSNAKE_H_
#define _OBJECTSNAKE_H_

/*!
 * \brief Suge spritearen luzera/altuera
 */
#define SNAKE_DIMENSION 16

/*!
 * \brief Sugearen mugimendua azkartzen edo moteltzen du. 1 azkarrena, IntSize motelena.
 */
#define ANIMATION_SPEED 1

/*!
 * \brief Suge buruaren spritearen irudiak egoera bakoitzeko zenbat sub-sprite
 * horizontal dituen.
 */
#define HEAD_FRAMES_PER_ANIMATION 4

/*!
 * \brief Suge buru objektuak izan ditzakeen egoerak definitzen dira
 */
typedef enum SpriteSnakeHeadState {
    W_HEAD_UP, // Suge burua gorantz doa
    W_HEAD_RIGHT, // Suge burua eskuinerantz doa
    W_HEAD_DOWN, // Suge burua beherantz doa
    W_HEAD_LEFT // Suge burua ezkerrerantz doa
} SpriteSnakeHeadState;

/*!
 * \brief Suge buru objektu bat definitzen du.
 */
typedef struct SnakeHead
{
    int x; // Objektuaren x posizioa gordetzen du
    int y; // Objektuaren y posizioa gordetzen du

    u16 *spriteGfxMem; // Sprite-a gordeko den memoriaren helbidea gordetzen du
    u8 *frameGfx; // Sprite-ak dituen tile kopurua gordetzen du. Tile bat = 4 pixel dira.

    SpriteSnakeHeadState state; // Objektuaren egoera gordetzen du
    int animFrame; // Objektuaren egoeraren zenbatgarren fotograman dagoen adierazten du

    int spriteId; // OAMen izango duen identifikatzailea
} SnakeHead;

/*!
 * \brief Sugeak izan ditzakeen gorputz zati maximoa
 */
#define MAX_SNAKE_BODY_PARTS 50

/*!
 * \brief Sugeak hasieran dituen gorputz kopurua
 */
#define MIN_SNAKE_BODY_PARTS 1

/*!
 * \brief Suge gorputzaren spritearen irudiak egoera bakoitzeko zenbat sub-sprite
 * horizontal dituen.
 */
#define BODY_FRAMES_PER_ANIMATION 2

/*!
 * \brief Suge gorputz objektuak izan ditzakeen egoerak definitzen dira
 */
typedef enum SpriteSnakeBodyState {
    W_BODY_VERTICAL, // Suge gorputza bertikalean mugitzen ari da
    W_BODY_HORIZONTAL, // Suge gorputza horizontalean mugitzen ari da
    W_BODY_NE, // Suge gorputza biratzen ari da goitik eta eskuinetik
    W_BODY_ES, // Suge gorputza biratzen ari da eskuinetik eta behetik
    W_BODY_SW, // Suge gorputza biratzen ari da behetik eta ezkerretik
    W_BODY_WN // Suge gorputza biratzen ari da ezkerretik eta goitik
} SpriteSnakeBodyState;

/*!
 * \brief Suge gorputz objektu bat definitzen du.
 */
typedef struct SnakeBody
{
    int x; // Objektuaren x posizioa gordetzen du
    int y; // Objektuaren y posizioa gordetzen du

    int prevX; // Objektuaren pausu bat atzera izan duen x posizioa
    int prevY; // Objektuaren pausu bat atzera izan duen y posizioa

    u16 *spriteGfxMem; // Spritea gordeko den memoriaren helbidea gordetzen du
    u8 *frameGfx; // Spriteak dituen tile kopurua gordetzen du. Tile bat = 4 pixel dira.

    SpriteSnakeBodyState state; // Objektuaren egoera gordetzen du
    int animFrame; // Objektuaren egoeraren zenbatgarren fotograman dagoen adierazten du

    int spriteId; // OAMen izango duen identifikatzailea
} SnakeBody;

/*!
 * \brief Suge isatsaren spritearen irudiak egoera bakoitzeko zenbat sub-sprite
 * horizontal dituen.
 */
#define TAIL_FRAMES_PER_ANIMATION 2

/*!
 * \brief Suge isats objektuak izan ditzakeen egoerak definitzen dira
 */
typedef enum SpriteSnakeTailState {
    W_TAIL_UP, // Suge isatsa gorantz doa
    W_TAIL_RIGHT, // Suge isatsa eskuinerantz doa
    W_TAIL_DOWN, // Suge isatsa beherantz doa
    W_TAIL_LEFT // Suge isatsa ezkerrerantz doa
} SpriteSnakeTailState;

/*!
 * \brief Suge gorputz objektu bat definitzen du.
 */
typedef struct SnakeTail
{
    int x; // Objektuaren x posizioa gordetzen du
    int y; // Objektuaren y posizioa gordetzen du

    u16 *spriteGfxMem; // Spritea gordeko den memoriaren helbidea gordetzen du
    u8 *frameGfx; // Spriteak dituen tile kopurua gordetzen du. Tile bat = 4 pixel dira.

    SpriteSnakeTailState state; // Objektuaren egoera gordetzen du
    int animFrame; // Objektuaren egoeraren zenbatgarren fotograman dagoen adierazten du

    int spriteId; // OAMen izango duen identifikatzailea
} SnakeTail;

/*!
 * \brief Suge oso bat definitzen da.
 */
typedef struct Snake
{
    SnakeHead snakeHead; // Sugearen burua
    int numSnakeBody; // Zenbat gorputz zati dituen orain
    SnakeBody snakeBody[MAX_SNAKE_BODY_PARTS]; // Sugearen gorputzak
    SnakeTail snakeTail; // Sugearen isatsa
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
 * \brief Sugeak bizirik edo hilda izateko balio posibleak zehazten ditu.
 */
typedef enum SNAKE_DEATH_STATE {
    SNAKE_ALIVE,
    SNAKE_DEAD
} SNAKE_DEATH_STATE;

/*!
 * \brief Sugea bizirik dagoen adiJokuak jokalariari aukera eman edo kendu egiten dio sugearen
 * norabidea aldatzeko.
 */
extern SNAKE_DEATH_STATE isSnakeDead;

/*!
 * \brief Sugearen erakuslea jaso eta mugitu egingo du norabide egokian baldin
 * eta soilik baldin pantaila barruko esparruan badago. Hurrengo mugimenduan
 * sugeak eremutik aterako balitz, erabiltzaileari kontrola kentzen dio eta 
 * mugitzen gelditzen da.
 * 
 * \param snake suge buru objektuaren erakuslea
 */
extern void moveSnake(Snake *snake);

/*!
 * \brief Sugearen erakuslea jaso eta suge buruaren egoera berria ezarriko dio
 * teklatutik jasotako norabidera, baldin eta soilik baldin aukera badu norabidez
 * aldatzeko.
 * 
 * \param snake suge objektuaren erakuslea
 */
extern void updateRotationStateSnakeHead(Snake *snake);

/*!
 * \brief Sugearen erakuslea jaso eta suge gorputzaren egoera berria ezarriko
 * dio teklatutik jasotako norabidera, baldin eta soilik baldin aukera badu
 * norabidez aldatzeko.
 * 
 * \param snake suge objektuaren erakuslea
 */
extern void updateRotationStateSnakeBody(Snake *snake);

/*!
 * \brief Sugearen erakuslea jaso eta suge isatsaren egoera berria ezarriko
 * dio teklatutik jasotako norabidera, baldin eta soilik baldin aukera badu
 * norabidez aldatzeko.
 * 
 * \param snake suge objektuaren erakuslea
 */
extern void updateRotationStateSnakeTail(Snake *snake);

/*!
 * \brief Sugearen norabide berdineko erakutsi beharreko sub-spritearen
 * posizioa handitzen edo reseteatzen du jolasak spritea aldatzeko (animatzeko)
 * jasotako sugearen erakuslea jasota.
 * 
 * \param snake suge objektuaren erakuslea
 */
extern void changeAnimationFrameSnake(Snake *snake);

/*!
 * \brief Sugea berrezartzen du.
 * 
 * \param snake suge objektuaren erakuslea
 */
extern void resetSnake(Snake *snake);

#endif