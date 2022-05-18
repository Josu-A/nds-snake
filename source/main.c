#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "definitions.h"
#include "graphics.h"
#include "sprites.h"
#include "jokoa01.h"
#include "peripherals.h"

/* Main funtzio nagusia */
int main(void)
{
	hasieratuGrafikoakSpriteak(); // Grafikoak eta spriteak hasieratu
	allowAllInterrupts(); // Etenak baimendu

	jokoa01(); // Jokoa zehazten duen funtzioa

	return(0);
}