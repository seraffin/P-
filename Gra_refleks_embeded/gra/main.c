#define F_CPU 2000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"
#include "extrino_spi.h"
#include <avr/pgmspace.h>

// uint8_t - unsigned integer o dlugosci 1 bajta
volatile uint8_t dioda;

volatile uint32_t rounds;

// uint32_t - unsigned integer o dlugosci 4 bajtow
volatile uint32_t timer;

volatile uint64_t rgb;

int main(void) {
	// Inizjalizacja wyswietlacza z biblioteki HD44780.c
	LcdInit();
	
	// FLIP przylutowany jest do n�ki E5, zerujemy bit 5 w rejestrze DIR na porcie E, poprzez wpisanie warto�ci PIN5_bm do rejestru DIRCLR
	// DIR - rejestr ten decyduje czy dana n�ka ma by� wej�ciem czy wyj�ciem. Wpisanie jedynki powoduje
	//		skonfigurowanie pinu jako wyj�cia, a zero oznacza wej�cie.
	// DIRCLR - zeruje stan bit�w odpowiedzialnych za to czy wskazana n�ka ma by� wej�ciem czy wyj�ciem ( w naszym przypadku wyjscie)
	PORTE.DIRCLR	=	PIN5_bm;
	
	// RGB - piny 0,1,2 jako wejscia
	PORTF.DIRSET	=	PIN2_bm | PIN1_bm | PIN0_bm;
	
	// klawiatura
	PORTE.DIRSET	=	PIN6_bm;
	PORTE.OUTSET	=	PIN6_bm; //ustawienie stanu wysokiego na pinie 6
	SpiInit();
	
	// timer do mierzenia czasu
	TCC0.PER		=	3125;						// rejestr okre�laj�cy maksymaln� warto��, kt�r� timer mo�e osi�gn��
	TCC0.CTRLB		=	TC_WGMODE_NORMAL_gc;		// tryb normalny
	TCC0.INTCTRLA	=	TC_OVFINTLVL_LO_gc;			// rejestr konfiguruj�cy przerwania - przepe�nienie ma generowa� przerwanie LO
	PMIC.CTRL		=	PMIC_LOLVLEN_bm;			// odblokowanie przerwa� o priorytecie LO
	sei();											// globalne odblokowanie przerwa�
	
	TCC0.CTRLA		=	TC_CLKSEL_DIV64_gc;			// ustawienie preskalera i uruchomienie timera
	
	// timer do przyciemnienia RGB
	
	TCE0.PER		=	333;
	TCE0.CTRLB		=	TC_WGMODE_NORMAL_gc;
	TCE0.INTCTRLA	=	TC_OVFINTLVL_LO_gc;
	
	// timer do losowania diody
	TCD0.CTRLA		=	TC_CLKSEL_DIV1_gc;
	TCD0.CTRLB		=	TC_WGMODE_NORMAL_gc;
	
	Lcd1;
	Lcd("Select size of");
	Lcd2;
	Lcd("a game");
	
	_delay_ms(2000);
	
	// konfiguracja systemu zdarze�
	EVSYS.CH0MUX       =    EVSYS_CHMUX_PORTE_PIN0_gc;        // pin E0 wywo�uje zdarzenie
	EVSYS.CH0CTRL      =    EVSYS_QDEN_bm|                    // w��czenie dekodera w systemie zdarze�
							EVSYS_DIGFILT_8SAMPLES_gc;        // uruchomienie filtru cyfrowowego
	
	// konfiguracja timera do encodera
	TCC1.CTRLA         =    TC_CLKSEL_EVCH0_gc;               // taktowanie systemem zdarze�
	TCC1.CTRLD         =    TC_EVACT_QDEC_gc |                // w��czenie dekodera kwadraturowego
							TC_EVSEL_CH0_gc;                  // dekoder zlicza impulsy z kana�u 0
	while(1)
	{
		LcdClear();
		
		if(!(PORTE.IN & PIN5_bm))
		{
			Lcd("Accepted! ");
			break;
		}
		
		Lcd("Turns: ");
		LcdDec(TCC1.CNT / 4);                                 // jeden przeskok to 4 impulsy (CNT - rejestr przechowuj�cy aktualn� wato�� timera)
		rounds = TCC1.CNT / 4;
		_delay_ms(100);                                       // czekanie 100ms
	}
	
	LcdClear();
	Lcd("Let's play");
	_delay_ms(2000);
	
	PortxRefresh();
	
	dioda = 1;
	
	uint32_t suma = 0;
	uint16_t licznik = 0;
	uint64_t ostatni = 0;
	uint64_t srednia = 0;
	uint8_t old_dioda = 0;
	
	timer = 0;
	
	while(1) {
		
		// Je�eli na naszym porcie X pojawi si� stan wysoki (1). Port X to nasze przyciski
		if(PORTX.IN == dioda)
		{
			rounds--;
			old_dioda = dioda;
			dioda = TCD0.CNTL & 0b00000111;
			dioda = 1 << dioda;
			
			if(dioda == old_dioda)
			{
				dioda = dioda < 1;
				if(dioda == 0) dioda = 1;
			}
			ostatni = timer;
			suma = suma + timer *100;
			
			
			licznik++;
			srednia = suma / licznik;
			
			timer = 0;
			
			rgb = PIN0_bm;							// Ustawienie koloru zielonego dla diody RGB
			TCE0.CTRLA		=	TC_CLKSEL_DIV1_gc;	// W��czenie timera = wy��czenie diody RGB
			
			LcdClear();
			Lcd1;				// Skok do pierwszej linii wyswietlacza
			Lcd("Hit:");
			LcdDec(licznik);
			Lcd(" Last:");
			LcdDecComma(ostatni, 1);
			Lcd("       ");
			Lcd2;
			Lcd("Average:");
			LcdDecComma(srednia, 3);
			Lcd("       ");
			
			_delay_ms(1);
			
			TCE0.CTRLA		=	0;		// Wy��czenie timera = wy��czenie diody RGB
			
			if(rounds == 0)
			{
				break;
			}
		}
		else
		if (PORTX.IN == old_dioda || PORTX.IN == 0){;}
		else
		{
			rgb = PIN2_bm;				// Ustawienie koloru czerwonego dla diody RGB
			TCE0.CTRLA		=	TC_CLKSEL_DIV1_gc;
			TCE0.CTRLA		=	0;
		}
	}
}


ISR(TCC0_OVF_vect){                                   // przerwanie przepe�nienia TCC0
	static uint8_t stan = 0;
	timer++;
	stan = !stan;
	if(stan) PORTX.OUT = 0;
	else PORTX.OUT = dioda;
}

ISR(TCE0_OVF_vect){
	PORTF.OUTSET = rgb;
	PORTF.OUTCLR = rgb;
}