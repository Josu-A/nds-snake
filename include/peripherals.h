#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

#include "definitions.h"

/*!
 * \brief Teklatuko botoiak sinkronizatzen ditu etenen bidez edo inkesta bidez
 *        indibidualki emandako TEK_konf aldagaiarekin.
 * 
 * TEK_konf parametroa 16 biteko int bat da eta bit bakoitzak tekla espezifiko 
 * bat etenen edo inkesta bidez konfiguratuta dagoela adierazten du. 1 balioa
 * etenen bidez badago, 0 balio inkesta bidez. Ikusi REG_KEYCNT definitions.h-en
 * tekla bitak ikusteko.
 * 
 * \param TEK_konf aldatu nahi diren teklak adierazten duen balioa
 */
extern void configureKeys(int TEK_konf);

/*!
 * \brief 0 Denboragailua hasieratzen du kontaketa zein baliotatik hastea
 *        nahi den Latch balioa emanda, eta honen kontrolagailua konfiguratzen
 *        du emandako TENP_konf balioarekin.
 * 
 * \param Latch hasieraketa balioa
 * \param TENP_konf konfiguraketa adierazten duen 16bit-eko balioa
 */
extern void configureTimer0(int Latch, int TENP_konf);

/*!
 * \brief Teklatuaren etenak baimentzen ditu.
 * 
 * Etenak baimentzeko lehendabizi mota guztietako eten guztiak galarazi behar dira,
 * gero REG_IE erregistroa aldatu teklatuaren etenak baimentzeko 
 * (ikusi REG_IE definitions.h-en) eta azkenik mota guztietako eten guztiak
 * baimendu behar dira.
 */
extern void allowInterruptsKeys();

/*!
 * \brief Teklatuaren etenak galarazten ditu.
 * 
 * Etenak galarazteko lehendabizi mota guztietako eten guztiak galarazi behar dira,
 * gero REG_IE erregistroa aldatu teklatuaren etenak galarazteko 
 * (ikusi REG_IE definitions.h-en) eta azkenik mota guztietako eten guztiak
 * baimendu behar dira.
 */
extern void denyInterruptsKeys();

/*!
 * \brief 0 denboragailuaren etenak baimentzen ditu.
 * 
 * Etenak baimentzeko lehenbizi mota guztietako eten guztiak galarazi behar dira,
 * gero REG_IE erregistroa aldatu denb0 etenak baimentzeko 
 * (ikusi REG_IE definitions.h-en) eta azkenik mota guztietako eten guztiak
 * baimendu behar dira.
 */
extern void allowInterruptsTimer0();

/*!
 * \brief 0 denboragailuaren etenak galarazten ditu.
 * 
 * Etenak galarazteko lehendabizi mota guztietako eten guztiak galarazi behar dira,
 * gero REG_IE erregistroa aldatu denb0 etenak galarazteko 
 * (ikusi REG_IE definitions.h-en) eta azkenik mota guztietako eten guztiak
 * baimendu behar dira.
 */
extern void denyInterruptsTimer0();

/**
 * \brief 0 denboragailua martxan jartzen du.
 * Erlojua martxan jartzeko TIMER0_CR erregistroan 7 bita aktibatzen du.
 */
extern void startClockTimer0();

/**
 * \brief 0 denboragailua gelditzen du.
 * Erlojua gelditzeko TIMER0_CR erregistroan 7 bita desaktibatzen du.
 */
extern void stopClockTimer0();

#endif