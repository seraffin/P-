/*
 * extrino_spi.h
 *
 * Created: 2014-05-18 14:47:35
 *  Author: Saturn
 */ 


#ifndef EXTRINO_SPI_H_
#define EXTRINO_SPI_H_

#define F_CPU 2000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>




struct PORTX_t {
	uint8_t volatile IN;
	uint8_t volatile OUT;
};
volatile struct PORTX_t PORTX; 

void SpiInit(void); 

uint8_t SpiTransmit(uint8_t data); 

uint8_t PortxRefresh(void);

#endif /* EXTRINO_SPI_H_ */