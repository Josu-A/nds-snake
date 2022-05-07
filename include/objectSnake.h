#ifndef _OBJECTSNAKE_H_
#define _OBJECTSNAKE_H_

#define SNAKE_DIMENSION 32
#define ANIMATION_SPEED 3
#define FRAMES_PER_ANIMATION 3

/*!
 * \brief Sugea objektu bat definitzen du.
 */
typedef struct Snake
{
    int x; // Objektuaren x posizioa gordetzen du
    int y; // Objektuaren y posizioa gordetzen du

    u16* spriteGfxMem; // Sprite-a gordeko den memoriaren helbidea gordetzen du
    u8* frameGfx; // Sprite-ak dituen tile kopurua gordetzen du. Tile bat = 4 pixel dira.

    int state; // Objektuaren egoera gordetzen du
    int animFrame; // Objektuaren egoeraren zenbatgarren fotograman dagoen adierazten du
} Snake;

/*!
 * \brief Suge objektu bat deklaratzen da c fitxategi batean.
 */
extern Snake snake;

/*!
 * \brief Suge objektuak izan ditzakeen egoerak definitzen dira
 */
enum SnakeSpriteState {
    W_UP, // Sugea gorantz doa
    W_RIGHT, // Sugea eskuinerantz doa
    W_DOWN, // Sugea beherantz doa
    W_LEFT // Sugea ezkerrerantz doa
};

typedef enum SNAKE_ROTATE_PERMISSION {
    SNAKE_CAN_NOT_ROTATE,
    SNAKE_CAN_ROTATE
} SNAKE_ROTATE_PERMISSION;

extern SNAKE_ROTATE_PERMISSION canSnakeRotate;

extern void moveSnake(Snake *snake);
extern void rotateSnake(Snake *snake);
extern void changeAnimationFrameSnake(Snake *snake);
extern void setPositionSnake(Snake *snake, int x, int y);
extern void setDefaultRotationSnake(Snake *sprite);

#endif