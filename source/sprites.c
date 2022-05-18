#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "definitions.h"
#include "sprites.h"
#include "objectSnake.h"
#include "objectApple.h"

/*!
 * \brief 8 bitez adierazitako zenbakia ([0, 255] tartean) 4 bitez adierazitako
 * tartera ([0, 31]) bihurtzen du, zenbaki gertuerena borobilduz.
 * 
 * \param value8Bit bihurtu nahi den 8bit zenbakia
 * \return emandako zenbakiaren 4bit-eko bihurketa
 */
static int convert8BitsTo4Bits(int value8Bit)
{
    return value8Bit * ((1<<4) - 1) / ((1<<8) - 1);
}

/*!
 * \brief 8 bitez adierazitako RGB balio bat jasoz 3 parametroen bidez, NDSak
 * irakurri dezakeen 15 bitez adierazitako kolore formatu batera bihurtzen eta
 * itzultzen du.
 * 
 * \param colorRed8bit [0, 255] adierazitako gorri kopurua
 * \param colorGreen8bit [0, 255] adierazitako berde kopurua
 * \param colorBlue8bit [0, 255] adierazitako urdin kopurua
 * \return NDS ulertzen duen 15biteko kolorea
 */
static int convertColor8BitsToRGB15(int colorRed8bit, int colorGreen8bit, int colorBlue8bit)
{
    return RGB15(convert8BitsTo4Bits(colorRed8bit), 
                 convert8BitsTo4Bits(colorGreen8bit), 
                 convert8BitsTo4Bits(colorBlue8bit));
}

/*!
 * \brief Pantaila nagusiko sprite-ek erabiltzeko kolore paleta ezartzen du.
 * Lehenengo kolorea, SPRITE_PALLETE[0], kolore gardena gisa erabiltzen da.
 */
static void setMainPallete()
{
    SPRITE_PALETTE[1] = convertColor8BitsToRGB15(0, 0, 0); // guztiz beltza
    SPRITE_PALETTE[2] = convertColor8BitsToRGB15(237, 237, 237); // zuri iluna
    SPRITE_PALETTE[3] = convertColor8BitsToRGB15(255, 255, 255); // guztiz zuri
    SPRITE_PALETTE[4] = convertColor8BitsToRGB15(0, 58, 1); // berde iluna
    SPRITE_PALETTE[5] = convertColor8BitsToRGB15(240, 5, 5); // gorria, sugearen mihiarentzat
    SPRITE_PALETTE[6] = convertColor8BitsToRGB15(0, 134, 20); // berde argia
    SPRITE_PALETTE[7] = convertColor8BitsToRGB15(255, 0, 0); // guztiz gorria
}

