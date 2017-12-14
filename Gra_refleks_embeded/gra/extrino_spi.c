/*
 * extrino_spi.c
 *
 * Created: 2014-05-18 14:47:22
 *  Author: Saturn
 */ 

#include "extrino_spi.h"


volatile struct PORTX_t PORTX; 

void SpiInit(void) {
	// sygna³y SS dla wszystkich peryferów na eXtrino XL
	PORTA.OUTSET	=	PIN3_bm | PIN4_bm;					// SPI MEM, OP AMP
	PORTA.DIRSET	=	PIN3_bm | PIN4_bm;					// SPI MEM, OP AMP
	PORTE.OUTSET	=	PIN3_bm | PIN6_bm | PIN7_bm;		// SD, PORTX, DIGPOT
	PORTE.DIRSET	=	PIN3_bm | PIN6_bm | PIN7_bm;		// SD, PORTX, DIGPOT
	
	PORTC.DIRSET	=	PIN4_bm | PIN5_bm | PIN7_bm;
	PORTC.DIRCLR	=	PIN6_bm;
	PORTC.OUTCLR	=	PIN7_bm | PIN6_bm | PIN5_bm | PIN4_bm;
	PORTC.REMAP		=	PORT_SPI_bm;						// zamiana miejscami SCK i MOSI
	SPIC.CTRL		=	SPI_ENABLE_bm|
						SPI_MASTER_bm|
						SPI_MODE_3_gc|
						SPI_PRESCALER_DIV64_gc;
	SPIC.INTCTRL	=	SPI_INTLVL_LO_gc;
	
	// przerwania
// 	PMIC.CTRL       =    PMIC_HILVLEN_bm|       // w³¹czenie przerwañ o priorytecie HI
//                          PMIC_LOLVLEN_bm;       // w³¹czenie przerwañ o priorytecie LO
//     sei();
	
}

uint8_t SpiTransmit(uint8_t data) {
	SPIC.DATA	=	data;
	while(SPIC.STATUS == 0);
	return SPIC.DATA;
}

uint8_t PortxRefresh(void) {
	cli();
	PORTE.OUTCLR	=	PIN6_bm;
	_delay_us(1);
//	PORTX.IN = SpiTransmit(PORTX.OUT);
	SPIC.DATA	=	PORTX.OUT;
//	while(SPIC.STATUS == 0);
	PORTX.IN = SPIC.DATA;
	_delay_us(1);
	PORTE.OUTSET	=	PIN6_bm;
	sei();
	return 1;		
		

}

ISR(SPIC_INT_vect) {
	//uint8_t delay = 1;
	PORTE.OUTSET	=	PIN6_bm;		// chip deselect
	//_delay_us(1);
	//while(delay++);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	PORTE.OUTCLR	=	PIN6_bm;		// chip select
	PORTX.IN = SPIC.DATA;
	SPIC.DATA	=	PORTX.OUT;
}