#include <nds.h> 		//nds-rako garatuta dagoen liburutegia
#include <stdio.h>		//c-ko liburutegi estandarra sarrera eta irteerako funtzioak definitzen dituena
#include <stdlib.h>		//c-ko liburutegi estandarra memoria erreserbak eta zenbaki konbertsioak egiteko
#include <unistd.h>		//Sistema eragileen arteko konpatibilitatea ziurtatzeko liburutegia

#include "definitions.h"
#include "graphics.h"
#include "backgrounds.h"
#include "sprites.h"

/**
 * Aldagai globalak deklaratzen dira.
 */
PrintConsole topScreenConsole;
PrintConsole bottomScreenConsole;
int bg3Sub;

/**
 * VRAM-a mapeatzen du bi pantailetan irudiak erakutsi ahal izateko.
 * 
 * A eta B bankuak pantaila nagusiko fondoko memoriara mapeatzen du. C bankua
 * bigarren pantailako fondoko memoriara. D bankua bigarren pantailako sprite0
 * memoriara mapeatzen du. E bankua pantaila nagusiko sprite0 erdirainoko memoriara
 * mapeatzen du.
 * 
 * 2D motore nagusiko bideo mota 5. motara ezartzen du. Honek, fondoko zerogarren
 * geruzan Text/3D fondoa erabiliko du; lehenengo geruzan, Text fondoa; bigarren
 * geruzan Extended fondoa; eta hirugarren geruzan, Extended fondoa. Horrez gain,
 * bigarren eta hirugarren fondoak aktibatzen ditu.
 * 
 * Bigarren 2D motoreko bideo mota 5. motara ezartzen du. Honek, fondoko zerogarren
 * geruzan Text fondoa erabiliko du; lehenengo geruzan, Text fondoa; bigarren geruzan
 * Extended fondoa; eta hirugarren geruzan, Extended fondoa. Horrez gain, hirugarren
 * fondoa aktibatzen da.
 */
void initVideo() {
    vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06020000,
                        VRAM_B_MAIN_BG_0x06040000,
                        VRAM_C_SUB_BG_0x06200000,
                        VRAM_D_SUB_SPRITE);
    vramSetBankE(VRAM_E_MAIN_SPRITE);
    vramSetBankF(VRAM_F_MAIN_BG_0x06000000);

    /*  Pantaila nagusirako bideo modua ezarri */
    videoSetMode(MODE_5_2D | // 5. bideo modua ezarri
                 DISPLAY_BG1_ACTIVE | // 1. fondoa pantailaratzeko aktibatu
                 DISPLAY_BG2_ACTIVE | // 2. fondoa pantailaratzeko aktibatu
                 DISPLAY_BG3_ACTIVE | // 3. fondoa pantailaratzeko aktibatu
                 DISPLAY_SPR_ACTIVE | // Spriteak pantailaratzeko aktibatu 
                 DISPLAY_SPR_1D); // 1D tile sprite-ak aktibatu

    /*  2. mailako pantaila bideo modua ezarri */
    videoSetModeSub(MODE_3_2D | // 3. bideo modua ezarri
                    DISPLAY_BG2_ACTIVE | // 2. fondoa aktibatu
                    DISPLAY_BG3_ACTIVE | // 3. fondoa aktibatu
                    DISPLAY_SPR_ACTIVE); // Spriteak pantailaratzeko aktibatu
}

/**
 * Fondo sistema konfiguratzen du.
 * 
 * Pantaila nagusiko 3 fondoari:
 * 
 * - Afinitatea ezartzen du 16 biteko 256x256 bitmap-arentzako.
 * - Memoriako helbidea ezarri.
 * - Geruzaren lehentasunari balio baxuena ezarri.
 * - Transformazio matrizeari identitate matrizea esleitu.
 * - Pantailako hasierako posizioa (0, 0)-n ezarri, goi-ezkerrean.
 * 
 * Pantaila nagusiko 2 fondoari:
 * 
 * - Afinitatea ezartzen du 16 biteko 128x128 bitmap-arentzako.
 * - Memoriako helbidea ezarri.
 * - Geruzaren lehentasunari 3 fondoarena baino lehentasun altuagoa ezarri.
 * - Transformazio matrizeari identitate matrizea esleitu.
 * - Pantailako hasierako posizioa.
 * 
 * Bigarren mailako pantailako 3 fondoari:
 * 
 * - Afinitatea ezartzen du 8 biteko 256x256 bitmap-arentzako.
 * - Memoriako helbidea ezarri.
 * - Geruzaren lehentasunari balio baxuena ezarri.
 */