static u8 spriteSnakeHead[4096] =
{
    // Suge burua gorantz egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,1,1, 0,0,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,3,3,4,4, 0,4,4,3,1,2,3,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,0,0,0,0, 1,1,4,4,4,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,3,3,4,4,4,0, 4,3,1,2,3,4,4,0, // 2 kuadrantea
    0,4,4,3,2,2,3,4, 0,4,4,4,3,3,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,2,2,3,4,4,0, 4,4,3,3,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge burua gorantz egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,4,4,4,5, 0,0,0,4,4,4,1,5, 0,0,4,4,4,4,4,1, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,3,3,4,4, 0,4,4,3,1,2,3,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,0,0,0,0, 5,1,4,4,4,0,0,0, 1,4,4,4,4,4,0,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,3,3,4,4,4,0, 4,3,1,2,3,4,4,0, // 2 kuadrantea
    0,4,4,3,2,2,3,4, 0,4,4,4,3,3,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,2,2,3,4,4,0, 4,4,3,3,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge burua gorantz egoera 3
    0,0,0,0,0,0,0,0, 0,0,0,0,4,4,4,5, 0,0,0,4,4,1,1,1, 0,0,4,4,4,4,1,5, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,3,3,4,4, 0,4,4,3,1,2,3,4, // 1 kuadrantea
    5,0,0,0,0,0,0,0, 4,4,4,4,0,0,0,0, 5,1,1,4,4,0,0,0, 5,1,4,4,4,4,0,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,3,3,4,4,4,0, 4,3,1,2,3,4,4,0, // 2 kuadrantea
    0,4,4,3,2,2,3,4, 0,4,4,4,3,3,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,2,2,3,4,4,0, 4,4,3,3,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge burua gorantz egoera 4
    0,0,0,0,0,0,0,0, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,1,5, 0,0,4,4,4,4,4,1, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,3,3,4,4, 0,4,4,3,1,2,3,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 5,4,4,4,0,0,0,0, 5,1,4,4,4,0,0,0, 1,4,4,4,4,4,0,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,3,3,4,4,4,0, 4,3,1,2,3,4,4,0, // 2 kuadrantea
    0,4,4,3,2,2,3,4, 0,4,4,4,3,3,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,2,2,3,4,4,0, 4,4,3,3,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge burua eskubirantz egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,4,0,0,0, 4,3,3,4,4,4,0,0, 3,2,2,3,4,4,0,0, 3,2,1,3,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,3,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,1,4,4,4,4,0,0, 4,4,1,1,1,1,0,0, 4,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea
    
    // Suge burua eskubirantz egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,4,0,0,0, 4,3,3,4,4,4,0,0, 3,2,2,3,4,4,0,0, 3,2,1,3,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,3,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,0,0,0,0,0,0, 4,0,0,5,0,5,5,0, 4,4,0,0,5,0,0,0, 4,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea
    
    // Suge burua eskubirantz egoera 3
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,4,0,0,0, 4,3,3,4,4,4,0,0, 3,2,2,3,4,4,0,0, 3,2,1,3,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,3,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,0,0,5,0,0,0, 4,0,0,5,0,5,5,5, 4,4,0,0,0,0,0,0, 4,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea
    
    // Suge burua eskubirantz egoera 4
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,4,0,0,0, 4,3,3,4,4,4,0,0, 3,2,2,3,4,4,0,0, 3,2,1,3,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,3,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,1,4,4,4,4,0,0, 4,4,1,1,1,5,5,0, 4,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge burua beherantz egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,3,3,4,4, 0,4,4,3,2,2,3,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,3,3,4,4,4,0, 4,3,2,2,3,4,4,0, // 2 kuadrantea
    0,4,4,3,2,1,3,4, 0,4,4,4,3,3,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,4,4,4,1,1, 0,0,0,0,4,4,4,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,2,1,3,4,4,0, 4,4,3,3,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,0,0, 1,1,4,4,4,0,0,0, 4,4,4,4,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge burua beherantz egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,3,3,4,4, 0,4,4,3,2,2,3,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,3,3,4,4,4,0, 4,3,2,2,3,4,4,0, // 2 kuadrantea
    0,4,4,3,2,1,3,4, 0,4,4,4,3,3,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,0,4,4,4,4,4,1, 0,0,0,4,4,4,1,5, 0,0,0,0,4,4,4,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,2,1,3,4,4,0, 4,4,3,3,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 1,4,4,4,4,4,0,0, 5,1,4,4,4,0,0,0, 5,4,4,4,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge burua beherantz egoera 3
    0,0,0,0,0,0,0,0, 0,0,0,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,3,3,4,4, 0,4,4,3,2,2,3,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,3,3,4,4,4,0, 4,3,2,2,3,4,4,0, // 2 kuadrantea
    0,4,4,3,2,1,3,4, 0,4,4,4,3,3,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,0,4,4,4,4,1,5, 0,0,0,4,4,1,1,5, 0,0,0,0,4,4,4,4, 0,0,0,0,0,0,0,5, // 3 kuadrantea
    4,3,2,1,3,4,4,0, 4,4,3,3,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 5,1,4,4,4,4,0,0, 1,1,1,4,4,0,0,0, 5,4,4,4,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge burua beherantz egoera 4
    0,0,0,0,0,0,0,0, 0,0,0,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,3,3,4,4, 0,4,4,3,2,2,3,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,3,3,4,4,4,0, 4,3,2,2,3,4,4,0, // 2 kuadrantea
    0,4,4,3,2,1,3,4, 0,4,4,4,3,3,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,0,4,4,4,4,4,1, 0,0,0,4,4,4,1,5, 0,0,0,0,4,4,4,5, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,3,2,1,3,4,4,0, 4,4,3,3,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 1,4,4,4,4,4,0,0, 5,1,4,4,4,0,0,0, 4,4,4,4,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge burua ezkerrerantz egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,4,4,4,3,3,4, 0,0,4,4,3,2,2,3, 0,0,4,4,3,1,2,3, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,3,3,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,1,4, 0,0,1,1,1,1,4,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea
    
    // Suge burua ezkerrerantz egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,4,4,4,3,3,4, 0,0,4,4,3,2,2,3, 0,0,4,4,3,1,2,3, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,3,3,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,4,4, 0,5,5,0,5,0,0,4, 0,0,0,5,0,0,4,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea
    
    // Suge burua ezkerrerantz egoera 3
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,4,4,4,3,3,4, 0,0,4,4,3,2,2,3, 0,0,4,4,3,1,2,3, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,3,3,4, 0,0,4,4,4,4,4,4, 0,0,0,5,0,0,4,4, 5,5,5,0,5,0,0,4, 0,0,0,0,0,0,4,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea
    
    // Suge burua ezkerrerantz egoera 4
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,4,4,4,3,3,4, 0,0,4,4,3,2,2,3, 0,0,4,4,3,1,2,3, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,3,3,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,1,4, 0,5,5,1,1,1,4,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0  // 4 kuadrantea
};

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
    snake->snakeHead.spriteGfxMem = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
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
static void animateSnakeHead(Snake *snake)
{
    int frame = snake->snakeHead.animFrame + snake->snakeHead.state
                * HEAD_FRAMES_PER_ANIMATION;
	int offset = frame * SNAKE_DIMENSION * SNAKE_DIMENSION;

    int i;
    for (i = 0; i < SNAKE_DIMENSION * SNAKE_DIMENSION / 2; i++)
        snake->snakeHead.spriteGfxMem[i] = snake->snakeHead.frameGfx[offset + i*2]
                                           | (snake->snakeHead.frameGfx[offset + i*2 + 1] << 8);
}

