#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "definitions.h"
#include "graphics.h"
#include "backgrounds.h"
#include "sprites.h"

/*!
 * \brief Goiko pantailako (bigarren pantaila) kontsola kontrolatzeko datuak
 * gordetzen ditu.
 */
PrintConsole topScreenConsole;

/*!
 * \brief Beheko pantailako (pantaila nagusia) kontsola kontrolatzeko datuak
 * gordetzen ditu.
 */
PrintConsole bottomScreenConsole;

/*!
 * \brief Bigarren pantailako 3 fondoaren NDS-arentzako indizea gordetzen du.
 * 0etik 7ra.
 */
int bg3Sub;

/*!
 * VRAM-a mapeatzen du bi pantailetan irudiak erakutsi ahal izateko.
 * 
 * A bankua pantaila nagusiko fondoa erakusteko hasieratzen da. Bankuak 
 * 0x06020000 - 0x0603FFFF memoria tartea hartuko du, 128 KB guztira.
 * Bertan hasieratuko da:
 * 
 * - 16 bit koloreko 256*256 bitmap fondoa 3 fondoan, 128 KB guztira: map-arentzat
 * 8ko oinarriarekin (0x06020000 - 0x0603FFFF memoria tartea hartu).
 * 
 * B bankua ez da erabiltzen.
 * 
 * C bankua bigarren pantailako fondoak erakusteko hasieratzen da. Bankuak 
 * 0x06200000 - 0x0621FFFF memoria tartea hartuko du, 128 KB guztira. 
 * Bertan hasieratuko dira:
 * 
 * - Kontsola bat 2 fondoan, 6 KB guztira: map-arentzat 2ko oinarriarekin
 * (0x06201000 memoria hartu) eta tile-entzako 0ko oinarriarekin
 * (0x06200000 - 0x06200800 memoria tartea hartu).
 * 
 * - 8 bit koloreko 256*256 bitmap fondoa 3 fondoan, 64 KB guztira: map-arentzat
 * 1eko oinarriarekin (0x06204000 - 0x06213800 memoria tartea hartu).
 * 
 * D bankua ez da erabiltzen.
 * 
 * E bankua pantaila nagusiko spriteak erakusteko hasieratzen da. Bankuak
 * 0x06400000 - 0x0640FFFF memoria tartea hartuko du, 64 KB guztira. Bertan
 * 256 koloreko paleta duten spriteak egongo dira.
 * 
 * F bankua pantaila nagusiko kontsola erakusteko hasieratzen da. Bankuak
 * 0x06000000 - 0x06003FFF memoria tartea hartuko du, 16 KB guztira. Bertan
 * hasieratuko da:
 * 
 * - Kontsola bat 1 fondoan, 6 KB guztira: map-arentzat 2ko oinarriarekin
 * (0x06001000 memoria hartu) eta tile-entzako 0ko oinarriarekin
 * (0x06000000 - 0x06000800 memoria tartea hartu).
 * 
 * 2D motore nagusiko bideo mota 5. motara ezartzen du. Honek, fondoko zerogarren
 * geruzan Text/3D fondoa erabiliko du; lehenengo geruzan, Text fondoa; bigarren
 * geruzan Extended fondoa; eta hirugarren geruzan, Extended fondoa. Horrez gain,
 * lehenengo eta hirugarren fondoak aktibatzen ditu, baita ere spriteak.
 * 
 * Bigarren 2D motoreko bideo mota 5. motara ezartzen du. Honek, fondoko zerogarren
 * geruzan Text fondoa erabiliko du; lehenengo geruzan, Text fondoa; bigarren geruzan
 * Extended fondoa; eta hirugarren geruzan, Extended fondoa. Horrez gain, bigarren
 * eta hirugarren fondoa aktibatzen da.
 */
static void initVideo() {
    vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06020000,
                        VRAM_B_LCD,
                        VRAM_C_SUB_BG_0x06200000,
                        VRAM_D_LCD);
    vramSetBankE(VRAM_E_MAIN_SPRITE);
    vramSetBankF(VRAM_F_MAIN_BG_0x06000000);

    /*  Pantaila nagusirako bideo modua ezarri */
    videoSetMode(MODE_5_2D | // 5. bideo modua ezarri
                 DISPLAY_BG1_ACTIVE | // 1. fondoa pantailaratzeko aktibatu, kontsolarentzat
                 DISPLAY_BG3_ACTIVE | // 3. fondoa pantailaratzeko aktibatu, fondoarentzat
                 DISPLAY_SPR_ACTIVE | // Spriteak pantailaratzeko aktibatu
                 DISPLAY_SPR_1D); // 1D tile sprite-ak aktibatu

    /*  2. mailako pantaila bideo modua ezarri */
    videoSetModeSub(MODE_3_2D | // 3. bideo modua ezarri
                    DISPLAY_BG2_ACTIVE | // 2. fondoa aktibatu, kontsolarentzat
                    DISPLAY_BG3_ACTIVE); // 3. fondoa aktibatu, fondoarentzat
}

