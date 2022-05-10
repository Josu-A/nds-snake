#ifndef _SERVICERUTINES_H_
#define _SERVICERUTINES_H_

/*!
 * \brief Etenen bidez konfiguratutako teklatuek jautiko duten funtzio nagusia.
 */
extern void interruptKeys();

/*!
 * \brief 0 denboragailuak etenak sortutakoan deituko duen funtzioa.
 */
extern void interruptTimer0();

/*!
 * \brief Eten zerbitzu-errutinak zehazten ditu teklatuarentzako eta
 * denboragailuarentzako.
 */
extern void setInterruptionServiceRutines();

#endif