static u8 spriteSnakeBody[3072] =
{
    // Suge gorputza bertikalki egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,4,4,4,4,4, 0,0,0,6,6,6,6,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,6,6,6,6,6,4,4, 0,4,4,4,4,4,6,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,0,0,0, 4,4,6,6,6,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,0,0,0,0,0, 4,4,4,0,0,0,0,0, // 2 kuadrantea
    0,6,6,6,6,6,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,0,4,4,4,4,4,6, 0,0,0,4,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,0,0,0,0, 6,6,6,6,0,0,0,0, 4,4,4,4,4,0,0,0, 6,6,6,6,6,0,0,0, 4,4,4,4,4,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza bertikalki egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,4,4,4,4,4, 0,0,0,6,6,6,6,4, 0,0,0,4,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,0,0,6,6,6, 0,0,0,0,0,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,4,0,0, 4,4,6,6,6,6,0,0, 4,4,4,4,4,4,0,0, 6,4,4,4,4,4,4,0, 4,6,4,4,4,4,4,0, // 2 kuadrantea
    0,0,0,0,0,6,6,6, 0,0,0,0,4,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    6,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,6,6,6,6,0, 4,4,6,4,4,4,4,0, 4,4,4,6,6,6,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza horizontalki egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,4,6,4,6,0,0,0, 0,4,6,4,6,4,0,0, 0,4,6,4,6,4,4,4, 0,4,6,6,6,4,4,4, 0,4,4,6,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,4,6,4,4,0, 0,0,6,4,6,4,4,0, 4,4,6,4,6,4,4,0, 4,4,6,4,6,4,4,0, 4,4,4,6,4,4,4,0, // 2 kuadrantea
    0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,6, 0,4,4,4,4,4,6,4, 0,4,4,4,4,4,6,4, 0,0,4,4,4,4,6,4, 0,0,0,0,0,4,6,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 6,4,4,4,4,4,4,0, 6,4,4,4,4,4,4,0, 6,4,4,4,4,4,0,0, 6,4,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza horizontalki egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,4,6, 0,0,4,4,4,4,4,6, 0,4,4,4,4,4,4,6, 0,4,4,4,4,4,4,6, 0,4,4,4,4,4,4,4, 0,4,4,4,4,4,4,4, 0,4,4,6,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,6,4,0,0,0,0,0, 4,6,4,4,4,4,0,0, 4,6,4,4,4,4,4,0, 4,6,4,4,4,4,4,0, 6,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, 4,4,4,4,4,4,4,0, // 2 kuadrantea
    0,4,6,4,6,4,4,4, 0,4,6,4,6,4,4,4, 0,4,6,4,6,4,4,4, 0,4,6,4,6,4,0,0, 0,4,6,4,6,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,6,4,0, 4,4,4,4,6,4,6,0, 4,4,4,4,6,4,6,0, 0,0,4,4,6,4,6,0, 0,0,0,4,6,4,6,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza biraketa ipar-eki egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,4,4,4,4,4,4, 0,0,6,6,6,6,4,4, 0,0,4,4,4,4,6,4, 0,0,6,6,6,6,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,6,0,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,6,6,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,6, 0,0,4,4,4,4,4,6, 0,0,0,4,4,4,4,6, 0,0,0,0,4,4,4,6, 0,0,0,0,0,4,4,6, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    6,4,4,4,4,6,0,0, 4,6,4,4,4,4,0,0, 4,6,4,4,4,4,0,0, 4,6,4,4,4,4,0,0, 4,6,4,4,4,4,0,0, 4,6,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza biraketa ipar-eki egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,4,4,4,4,4,4, 0,0,6,6,6,6,6,4, 0,0,4,4,4,4,4,6, 0,0,6,6,6,6,6,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,6,6,0,0,0,0, 4,6,6,4,4,0,0,0, 4,4,6,6,6,6,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,6,6,6,6,6,4, 0,0,4,4,4,4,4,6, 0,0,0,6,6,6,6,4, 0,0,0,0,4,4,4,4, 0,0,0,0,0,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,0,0, 4,4,4,4,4,6,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,6,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza biraketa eki-hego egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,6,6,6,6,6,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,6,4,6,4,0,0, 4,4,6,4,6,4,0,0, 4,4,6,4,6,4,0,0, 4,4,6,4,6,4,0,0, 4,4,4,6,4,4,0,0, 4,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,6, 0,0,6,6,6,6,6,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,6, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 6,6,6,6,6,0,0,0, 6,4,4,4,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza biraketa eki-hego egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,6,6,6,6,4, 0,0,4,4,4,4,4,6, 0,0,6,6,6,6,6,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,6,4,0,0, 4,4,4,4,4,6,0,0, 4,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,6,6,6,6,6,4, 0,0,4,4,4,4,4,6, 0,0,6,6,6,6,6,4, 0,0,4,4,4,4,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,6,6,6,6,0,0, 4,6,6,4,4,0,0,0, 4,4,6,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza biraketa hego-mende egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,4,4,6,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,6,6,6,6,6,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,6,6,6,6,6, 0,0,0,0,4,4,4,6, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    6,4,4,4,4,4,0,0, 4,6,6,6,6,6,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 6,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza biraketa hego-mende egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,4,6,4,4,4,4, 0,0,4,6,4,4,4,4, 0,0,4,6,4,4,4,4, 0,0,4,6,4,4,4,4, 0,0,6,4,4,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,4,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,6,6,6,6,0,0,0, 6,4,4,4,4,4,0,0, 4,6,6,6,6,6,0,0, 4,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,6,6,6,6,4,4, 0,0,0,4,4,6,6,4, 0,0,0,0,6,6,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,6,6,6,6,6,0,0, 6,4,4,4,4,4,0,0, 4,6,6,6,6,6,0,0, 4,4,4,4,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza biraketa mende-ipar egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,4,4,4,6, 0,0,0,6,6,6,6,6, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 6,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,6,6,6,6,6,0,0, 6,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,4,4,4,4, 0,0,4,4,6,4,4,4, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,6,6,6,6,6,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge gorputza biraketa mende-ipar egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,6,4,4,4, 0,0,0,4,6,4,4,4, 0,0,6,4,6,4,4,4, 0,0,6,6,6,4,4,4, 0,0,4,6,4,4,4,4, 0,0,4,4,4,4,4,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,6,4,4,4,4,0,0, 4,4,6,6,6,6,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, // 2 kuadrantea
    0,0,4,4,6,4,4,4, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,4,6,4,6,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,6,4,4,4,0,0, 4,6,4,6,4,4,0,0, 4,6,4,6,4,4,0,0, 4,6,4,6,4,0,0,0, 4,6,4,6,0,0,0,0, 4,6,4,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 // 4 kuadrantea
};

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
    snake->snakeBody[0].spriteGfxMem = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
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
static void animateSnakeBody(Snake *snake)
{
    int frame = snake->snakeBody[0].animFrame + snake->snakeBody[0].state 
                * BODY_FRAMES_PER_ANIMATION;
    int offset = frame * SNAKE_DIMENSION * SNAKE_DIMENSION;

    int i;
    for (i = 0; i < SNAKE_DIMENSION * SNAKE_DIMENSION / 2; i++)
        snake->snakeBody[0].spriteGfxMem[i] = snake->snakeBody[0].frameGfx[offset + i*2]
                                              | (snake->snakeBody[0].frameGfx[offset + i*2 + 1] << 8);
}

