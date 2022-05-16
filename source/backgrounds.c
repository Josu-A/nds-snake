#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "backgrounds.h"
#include "definitions.h"
#include "graphics.h"

#include "titleTopScreen.h" // grit-ek sortzen du
#include "bgGamemodeSelect.h" // grit-ek sortzen du
#include "bgGamemodeSelectModes.h" // grit-ek sortzen du
#include "bgGameplay.h" // grit-ek sortzen du

/*!
 * \brief Goiko pantailan jokoaren banner nagusia kopiatzen du bigarren pantailako
 * 3 fondoaren helbidera, eta 8 bmp-eko irudiaren kolore paleta ezartzen du bigarren
 * pantailan.
 */
void showSubBgGameTitleScreen()
{
    // Spritea kopiatu memorian erakusteko
    dmaCopyHalfWords(DMA_CHANNEL, // Erabiltzeko DMA kanala
                     titleTopScreenBitmap, // Automatikoki sortzen den aldagaia
                     bgGetGfxPtr(bg3Sub), // Bigarren mailako pantailaren 3 fondoaren helbidea
                     titleTopScreenBitmapLen); // Luzera (bytetan) automatikoki sortzen den aldagaia
    
    /* Spritearen paleta kopiatu memorian spritearen kolore egokiak erakutsi
       ahal izateko. */
    dmaCopyHalfWords(DMA_CHANNEL, // Erabiltzeko DMA kanala
                     titleTopScreenPal, // Automatikoki sortzen den aldagaia
                     BG_PALETTE_SUB, // Bigarren mailako pantailaren fondoen paletaren helbidea
                     titleTopScreenPalLen); // Luzera (bytetan) automatikoki sortzen den aldagaia
}

/*!
 * \brief Goiko pantailan joko motak aukeratzerakoan agertzen den titulua kopiatzen
 * du bigarren pantailako 3 fondoaren helbidera, 8 bmp-eko kolore paleta izanda.
 */
void showSubBgGamemodeSelect()
{
    // Spritea kopiatu memorian erakusteko
    dmaCopyHalfWords(DMA_CHANNEL, // Erabiltzeko DMA kanala
                     bgGamemodeSelectBitmap, // Automatikoki sortzen den aldagaia
                     bgGetGfxPtr(bg3Sub), // Bigarren mailako pantailaren 3 fondoaren helbidea
                     bgGamemodeSelectBitmapLen); // Luzera (bytetan) automatikoki sortzen den aldagaia
    
    /* Spritearen paleta kopiatu memorian spritearen kolore egokiak erakutsi
       ahal izateko. */
    dmaCopyHalfWords(DMA_CHANNEL, // Erabiltzeko DMA kanala
                     bgGamemodeSelectPal, // Automatikoki sortzen den aldagaia
                     BG_PALETTE_SUB, // Bigarren mailako pantailaren fondoen paletaren helbidea
                     bgGamemodeSelectPalLen); // Luzera (bytetan) automatikoki sortzen den aldagaia
}

/*!
 * \brief Beheko pantailan joko motak aukeratzeko fondoa kopiatzen du pantaila
 * nagusiko 3 fondoaren helbidera, 16 bit kolore mota erabiliz.
 */ 
void showMainBgGamemodeSelectModes()
{
	dmaCopyHalfWords(DMA_CHANNEL, // Erabiltzeko DMA kanala
                     bgGamemodeSelectModesBitmap, // Automatikoki sortzen den aldagaia
                     (uint16 *)BG_BMP_RAM(8), // Pantaila nagusiko 3 fondoaren helbidea
                     bgGamemodeSelectModesBitmapLen); // Luzera (bytetan) automatikoki sortzen den aldagaia
}

/*!
 * \brief Beheko pantailan jokatzerakoan agertzen den fondoa kopiatzen du pantaila
 * nagusiko 3 fondoaren helbidera, 16 bit kolore mota erabiliz.
 */ 
void showMainBgGameplay()
{
	dmaCopyHalfWords(DMA_CHANNEL, // Erabiltzeko DMA kanala
                     bgGameplayBitmap, // Automatikoki sortzen den aldagaia
                     (uint16 *)BG_BMP_RAM(8), // Pantaila nagusiko 3 fondoaren helbidea
                     bgGameplayBitmapLen); // Luzera (bytetan) automatikoki sortzen den aldagaia
}