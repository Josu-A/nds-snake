#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "definitions.h"
#include "graphics.h"
#include "sprites.h"
#include "jokoa01.h"

int main(void)
{
	hasieratuGrafikoakSpriteak();
	allowAllInterrupts();

	jokoa01();

	return(0);
}