static u8 spriteSnakeTail[2048] =
{
    // Suge isatsa gorantz egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,4,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,0,0,4,4,4,6, 0,0,0,0,4,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,0,0,6,6,6, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,4,0,0,0, 6,6,6,6,6,6,0,0, 4,4,4,4,4,0,0,0, 6,6,6,6,6,6,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 6,4,4,4,4,0,0,0, // 2 kuadrantea
    0,0,0,0,0,4,4,4, 0,0,0,0,0,6,6,6, 0,0,0,0,4,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,0,0,4,4,4, 0,0,0,0,0,0,4,4, 0,0,0,0,0,0,0,4, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,6,4,4,4,0,0,0, 6,4,4,4,4,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,0,0,0,0,0, 4,4,0,0,0,0,0,0, 4,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge isatsa gorantz egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,4,4,4,4,4, 0,0,0,6,6,6,6,4, 0,0,0,4,4,4,4,6, 0,0,6,6,6,6,6,4, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,0,4,4,4,4,6, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,0,0,0,0, 6,6,6,0,0,0,0,0, // 2 kuadrantea
    0,0,0,4,4,4,6,4, 0,0,0,4,4,4,4,6, 0,0,0,0,4,4,4,4, 0,0,0,0,0,4,4,4, 0,0,0,0,0,0,4,4, 0,0,0,0,0,0,0,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,0,0,0,0,0, 6,6,6,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,0,0,0,0,0, 4,4,0,0,0,0,0,0, 4,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge isatsa eskubirantz egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,4,4,0,0, 0,0,0,4,4,4,6,4, 0,0,4,4,4,4,6,4, 0,4,4,4,4,4,6,4, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,4,4,0, 0,4,4,4,4,4,4,0, 6,4,4,4,4,4,4,0, 6,4,4,4,4,4,4,0, 6,4,4,4,4,4,4,0, // 2 kuadrantea
    0,0,4,4,4,4,6,4, 0,0,0,4,4,4,4,6, 0,0,0,0,4,4,4,4, 0,0,0,0,0,4,4,4, 0,0,0,0,0,0,4,4, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    6,4,4,4,6,4,4,0, 4,4,4,6,4,6,4,0, 4,4,4,6,4,6,4,0, 4,4,4,6,4,6,4,0, 4,4,4,6,4,6,4,0, 0,4,4,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge isatsa eskubirantz egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,4,4, 0,0,4,4,4,4,6,4, 0,4,4,4,4,6,4,6, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,6,0,0,0,0,0, 4,4,6,4,6,4,0,0, 4,4,6,4,6,4,0,0, 4,4,6,4,6,4,0,0, 4,4,6,4,6,4,0,0, 4,4,4,6,4,4,0,0, // 2 kuadrantea
    4,4,4,4,4,6,4,6, 0,4,4,4,4,6,4,6, 0,0,4,4,4,6,4,6, 0,0,0,4,4,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 0,0,0,0,4,4,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea

    // Suge isatsa beherantz egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,4, 0,0,0,0,0,0,4,4, 0,0,0,0,0,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,0,0,6,6,6, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,0,0,0,0,0,0,0, 4,4,0,0,0,0,0,0, 4,4,4,0,0,0,0,0, 4,4,4,4,0,0,0,0, 6,4,4,4,4,0,0,0, // 2 kuadrantea
    0,0,0,0,0,4,4,4, 0,0,0,0,0,6,6,6, 0,0,0,0,4,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,0,4,4,4,6, 0,0,0,4,4,4,4,4, 0,0,0,4,4,4,4,4, // 3 kuadrantea
    4,6,4,4,4,0,0,0, 6,4,4,4,4,0,0,0, 4,4,4,4,4,4,0,0, 4,4,4,4,4,4,0,0, 6,6,6,6,6,6,0,0, 4,4,4,4,4,0,0,0, 6,6,6,6,6,0,0,0, 4,4,4,4,4,0,0,0, // 4 kuadrantea

    // Suge isatsa beherantz egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,4, 0,0,0,0,0,0,4,4, 0,0,0,0,0,4,4,4, 0,0,0,0,4,4,4,4, 0,0,0,4,4,4,4,6, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,0,0,0,0,0,0,0, 4,4,0,0,0,0,0,0, 4,4,4,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,0,0,0,0, 6,6,6,0,0,0,0,0, // 2 kuadrantea
    0,0,0,4,4,4,6,4, 0,0,0,4,4,4,4,6, 0,0,4,4,4,4,4,4, 0,0,4,4,4,4,4,4, 0,0,6,6,6,6,6,4, 0,0,0,4,4,4,4,6, 0,0,0,6,6,6,6,4, 0,0,0,4,4,4,4,4, // 3 kuadrantea
    4,4,4,0,0,0,0,0, 6,6,6,0,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,0,0,0, 4,4,4,4,4,0,0,0, // 4 kuadrantea

    // Suge isatsa ezkerrerantz egoera 1
    0,0,0,0,0,0,0,0, 0,0,0,0,6,4,4,0, 0,4,6,4,6,4,4,4, 0,4,6,4,6,4,4,4, 0,4,6,4,6,4,4,4, 0,4,6,4,6,4,4,4, 0,4,6,4,6,4,4,4, 0,4,4,6,4,4,4,6, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 4,4,0,0,0,0,0,0, 4,4,4,0,0,0,0,0, 4,4,4,4,0,0,0,0, 6,4,4,4,4,0,0,0, 4,6,4,4,4,4,0,0, // 2 kuadrantea
    0,4,4,4,4,4,4,6, 0,4,4,4,4,4,4,6, 0,4,4,4,4,4,4,6, 0,4,4,4,4,4,4,0, 0,4,4,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,6,4,4,4,4,4,0, 4,6,4,4,4,4,0,0, 4,6,4,4,4,0,0,0, 0,0,4,4,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 4 kuadrantea
    
    // Suge isatsa ezkerrerantz egoera 2
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,4,4,0,0,0,0,0, 0,4,4,4,4,4,4,0, 0,4,4,4,4,4,4,6, 0,4,4,4,4,4,4,6, 0,4,4,4,4,4,4,6, // 1 kuadrantea
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,4,4,0,0,0,0, 4,6,4,4,4,0,0,0, 4,6,4,4,4,4,0,0, 4,6,4,4,4,4,4,0, // 2 kuadrantea
    0,4,4,6,4,4,4,6, 0,4,6,4,6,4,4,4, 0,4,6,4,6,4,4,4, 0,4,6,4,6,4,4,4, 0,4,6,4,6,4,4,4, 0,0,0,0,6,4,4,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    4,6,4,4,4,4,0,0, 6,4,4,4,4,0,0,0, 4,4,4,4,0,0,0,0, 4,4,4,0,0,0,0,0, 4,4,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 // 4 kuadrantea
};

