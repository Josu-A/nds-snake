#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "backgrounds.h"
#include "definitions.h"
#include "graphics.h"

#include "Atea.h"
#include "AteaIrekita.h"
#include "titleTopScreen.h"

void erakutsiAtea() {
	
	dmaCopyHalfWords(DMA_CHANNEL,
                     AteaBitmap, /* Automatikoki sortzen den aldagaia */
                     (uint16 *)BG_BMP_RAM(0), /* Fondo nagusiaren helbidea */
                     AteaBitmapLen); /* Luzera (bytetan) automatikoki sortzen den aldagaia */
}

void erakutsiAteaIrekita() {
		
    dmaCopyHalfWords(DMA_CHANNEL,
                     AteaIrekitaBitmap, /* Automatikoki sortzen den aldagaia */
                     (uint16 *)BG_BMP_RAM(0), /* Fondo nagusiaren helbidea */
                     AteaIrekitaBitmapLen); /* Luzera (bytetan) automatikoki sortzen den aldagaia */
}

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