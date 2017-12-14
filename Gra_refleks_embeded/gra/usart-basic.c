
/*
#define F_CPU 32000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "hd44780.h"

#define Usart(tekst) 	UsartWriteProgmem(PSTR(tekst))

void UsartInit(void) {
	// TX = E3
	// RX = E2
	
	// 57600 baud
	// BSEL = 135
	// BSCALE = -2
	
	// ustawienie wejœæ i wyjœæ
	PORTE.OUTCLR		=	PIN2_bm;
	PORTE.OUTSET		=	PIN3_bm;
	PORTE.DIRCLR		=	PIN2_bm;
	PORTE.DIRSET		=	PIN3_bm;
	
	// konfiguracja USART
	USARTD0.CTRLC		=	USART_CMODE_ASYNCHRONOUS_gc|	// tryb asynchroniczny
							USART_PMODE_DISABLED_gc|		// bez bitu parzystoœci
							// sbmode ustawia 2 bity stopu
							USART_CHSIZE_8BIT_gc;			// tryb 8-bitowy
	USARTD0.BAUDCTRLA	=	135;
	USARTD0.BAUDCTRLB	=	(-2 << 4);//0b11100000;
	USARTD0.CTRLB		=	USART_RXEN_bm|					// w³¹czenie odbiornika
							USART_TXEN_bm;					// w³¹czenie nadajnika
}

uint8_t UsartRead(void) {
	while((USARTD0.STATUS & USART_RXCIF_bm) == 0);
//	USARTD0.STATUS |= USART_RXCIF_bm;
	return USARTD0.DATA;
}

void UsartData(uint8_t data) {
	USARTD0.DATA = data;
	while(!(USARTD0.STATUS & USART_DREIF_bm));
}

void UsartWrite(char * text) {
	while(*text) UsartData(*text++);
}

void UsartWriteProgmem(const char * data) {
	char bufor;
	while ((bufor = (char)pgm_read_byte(data++))) UsartData(bufor);
}

void UsartDec(int32_t liczba) {					// zaktualizowaæ
	if(liczba == 0) {
		Usart("0");
		return;
	}
	
	if(liczba < 0) {
		Usart("-");
		liczba = -liczba;
	}
	
	uint8_t cyfra[10] ; 
	int8_t i=0;
	
	while(liczba) {
		cyfra[i] = (uint8_t)(liczba%10);
		liczba = liczba / 10;
		++i;
	}
	--i;

	for(;i>=0;--i) {
		UsartData(cyfra[i]+48);
	}  

}

void UsartHex(int32_t liczba) {
	uint8_t i;
	uint8_t temp;
	
	if(liczba < 0) {
		Usart("-");
		liczba = -liczba;
	}
	
	for(i=0;i<2;++i) {
		if(!i) temp = liczba>>4;
			else temp = liczba % 16;
		switch(temp) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				UsartData(temp+48);
				break;
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				UsartData(temp+55);
				break;
		}
	}
}

void UsartBin(int32_t liczba) {
	if(liczba==0) {
		Usart("00000000");
		return;
	}
	
	uint8_t cyfra[8] ; 
	int8_t i;
	
	for(i=7;i>=0;--i) {
		cyfra[i] = liczba%2;
		liczba = liczba / 2;
	}

	for(i=0;i<8;++i) {
		UsartData(cyfra[i]+48);
	}  
}

void Osc32MHz(void) {
    OSC.CTRL     =    OSC_RC32MEN_bm;						// w³¹czenie oscylatora 32MHz
    while(!(OSC.STATUS & OSC_RC32MRDY_bm));					// czekanie na ustabilizowanie siê generatora
    CPU_CCP      =    CCP_IOREG_gc;							// odblokowanie zmiany Ÿród³a sygna³u
    CLK.CTRL     =    CLK_SCLKSEL_RC32M_gc;					// zmiana Ÿród³a sygna³u na RC 32MHz
}

#define CHIP_SELECT		PORTE.OUTCLR	=	PIN4_bm
#define CHIP_DESELECT	PORTE.OUTSET	=	PIN4_bm

void SpiInit(void) {
	PORTE.OUTCLR	=	0b11110000;
	PORTE.DIRSET	=	PIN4_bm | PIN5_bm | PIN7_bm;
	PORTE.DIRCLR	=	PIN6_bm;
	PORTE.REMAP		=	PORT_SPI_bm;						// zamiana miejscami SCK i MOSI
	SPIE.CTRL	=	SPI_ENABLE_bm|
					SPI_MASTER_bm|
					SPI_MODE_3_gc|
					SPI_PRESCALER_DIV64_gc;
	
}

uint8_t SpiTransmit(uint8_t data) {
	SPIE.DATA	=	data;
	while(SPIE.STATUS == 0);
	return SPIE.DATA;
}

int main(void) {
	Osc32MHz();
	LcdInit();
	UsartInit();
	SpiInit();
	
	USARTD0.DATA = 't';
	USARTD0.DATA = 'e';
	USARTD0.DATA = 's';
	USARTD0.DATA = 't';
	
	PORTA.DIR	=	PIN3_bm | PIN4_bm | PIN5_bm;
	
	
//	if((PORTE.IN & PIN2_bm) == 0) Lcd("0");
//	if((PORTE.IN & PIN2_bm)) Lcd("1");
	
	uint8_t temp;
	uint8_t temp1;
	uint8_t temp2;
	uint8_t temp3;
	
	while(1) {
		temp = UsartRead();
		switch(temp) {
			case '1':
				CHIP_SELECT;
				_delay_us(1);
				temp1 = SpiTransmit(temp1);
//				temp2 = SpiTransmit(0b11000001);   // diodki do zapalenia
//				temp3 = SpiTransmit(0);
				_delay_us(1);
				CHIP_DESELECT;
				
				UsartBin(temp1);
				Usart(" ");
// 				UsartBin(temp2);
// 				Usart(" ");
// 				UsartBin(temp3);
// 				Usart(" ");
				Usart("\n\r");
				
				break;
			case '2':
				CHIP_SELECT;
				break;
			case '3':
				CHIP_DESELECT;
				break;
				
			case '4':
				UsartBin(0b01010101);
				break;
				
			case 'q':
				PORTA.OUTTGL	=	PIN5_bm;
				break;
				
			case 'w':
				PORTA.OUTTGL	=	PIN4_bm;
				break;
				
			case 'e':
				PORTA.OUTTGL	=	PIN3_bm;
				break;
		}
		temp = 0;
	}

}

*/