/*!
 * \brief Sugea buruaren spritearentzako memoria alokatzen da, eta alokatutako
 * helbidea parametroz jasotako snake-aren erakuslearen buruaren spriteGfxMem
 * aldagaian gordetzen da. Horrez gain, spriteak dituen tile-en informazioaren
 * erakuslea gordetzen da buruko frameGfx-en.
 * 
 * \param snake suge egituraren erakuslea
 * \param gfx sugea isatsaren tile-en erakuslea
 */
static void initSnakeTail(Snake *snake, u8 *gfx)
{
    snake->snakeTail.spriteGfxMem = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
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
static void animateSnakeTail(Snake *snake)
{
    int frame = snake->snakeTail.animFrame + snake->snakeTail.state
                * TAIL_FRAMES_PER_ANIMATION;
	int offset = frame * SNAKE_DIMENSION * SNAKE_DIMENSION;

    int i;
    for (i = 0; i < SNAKE_DIMENSION * SNAKE_DIMENSION / 2; i++)
        snake->snakeTail.spriteGfxMem[i] = snake->snakeTail.frameGfx[offset + i*2]
                                           | (snake->snakeTail.frameGfx[offset + i*2 + 1] << 8);
}

/*!
 * \brief Suge osoaren egoera eta animazioaren framearekin momentuko sub-spritearen
 * indizea kalkulatzen du atal bakoitzarentzat. Indize honen bitartez memorian
 * gordeta dagoen tile-en erakusletik nahi dugun frame-aren helbidea lortzen da,
 * eta azkenik bertako framea memoriatik spritea irakurtzen den lekuan kopiatzen
 * da spritea eguneratu ahal izateko.
 *
 * \param snake suge egituraren erakuslea
 */
void animateSnake(Snake *snake)
{
    /* Suge buruak izan behar duen frame berria lortzen du eta spritea gordetzen
    den memoriako helbidean hori ezartzen du. */
    animateSnakeHead(snake);

    /* Suge gorputzak izan behar duen frame berria lortzen du eta spritea gordetzen
    den memoriako helbidean hori ezartzen du. */
	animateSnakeBody(snake);

    /* Suge isatsak izan behar duen frame berria lortzen du eta spritea gordetzen
    den memoriako helbidean hori ezartzen du. */
	animateSnakeTail(snake);
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
           SpriteColorFormat_256Color, // kolore formatua
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
           SpriteColorFormat_256Color, // kolore formatua
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
           SpriteColorFormat_256Color, // kolore formatua
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
           SpriteColorFormat_256Color, // kolore formatua
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
           SpriteColorFormat_256Color, // kolore formatua
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
           SpriteColorFormat_256Color, // kolore formatua
           snake->snakeTail.spriteGfxMem, // spritea agertzen den helbidea memorian
           -1, // indize afinea
           false, // indize afinea bada tamaina bikoiztu errotazioan
           true, // spritea ezkutatu nahi den, ezin da afinea izan
           false, // bertikalki bolkatu spritea
           false, // horizontalki bolkatu spritea
           false); // mosaicoa aplikatu nahi den
}

