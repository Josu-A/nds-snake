#ifndef _BACKGROUNDS_H_
#define _BACKGROUNDS_H_

/*!
 * \brief Goiko pantailan jokoaren banner nagusia kopiatzen du bigarren pantailako
 * 3 fondoaren helbidera, eta 8 bmp-eko irudiaren kolore paleta ezartzen du bigarren
 * pantailan.
 */
extern void showSubBgGameTitleScreen();

/*!
 * \brief Goiko pantailan joko motak aukeratzerakoan agertzen den titulua kopiatzen
 * du bigarren pantailako 3 fondoaren helbidera, 8 bmp-eko kolore paleta izanda.
 */
extern void showSubBgGamemodeSelect();

/*!
 * \brief Beheko pantailan joko motak aukeratzeko fondoa kopiatzen du pantaila
 * nagusiko 3 fondoaren helbidera, 16bit kolore mota erabiliz.
 */ 
extern void showMainBgGamemodeSelectModes();

/*!
 * \brief Beheko pantailan jokatzerakoan agertzen den fondoa kopiatzen du pantaila
 * nagusiko 3 fondoaren helbidera, 16 bit kolore mota erabiliz.
 */ 
extern void showMainBgGameplay();

/*!
 * \brief Goiko pantailan jokatze bitartean agertzen den fondoa kopiatzen du
 * bigarren pantailako 3 fondoaren helbidera, 8 bmp-eko kolore paleta izanda.
 */
extern void showSubBgGameplayInfo();

#endif