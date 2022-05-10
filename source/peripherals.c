#include <nds.h>
#include <stdio.h>

#include "peripherals.h"

/*!
 * \brief Teklatuko botoiak sinkronizatzen ditu etenen bidez edo inkesta bidez
 * indibidualki emandako TEK_konf aldagaiarekin. TEK_konf parametroa 16 biteko
 * int bat da eta bit bakoitzak tekla espezifiko bat etenen edo inkesta bidez
 * konfiguratuta dagoela adierazten du. 1 balioa etenen bidez badago, 0 balio
 * inkesta bidez. Jadanik 14 bita aktibatzen du.
 * 
 * \param TEK_konf aldatu nahi diren teklak adierazten duen balioa
 */
void configureKeys(int TEK_konf)
{
	REG_KEYCNT |= 0x1<<14 | TEK_konf;
}

/*!
 * \brief 0 Denboragailua hasieratzen du kontaketa zein baliotatik hastea nahi
 * den Latch balioa emanda, eta honen kontrolagailua konfiguratzen du emandako
 * TENP_konf balioarekin.
 * 
 * \param Latch hasieraketa balioa
 * \param TENP_konf konfiguraketa adierazten duen 16bit-eko balioa
 */
void configureTimer0(int Latch, int TENP_konf)
{
	TIMER0_DATA = Latch;
	TIMER0_CR |= TENP_konf;
}

/*!
 * \brief Teklatuaren etenak baimentzen ditu. Etenak baimentzeko lehenbizi mota
 * guztietako eten guztiak galarazi behar dira, gero REG_IE erregistroa aldatu
 * teklatuaren etenak baimentzeko eta azkenik mota guztietako eten guztiak
 * baimendu behar dira.
 */
void allowInterruptsKeys()
{
	denyAllInterrupts();
	REG_IE |= IRQ_KEYS;
	allowAllInterrupts();
}

/*!
 * \brief Teklatuaren etenak galarazten ditu. Etenak galarazteko lehendabizi
 * mota guztietako eten guztiak galarazi behar dira, gero REG_IE erregistroa
 * aldatu teklatuaren etenak galarazteko eta azkenik mota guztietako eten
 * guztiak baimendu behar dira.
 */
void denyInterruptsKeys()
{
	denyAllInterrupts();
	REG_IE &= ~(IRQ_KEYS);
	allowAllInterrupts();
}  

/*!
 * \brief 0 denboragailuaren etenak baimentzen ditu. Etenak baimentzeko
 * lehenbizi mota guztietako eten guztiak galarazi behar dira, gero REG_IE
 * erregistroa aldatu denb0 etenak baimentzeko eta azkenik mota guztietako
 * eten guztiak baimendu behar dira.
 */
void allowInterruptsTimer0()
{
	denyAllInterrupts();
	REG_IE |= IRQ_TIMER0;
	allowAllInterrupts();
}

/*!
 * \brief 0 denboragailuaren etenak galarazten ditu. Etenak galarazteko
 * lehendabizi mota guztietako eten guztiak galarazi behar dira, gero REG_IE
 * erregistroa aldatu denb0 etenak galarazteko eta azkenik mota guztietako
 * eten guztiak baimendu behar dira.
 */
void denyInterruptsTimer0()
{
	denyAllInterrupts();
	REG_IE &= ~(IRQ_TIMER0);
	allowAllInterrupts();
}

/*!
 * \brief 0 denboragailua martxan jartzen du. Erlojua martxan jartzeko
 * TIMER0_CR erregistroan 7 bita aktibatzen du.
 */
void startClockTimer0()
{
	TIMER0_CR |= TIMER_ENABLE;
}

/*!
 * \brief 0 denboragailua gelditzen du. Erlojua gelditzeko TIMER0_CR
 * erregistroan 7 bita desaktibatzen du.
 */
void stopClockTimer0()
{
	TIMER0_CR &= ~(TIMER_ENABLE);
}