static u8 spriteApple[256] =
{
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,7,7,7,4, 0,0,0,7,7,7,7,7, 0,0,7,7,7,7,7,7, 0,7,7,7,7,7,7,7, 0,7,7,7,7,7,7,7, 0,7,7,7,7,7,7,7, // 1 kuadrantea
    0,4,0,0,0,0,0,0, 4,0,0,0,0,0,0,0, 7,7,7,7,0,0,0,0, 7,7,7,7,7,0,0,0, 7,7,7,7,7,7,0,0, 7,7,7,7,3,7,7,0, 7,7,7,7,3,3,7,0, 7,7,7,7,7,7,7,0, // 2 kuadrantea
    0,7,7,7,7,7,7,7, 0,7,7,7,7,7,7,7, 0,7,7,7,7,7,7,7, 0,0,7,7,7,7,7,7, 0,0,0,7,7,7,7,7, 0,0,0,0,7,7,7,7, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // 3 kuadrantea
    7,7,7,7,7,7,7,0, 7,7,7,7,7,7,7,0, 7,7,7,7,7,7,7,0, 7,7,7,7,7,7,0,0, 7,7,7,7,7,0,0,0, 7,7,7,7,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 // 4 kuadrantea
};

/*!
 * \brief Sagar spritearentzako memoria alokatzen da, eta alokatutako helbidea
 * parametroz jasotako sagarraren erakuslearen spriteGfxMem aldagaian gordetzen
 * da.
 * 
 * \param apple sagar egituraren erakuslea
 * \param gfx sagar tile-en erakuslea
 */
