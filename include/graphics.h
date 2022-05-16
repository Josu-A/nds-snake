#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

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
extern void hasieratuGrafikoakSpriteak();

/*!
 * \brief Bigarren pantailako 3 fondoaren NDS-arentzako indizea gordetzen du.
 * 0etik 7ra.
 */
extern int bg3Sub;

#endif