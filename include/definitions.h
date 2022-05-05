#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <nds.h>		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

/*!
 *  \brief Interrupt Master Enable -- Eten guztiak baimendu-galarazi
 *
 *  Interrupt Master Enable erregistroak mota guztietako etenak baimentzen edo
 *  galarazten ditu. 0 balioa badu eten guztiak galarazita daude, eta 1 balioa
 *  badu etenak onartuta daude.
 */
// "REG_IME"-en definituta "libnds/include/nds/interrupts.h"-en (*(vuint32*)0x04000208) balioarekin

/*!
 *  \brief Interrupt Enable -- Etenak banaka baimendu-galarazi
 *
 *  Interrupt Enable erregistroak eten zehatzak baimentzen edo galarazten ditu.
 *  Baimendu ahal izateko IME erregistroan baimendu behar dira lehenengo. Etena
 *  galarazia dago bitak 0 balioa badu, eta etena baimenduta dago 1 balio badu.
 *  
 *  |Bit|  Maskara  |      Definizioa     ||Bit|      Maskara     |        Definizioa       |
 *  |---|-----------|---------------------||---|------------------|-------------------------|
 *  | 0 | IRQ_VBLANC| Freskatze bertikala || 12|     IRQ_KEYS     |         Teklatua        |
 *  | 1 | IRQ_HBLANC|Freskatze horizontala|| 13|     IRQ_CART     |      GBA kartutxoa      |
 *  | 2 | IRQ_VCOUNT|     Vcount match    || 16|   IRQ_IPC_SYNC   |IPC-arekin sinkronizazioa|
 *  | 3 | IRQ_TIMER0|   0 denboragailua   || 17|  IRQ_FIFO_EMPTY  |    FIFO bidali hutsa    |
 *  | 4 | IRQ_TIMER1|   1 denboragailua   || 18|IRQ_FIFO_NOT_EMPTY|     FIFO jaso betea     |
 *  | 5 | IRQ_TIMER2|   2 denboragailua   || 19|     IRQ_CARD     |       DS kartutxoa      |
 *  | 6 | IRQ_TIMER3|   3 denboragailua   || 20|   IRQ_CARD_LINE  |     Kartutxo lerroa     |
 *  | 7 |IRQ_NETWORK|     Serie ataka     || 21| IRQ_GEOMETRY_FIFO|   Geometriarako FIFOa   |
 *  | 8 |  IRQ_DMA0 |        0 DMA        || 22|      IRQ_LID     |         Bisagra         |
 *  | 9 |  IRQ_DMA1 |        1 DMA        || 23|      IRQ_SPI     |           SPI           |
 *  | 10|  IRQ_DMA2 |        2 DMA        || 24|     IRQ_WIFI     |           WIFI          |
 *  | 11|  IRQ_DMA3 |        3 DMA        || ~0|      IRQ_ALL     |           Dena          |
 */
// "REG_IE"-en definituta "libnds/include/nds/interrupts.h"-en (*(vuint32*)0x04000210) balioarekin

/*!
 *  \brief Interrupt Flag -- Eten eskaeren erregistroa
 *
 *  Interrupt Flag erregistroak eten eskaerak gestionatzen ditu. Bit bakoitzak
 *  eten mota bat errepresentatzen du. Bitak 1 balioa badu etena aktibatu
 *  dela esan nahi du, eta 0 bada ez bada ez da aktibatu.
 *      
 *  |Bit|  Maskara  |      Definizioa     ||Bit|      Maskara     |        Definizioa       |
 *  |---|-----------|---------------------||---|------------------|-------------------------|
 *  | 0 | IRQ_VBLANC| Freskatze bertikala || 12|     IRQ_KEYS     |         Teklatua        |
 *  | 1 | IRQ_HBLANC|Freskatze horizontala|| 13|     IRQ_CART     |      GBA kartutxoa      |
 *  | 2 | IRQ_VCOUNT|     Vcount match    || 16|   IRQ_IPC_SYNC   |IPC-arekin sinkronizazioa|
 *  | 3 | IRQ_TIMER0|   0 denboragailua   || 17|  IRQ_FIFO_EMPTY  |    FIFO bidali hutsa    |
 *  | 4 | IRQ_TIMER1|   1 denboragailua   || 18|IRQ_FIFO_NOT_EMPTY|     FIFO jaso betea     |
 *  | 5 | IRQ_TIMER2|   2 denboragailua   || 19|     IRQ_CARD     |       DS kartutxoa      |
 *  | 6 | IRQ_TIMER3|   3 denboragailua   || 20|   IRQ_CARD_LINE  |     Kartutxo lerroa     |
 *  | 7 |IRQ_NETWORK|     Serie ataka     || 21| IRQ_GEOMETRY_FIFO|   Geometriarako FIFOa   |
 *  | 8 |  IRQ_DMA0 |        0 DMA        || 22|      IRQ_LID     |         Bisagra         |
 *  | 9 |  IRQ_DMA1 |        1 DMA        || 23|      IRQ_SPI     |           SPI           |
 *  | 10|  IRQ_DMA2 |        2 DMA        || 24|     IRQ_WIFI     |           WIFI          |
 *  | 11|  IRQ_DMA3 |        3 DMA        || ~0|      IRQ_ALL     |           Dena          |
 */