void initBackgrounds() {
    /**
     * Pantaila nagusiko 3 fondoaren afinitatea ezarri 16 biteko koloretarako.
     */
    REG_BG3CNT = BG_BMP16_256x256 |
                 BG_BMP_BASE(1) | // Memoriako hasierako helbidea
                 BG_PRIORITY(BG_PRIORITY_3); // Lehentasun baxua

    /**
     * Pantaila nagusiko 3 fondoaren transformazio matrizeari identitate matrizea
     * esleitu.
     */
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;

    /**
     * Pantaila nagusiko 3 fondoaren posizo egoera ezartzen du, (0, 0) goi-ezkerreko
     * puntua izanez.
     */
    REG_BG3X = 0;
    REG_BG3Y = 0;

    /**
     * Pantaila nagusiko 2 fondoaren afinitatea ezarri 16 biteko koloretarako.
     */
    REG_BG2CNT = BG_BMP16_128x128 |
                 BG_BMP_BASE(9) | // Memoriako hasierako helbidea
                 BG_PRIORITY(BG_PRIORITY_2);  // Lehentasun baxua

    /**
     * Pantaila nagusiko 2 fondoaren transformazio matrizeari identitate matrizea esleitu.
     */
    REG_BG2PA = 1 << 8;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 1 << 8;

    /**
     * Pantaila nagusiko 2 fondoaren posizo egoera ezartzen du, (0, 0) goi-ezkerreko
     * puntua izanez.
     */
    REG_BG2X = -(SCREEN_WIDTH / 2 - 32) << 8;
    REG_BG2Y = -32 << 8;

    
    /**
     *  Bigarren mailako pantailako 3 fondoaren afinitatea ezarri 16 biteko koloretarako. 
     */
    bg3Sub = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 1, 0);
    REG_BG3CNT_SUB |= BG_PRIORITY(BG_PRIORITY_3); // Lehentasun baxua
}

void initSprites()
{
    oamInit(&oamMain, SpriteMapping_1D_128, false);   // Jadanik oamUpdate(&oamMain) exekutatzen du
    oamInit(&oamSub, SpriteMapping_1D_128, false);    // Jadanik oamUpdate(&oamSub) exekutatzen du

    configureSnake(&snake);
}

/**
 * Grafikoak eta spriteak hasieratzen ditu. Horretarako:
 * 
 * - 2D grafiko motorea pizten du.
 * - Pantaila nagusia beheko pantaila fisikoan ezartzen du.
 * - VRAM-a mapeatzen du bi pantailetan irudiak erakutsi ahal izateko.
 * - Fondo sistema konfiguratzen du.
 * - Hasiera pantailako titulua memorian txertatzen du.
 * - Spriteak hasieratzen ditu.
 */
void hasieratuGrafikoakSpriteak()
{
    /** 2D grafiko motorea pizten du. */
	powerOn(POWER_ALL_2D);

    /** Pantaila nagusia beheko pantaila fisikoan ezartzen du. */
    lcdMainOnBottom();
    /** VRAM-a mapeatzen du bi pantailetan irudiak erakutsi ahal izateko. */
    initVideo();

    /** Fondo sistema konfiguratzen du. */
    initBackgrounds();
    showTitleTopScreen();

    /** Spriteak hasieratu. */
    initSprites();

    consoleInit(&topScreenConsole, 2, BgType_Text4bpp, BgSize_T_256x256, 2, 0, false, true);
    consoleInit(&bottomScreenConsole, 1, BgType_Text4bpp, BgSize_T_256x256, 2, 0, true, true);
}