static void initApple(Apple *apple, u8 *gfx)
{
    apple->spriteGfxMem = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    apple->spriteId = 3;

    int i;
    for (i = 0; i < APPLE_DIMENSION * APPLE_DIMENSION / 2; i++)
        apple->spriteGfxMem[i] = gfx[i*2]
                                 | (gfx[i*2 + 1] << 8);
}

/*!
 * \brief OAM sarrera zehazten du sagarra ikusarazteko zehaztutako balioekin.
 *
 * \param apple sagar egitura duen erakuslea
 */
void displayApple(Apple *apple)
{
    oamSet(&oamMain, // oam
           apple->spriteId, // id
           apple->x, // x
           apple->y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_16x16, // tamaina 
           SpriteColorFormat_256Color, // kolore formatua
           apple->spriteGfxMem, // spritea agertzen den helbidea memorian
           -1, // indize afinea
           false, // indize afinea bada tamaina bikoiztu errotazioan
           false, // spritea ezkutatu nahi den, ezin da afinea izan
           false, // bertikalki bolkatu spritea
           false, // horizontalki bolkatu spritea
           false); // mosaicoa aplikatu nahi den
}

/*!
 * \brief OAM sarrera zehazten du sagarra ezkutatzeko zehaztutako balioekin.
 *
 * \param apple sagar egitura duen erakuslea
 */