// "REG_IF"-en definituta "libnds/include/nds/interrupts.h"-en (*(vuint32*)0x04000214)
// balioarekin

/*!
 *  \brief Mota guztietako eten guztiak baimentzen ditu
 */
#define allowAllInterrupts() REG_IME = IME_ENABLE

/*!
 *  \brief Mota guztietako eten guztiak galarazten ditu
 */
#define denyAllInterrupts() REG_IME = IME_DISABLE

/*!
 *  \brief Teklatuaren datu-erregistroa
 *  
 *  Datu-erregistroak bit bat errepresenatuta izango du tekla bakoitzeko. Bitak
 *  0 balioa izango du tekla sakatuta baldin badago, edo 1 balioa sakatuta ez
 *  badago.
 *
 *  |bit| tekla ||bit| tekla |
 *  |---|-------||---|-------|
 *  | 0 |   A   || 5 |  LEFT |
 *  | 1 |   B   || 6 |   UP  |
 *  | 2 | SELECT|| 7 |  DOWN |
 *  | 3 | START || 8 |   R   |
 *  | 4 | RIGHT || 9 |   L   |
 */
// "REG_KEYINPUT"-en definituta "libnds/include/nds/input.h"-en (*(vu16*)0x4000130)
// balioarekin

/*!
 *  \brief Teklatuaren kontrol-erregistroa
 *
 *  Kontrol-erregistroak bit bat errepresentatuta izango du tekla bakoitzeko.
 *  Bit bakoitzak teklaren sinkronizazioa etenaren bidez edo inkestaren bidez
 *  egongo den adierazten du. Tekla etenen bidez baimenduta egoteko bitak 1
 *  balioa hartuko du, eta ez badago baimenduta, inkesta bidez erabiltzeko,
 *  0 balioa.
 *
 *  Horrez gain, 14 eta 15 bitak erabiltzen dira.
 *
 *  14 bitak teklatu eten orokorrak ea baimenduta dauden adierazten du.
 *  Baimenduta badaude 1 balioa badu, eta ez badaude, 0 balioa.
 *
 *  15 bitak teklatu eten baldintza nolakoa izango den adierazten du. 0 balioa
 *  badu OR baldintza izango du, eta 1 balioa badu, AND baldintza.
 *
 *  |bit| tekla || bit| tekla |
 *  |---|-------||----|-------|
 *  | 0 |   A   ||  6 |   UP  |
 *  | 1 |   B   ||  7 |  DOWN |
 *  | 2 | SELECT||  8 |   R   |
 *  | 3 | START ||  9 |   L   |
 *  | 4 | RIGHT || 14 | OFF/ON|
 *  | 5 |  LEFT || 15 | OR/AND|
 */
// "REG_KEYCNT"-en definituta "libnds/include/nds/input.h"-en (*(vu16*)0x4000132)
// balioarekin

/*!
 *  \brief Timer0 denboragailuaren kontrol erregistroa
*/
// "TIMER0_CR"-en definituta "libnds/include/nds/timers.h"-en (*(vu16*)0x04000102)
// balioarekin

/*!
 *  \brief Timer0 denboragailuaren datu-erregistroa
 */
// "TIMER0_DATA"-en definituta "libnds/include/nds/timers.h"-en (*(vu16*)0x04000100)
// balioarekin

/*!
 *  \brief Ukimen-pantailaren erregistroa
 */
extern touchPosition PANT_DAT;

/*!
 * \brief Irudiak memorian kopiatzeko DMA kanala aukeratu
 */
#define DMA_CHANNEL 3

/******************************************************************************
 * begin { Automata }
******************************************************************************/

/*!
 * \brief Automataren egoera posibleak gordetzen ditu.
 */
typedef enum AUTOMATON_STATES {
    AUTOMATON_START, // Jokoa irekitzean
    AUTOMATON_SELECTION, // Joko mota aukeratzen
    AUTOMATON_PLAYING, // Jolasten
    AUTOMATON_PAUSED, // Jokoa pausatuta
    AUTOMATON_ENDING // Jokoa amaituta
} AUTOMATON_STATES;

/*!
 * \brief Automataren egoera gordeko duen aldagaia deklaratzen da c
 * fitxategi batean.
 */
extern int AUTOMATON_STATE;

/******************************************************************************
 * end { Automata }
******************************************************************************/

#define ETEN_SEGUNDOKO 10
#define SEG_0_5 (ETEN_SEGUNDOKO / 2)
#define SEG_1 (ETEN_SEGUNDOKO * 1)

/******************************************************************************
 * begin { Sugea }
******************************************************************************/

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
 * \brief Suge objektuak izan detzakeen egoerak definitzen dira
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

/******************************************************************************
 * end { Sugea }
******************************************************************************/

#endif