/*!
 * Fondo sistema konfiguratzen du.
 * 
 * Pantaila nagusiko 3 fondoari:
 * 
 * - Afinitatea ezartzen du 16 biteko koloredun 256x256 bitmap-arentzako.
 * - Memoriako helbidea ezarri.
 * - Geruzaren lehentasunari balio baxuena ezarri.
 * - Transformazio matrizeari identitate matrizea esleitu.
 * - Pantailako hasierako posizioa (0, 0)-n ezarri, goi-ezkerrean.
 * 
 * Bigarren mailako pantailako 3 fondoari:
 * 
 * - Afinitatea ezartzen du 8 biteko koloredun 256x256 bitmap-arentzako.
 * - Memoriako helbidea ezarri.
 * - Geruzaren lehentasunari balio baxuena ezarri.
 */
static void initBackgrounds() {
    // Pantaila nagusiko 3 fondoaren afinitatea ezarri 16 biteko koloretarako.
    REG_BG3CNT = BG_BMP16_256x256 |
                 BG_BMP_BASE(8) | // Memoriako hasierako helbidea
                 BG_PRIORITY(BG_PRIORITY_3); // Lehentasun baxuena ezarri
    /* Pantaila nagusiko 3 fondoaren transformazio matrizeari identitate matrizea
       esleitu. */
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;
    /* Pantaila nagusiko 3 fondoaren posizo egoera ezartzen du, (0, 0)
       goi-ezkerreko puntua izanez. */
    REG_BG3X = 0;
    REG_BG3Y = 0;
    
    /* Bigarren mailako pantailako 3 fondoa hasieratzen du 256x256 tamainako
       eta 8 biteko koloredunaren fondoentzat, eta bere indizea gordetzen du. */
    bg3Sub = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 1, 0);
    bgSetPriority(bg3Sub, BG_PRIORITY(BG_PRIORITY_3)); // Lehentasun baxuena ezarri
}

/*!
 * \brief Pantaila bakoitzean kontsola bana hasieratzen dira eta pantaila nagusiko
 * kontsolari 0 lehentasuna ezartzen zaio.
 */
static void initConsoles()
{
    /* Bigarren mailako pantailako kontsola (goian agerriko dena) hasieratu 2
       fondoan map-eko 2 oinarriarekin */
    consoleInit(&topScreenConsole, 2, BgType_Text4bpp, BgSize_T_256x256, 2, 0, false, true);
    /* Lehenengo mailako pantailako kontsola (behean agerriko dena) hasieratu 1
       fondoan map-eko 2 oinarriarekin */
    consoleInit(&bottomScreenConsole, 1, BgType_Text4bpp, BgSize_T_256x256, 2, 0, true, true);
    bgSetPriority(1, BG_PRIORITY(BG_PRIORITY_0)); // Lehen mailako pantailako kontsolari lehentasun altuena
}

/*!
 * Grafikoak eta spriteak hasieratzen ditu. Horretarako:
 * 
 * - 2D grafiko motorea pizten du.
 * - Pantaila nagusia beheko pantaila fisikoan ezartzen du.
 * - VRAM-a mapeatzen du bi pantailetan irudiak erakutsi ahal izateko.
 * - Fondo sistema konfiguratzen du.
 * - Spriteak hasieratzen ditu.
 * - Kontsolak hasieratzen ditu.
 */
void hasieratuGrafikoakSpriteak()
{
    // 2D grafiko motorea pizten du.
	powerOn(POWER_ALL_2D);

    // Pantaila nagusia beheko pantaila fisikoan ezartzen du.
    lcdMainOnBottom();
    // VRAM-a mapeatzen du bi pantailetan irudiak erakutsi ahal izateko.
    initVideo();
    // Fondo sistema konfiguratzen du.
    initBackgrounds();
    // Spriteak hasieratu.
    initSprites();
    // Bi pantailetan kontsola bana hasieratzen da informazioa adierazteko.
    initConsoles();
}