void hideApple(Apple *apple)
{
    oamSet(&oamMain, // oam
           apple->spriteId, // id
           apple->x, // x
           apple->y, // y
           1, // lehentasuna
           0, // paleta alfa
           SpriteSize_16x16, // tamaina
           SpriteColorFormat_256Color, // kolore formatua
           apple->spriteGfxMem, // spritea agertzen den helbidea memorian
           -1, // indize afinea
           false, // indize afinea bada tamaina bikoiztu errotazioan
           true, // spritea ezkutatu nahi den, ezin da afinea izan
           false, // bertikalki bolkatu spritea
           false, // horizontalki bolkatu spritea
           false); // mosaicoa aplikatu nahi den
}

/*!
 * \brief 2D sprite motoreak hasieratzen ditu bi pantailetan tiledun eta bitmapdun
 * spriteak ageri daitezen. Baita ere, sugearentzat eta sagarrarentzat memoria
 * alokatzen du.
 */
void initSprites()
{
    setMainPallete();

    oamInit(&oamMain, SpriteMapping_1D_128, false);   // Jadanik oamUpdate(&oamMain) exekutatzen du
    oamInit(&oamSub, SpriteMapping_1D_32, false);    // Jadanik oamUpdate(&oamSub) exekutatzen du

    initSnakeHead(&snake, spriteSnakeHead);
    initSnakeBody(&snake, spriteSnakeBody);
    initSnakeTail(&snake, spriteSnakeTail);
    initApple(&apple, spriteApple);
}