#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "backgrounds.h"
#include "definitions.h"
#include "graphics.h"

#include "titleTopScreen.h"
#include "bgGamemodeSelect.h"
#include "bgGamemodeSelectModes.h"

/*!
 * \brief Goiko pantailan jokoaren banner nagusia kopiatzen du bigarren pantailako
 * 3 fondoaren helbidera, eta 8 bmp-eko irudiaren kolore paleta ezartzen du bigarren
 * pantailan.
 */
void showTitleTopScreen()
{
    dmaCopyHalfWords(DMA_CHANNEL,
                     titleTopScreenBitmap, /* Automatikoki sortzen den aldagaia */
                     bgGetGfxPtr(bg3Sub), /* Bigarren mailako pantailaren 3 fondoaren helbidea */
                     titleTopScreenBitmapLen); /* Luzera (bytetan) automatikoki sortzen den aldagaia */
    dmaCopyHalfWords(DMA_CHANNEL,
                     titleTopScreenPal, /* Automatikoki sortzen den aldagaia */
                     BG_PALETTE_SUB, /* Bigarren mailako pantailaren paletaren helbidea */
                     titleTopScreenPalLen); /* Luzera (bytetan) automatikoki sortzen den aldagaia */
}

/*!
 * \brief Goiko pantailan joko motak aukeratzerakoan agertzen den titulua kopiatzen
 * du bigarren pantailako 3 fondoaren helbidera, 8 bmp-eko kolore paleta izanda.
 */
void showBgGamemodeSelect()
{
    dmaCopyHalfWords(DMA_CHANNEL,
                     bgGamemodeSelectBitmap,
                     bgGetGfxPtr(bg3Sub),
                     bgGamemodeSelectBitmapLen);
    dmaCopyHalfWords(DMA_CHANNEL,
                     bgGamemodeSelectPal,
                     BG_PALETTE_SUB,
                     bgGamemodeSelectPalLen);
}

/*!
 * \brief Beheko pantailan joko motak aukeratzeko fondoa kopiatzen du pantaila
 * nagusiko 3 fondoaren helbidera, 16bit kolore mota erabiliz.
 */ 
void showBgGamemodeSelectModes()
{
	dmaCopyHalfWords(DMA_CHANNEL,
                     bgGamemodeSelectModesBitmap, /* Automatikoki sortzen den aldagaia */
                     (uint16 *)BG_BMP_RAM(8), /* Fondo nagusiaren helbidea */
                     bgGamemodeSelectModesBitmapLen); /* Luzera (bytetan) automatikoki sortzen den aldagaia */
}