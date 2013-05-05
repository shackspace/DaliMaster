#include <avr/io.h>

#include "suart.h"

#include "fifo.h" 
#include <util/delay.h>

#define BAUDRATE 9600

#define nop() __asm volatile ("nop")

#ifdef SUART_TXD
    #define SUART_TXD_PORT PORTD
    #define SUART_TXD_DDR  DDRD
    #define SUART_TXD_BIT  PD2
    static volatile uint16_t outframe;
#endif // SUART_TXD 

#ifdef SUART_RXD
    #define SUART_RXD_PORT PORTB
    #define SUART_RXD_PIN  PINB
    #define SUART_RXD_DDR  DDRB
    #define SUART_RXD_BIT  PB6
    static volatile uint16_t inframe;
    static volatile uint8_t inbits, received;

    #ifdef _FIFO_H_
        #define INBUF_SIZE 4
        static uint8_t inbuf[INBUF_SIZE];
        fifo_t infifo;
    #else // _FIFO_H_ 
        static volatile uint8_t indata;
    #endif // _FIFO_H_ 
#endif // SUART_RXD 

// Initialisierung für einen ATmega8 
// Für andere AVR-Derivate sieht dies vermutlich anders aus: 
// Registernamen ändern sich (zB TIMSK0 anstatt TIMSK, etc). 
void suart_init()
{
    uint8_t tifr = 0;
    uint8_t sreg = SREG;
    cli();

    // Mode #4 für Timer1 
    // und volle MCU clock 
    // IC Noise Cancel 
    // IC on Falling Edge 
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS10) | (0 << ICES1) | (1 << ICNC1);

    // OutputCompare für gewünschte Timer1 Frequenz 
    OCR1A = (uint16_t) ((uint32_t) F_CPU/BAUDRATE);

#ifdef SUART_RXD
    GTCCR = (1 << ICPSEL1);
    SUART_RXD_DDR  &= ~(1 << SUART_RXD_BIT);
    SUART_RXD_PORT &= ~(1 << SUART_RXD_BIT);
    TIMSK1 |= (1 << ICIE1);
    tifr  |= (1 << ICF1) | (1 << OCF1B);
#else
    TIMSK1 &= ~(1 << ICIE1);
#endif // SUART_RXD 

#ifdef SUART_TXD
    tifr |= (1 << OCF1A);
    SUART_TXD_PORT |= (1 << SUART_TXD_BIT);
    SUART_TXD_DDR  |= (1 << SUART_TXD_BIT);
    outframe = 0;
#endif // SUART_TXD 

    TIFR1 = tifr;

    SREG = sreg;

#ifdef _FIFO_H_
   fifo_init (&infifo,   inbuf, INBUF_SIZE);
#endif // _FIFO_H_

}

#ifdef SUART_TXD
void suart_putc (const char c)
{
    do
    {
        sei(); nop(); cli(); // yield(); 
    } while (outframe);

    // frame = *.P.7.6.5.4.3.2.1.0.S   S=Start(0), P=Stop(1), *=Endemarke(1) 
    outframe = (3 << 9) | (((uint8_t) c) << 1);

    TIMSK1 |= (1 << OCIE1A);
    TIFR1   = (1 << OCF1A);

    sei();
}
#endif // SUART_TXD 

#ifdef SUART_TXD

void suart_putstring(char* s)
{
    while(*s)
    {
        suart_putc(*s);
	s++;
	_delay_ms(1);
    }
}

#endif // SUART_TXD 

#ifdef SUART_TXD
ISR ( TIMER1_COMPA_vect)
{
    uint16_t data = outframe;
   
    if (data & 1)      SUART_TXD_PORT |=  (1 << SUART_TXD_BIT);
    else               SUART_TXD_PORT &= ~(1 << SUART_TXD_BIT);
   
    if (1 == data)
    {
        TIMSK1 &= ~(1 << OCIE1A);
    }   
   
    outframe = data >> 1;
}
#endif // SUART_TXD

#ifdef SUART_RXD
ISR ( TIMER1_CAPT_vect)
{
    uint16_t icr1  = ICR1;
    uint16_t ocr1a = OCR1A;
   
    // Eine halbe Bitzeit zu ICR1 addieren (modulo OCR1A) und nach OCR1B
    uint16_t ocr1b = icr1 + ocr1a/2;
    if (ocr1b >= ocr1a)
        ocr1b -= ocr1a;
    OCR1B = ocr1b;
   
    TIFR1 = (1 << OCF1B);
    TIMSK1 = (TIMSK1 & ~(1 << ICIE1)) | (1 << OCIE1B);
    inframe = 0;
    inbits = 0;
}
#endif // SUART_RXD

#ifdef SUART_RXD
ISR (TIMER1_COMPB_vect)
{
    uint16_t data = inframe >> 1;
   
    if (SUART_RXD_PIN & (1 << SUART_RXD_BIT))
        data |= (1 << 9);
      
    uint8_t bits = inbits+1;
   
    if (10 == bits)
    {
        if ((data & 1) == 0)
            if (data >= (1 << 9))
            {
#ifdef _FIFO_H_         
                _inline_fifo_put (&infifo, data >> 1);
#else            
                indata = data >> 1;
#endif // _FIFO_H_            
                received = 1;
            }
      
        TIMSK1 = (TIMSK1 & ~(1 << OCIE1B)) | (1 << ICIE1);
        TIFR1 = (1 << ICF1);
    }
    else
    {
        inbits = bits;
        inframe = data;
    }
}
#endif // SUART_RXD

#ifdef SUART_RXD
#ifdef _FIFO_H_

int suart_getc_wait()
{
    return (int) fifo_get_wait (&infifo);
}

int suart_getc_nowait()
{
    return fifo_get_nowait (&infifo);
}

#else // _FIFO_H_

int suart_getc_wait()
{
    while (!received)   {}
    received = 0;
   
    return (int) indata;
}

int suart_getc_nowait()
{
    if (received)
    {
        received = 0;
        return (int) indata;
    }
   
    return -1;
}

#endif // _FIFO_H_
#endif // SUART_RXD

