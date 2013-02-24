#include "euart.h"

#define MSK_UART_5BIT           0x00 /* UCSRC register */
#define MSK_UART_6BIT           0x02
#define MSK_UART_7BIT           0x04
#define MSK_UART_8BIT           0x06
#define MSK_UART_9BIT           0x06
#define MSK_UART_UPM            0x30
#define MSK_UART_USBS           0x08

#define MSK_UART_RX_DONE        0x80 /* UCSRA register */
#define MSK_UART_TX_COMPLETE    0x40
#define MSK_UART_DRE            0x20
#define MSK_UART_FE             0x10
#define MSK_UART_UPE            0x04
#define MSK_UART_DOR            0x08

#define MSK_UART_RXCIE          0x80 /* UCSRB register */
#define MSK_UART_TXCIE          0x40
#define MSK_UART_UDRIE          0x20
#define MSK_UART_ENABLE_RX      0x10
#define MSK_UART_ENABLE_TX      0x08
#define MSK_UART_TX_BIT9        0x01
#define MSK_UART_RX_BIT9        0x02

#define MSK_EUSART_TX_5BIT		0x00 /* EUCSRA register */
#define MSK_EUSART_TX_6BIT		0x10
#define MSK_EUSART_TX_7BIT		0x20
#define MSK_EUSART_TX_8BIT		0x30
#define MSK_EUSART_TX_9BIT		0x70
#define MSK_EUSART_TX_13BIT		0x80
#define MSK_EUSART_TX_14BIT		0x90
#define MSK_EUSART_TX_15BIT		0xA0
#define MSK_EUSART_TX_16BIT		0xB0
#define MSK_EUSART_TX_17BIT		0xF0

#define MSK_EUSART_RX_5BIT		0x00
#define MSK_EUSART_RX_6BIT		0x01
#define MSK_EUSART_RX_7BIT		0x02
#define MSK_EUSART_RX_8BIT		0x03
#define MSK_EUSART_RX_9BIT		0x07
#define MSK_EUSART_RX_13BIT		0x08
#define MSK_EUSART_RX_14BIT		0x09
#define MSK_EUSART_RX_15BIT		0x0A
#define MSK_EUSART_RX_16BIT		0x0B
#define MSK_EUSART_RX_1617BIT	0x0E
#define MSK_EUSART_RX_17BIT		0x0F

#define MSK_EUSART_BODR			0x01 /* EUCSRB register */
#define MSK_EUSART_EMCH			0x02
#define MSK_EUSART_EUSBS		0x08
#define MSK_EUSART_EUSART		0x10

#define MSK_EUSART_STP			0x03 /* EUCSRC register */
#define MSK_EUSART_F1617		0x04
#define MSK_EUSART_FEM			0x08

//_____ M A C R O S ________________________________________________________

#define EUSART_set_ubrr(fosc,bdr)  { UBRRH  = (Uint8)((((((((Uint32)fosc*1000L)<<1)/((Uint32)bdr*16))+1)>>1)-1)>>8); \
                                     UBRRL  = (Uint8)(( (((((Uint32)fosc*1000)<<1)/((Uint32)bdr*16))+1)>>1)-1)    ; }
#define EUSART_set_mubrr(fosc,bdr) { MUBRRH = (Uint8)(((((((Uint32)fosc*1000L)<<1)/((Uint32)(bdr)))+1)>>1)>>8); \
                                     MUBRRL = (Uint8)( (((((Uint32)fosc*1000)<<1)/((Uint32)(bdr)))+1)>>1)    ; }
    // ----------
#define EUSART_clear()           { UCSRB=0; EUCSRB=0; UCSRC=UDR; EUCSRC=EUDR; UCSRA=0x40; EUCSRA=0x33; \
                                   UCSRC=0x06; EUCSRC=0; UBRRH=0; UBRRL=0; MUBRRH=0; MUBRRL=0;         }
    // ----------
#define EUSART_enable()          ( EUCSRB |=  MSK_EUSART_EUSART )
#define EUSART_disable()         ( EUCSRB &= ~MSK_EUSART_EUSART )
#define EUSART_tx_complete()     ( UCSRA  &   MSK_UART_TX_COMPLETE )
#define EUSART_set_tx_busy()
#define EUSART_put_msb(c)        ( EUDR = c )
#define EUSART_put_lsb(c)        ( UDR  = c )
#define EUSART_rx_ready()        ( UCSRA & MSK_UART_RX_DONE )
#define EUSART_tx_buffer_ready() ( UCSRA & MSK_UART_DRE )
#define EUSART_get_msb()         ( EUDR )
#define EUSART_get_lsb()         ( UDR  )

    // ----------
#define EUSART_ack_rx()
#define EUSART_ack_tx()                 ( UCSRA &= ~MSK_UART_TX_COMPLETE )

    // ----------
#define EUSART_enable_rx()              ( UCSRB |= MSK_UART_ENABLE_RX )
#define EUSART_enable_tx()              ( UCSRB |= MSK_UART_ENABLE_TX )

    // ----------
#define EUSART_disable_rx()             ( UCSRB &= ~MSK_UART_ENABLE_RX )
#define EUSART_disable_tx()             ( UCSRB &= ~MSK_UART_ENABLE_TX )

    // ----------
#define EUSART_set_size(n_tx,n_rx)      { EUCSRA = 0; EUCSRA |= n_tx; EUCSRA |= n_rx; }

    // ----------
#define EUSART_enable_rx_interrupt()    ( UCSRB |=  MSK_UART_RXCIE )
#define EUSART_disable_rx_interrupt()   ( UCSRB &= ~MSK_UART_RXCIE )
#define EUSART_enable_udre_interrupt()  ( UCSRB |=  MSK_UART_UDRIE )
#define EUSART_disable_udre_interrupt() ( UCSRB &= ~MSK_UART_UDRIE )
#define EUSART_enable_tx_interrupt()    ( UCSRB |=  MSK_UART_TXCIE )
#define EUSART_disable_tx_interrupt()   ( UCSRB &= ~MSK_UART_TXCIE )

    // ----------
#define EUSART_enable_manchester()      ( EUCSRB |=  MSK_EUSART_EMCH )
#define EUSART_disable_manchester()     ( EUCSRB &= ~MSK_EUSART_EMCH )
#define EUSART_enable_msb_first()       ( EUCSRB |=  MSK_EUSART_BODR )
#define EUSART_enable_lsb_first()       ( EUCSRB &= ~MSK_EUSART_BODR )

    // ----------
#define UART_set_parity(par)            { UCSRC  &= ~MSK_UART_UPM; UCSRC |= par; }
#define UART_set_one_tx_stop()          ( UCSRC  &= ~MSK_UART_USBS )
#define UART_set_two_tx_stop()          ( UCSRC  |=  MSK_UART_USBS )
#define EUSART_set_one_rx_stop()        ( EUCSRB &= ~MSK_EUSART_EUSBS )
#define EUSART_set_two_rx_stop()        ( EUCSRB |=  MSK_EUSART_EUSBS )

    // ----------
#define EUSART_get_parity_error()       ( UCSRA  & MSK_UART_UPE)
#define EUSART_get_ovr_error()          ( UCSRA  & MSK_UART_DOR)
#define EUSART_get_fe_error()           ( UCSRA  & MSK_UART_FE )
#define EUSART_get_fem_error()          ((EUCSRC & MSK_EUSART_FEM) << 2 )
#define EUSART_get_stp()                ( EUCSRC & MSK_EUSART_STP)

    // ----------
#define EUSART_get_f1617()              ((EUCSRC & MSK_EUSART_F1617) << 4 )
#define EUSART_get_bit17()              ((UCSRB  & MSK_UART_RX_BIT9) << 6 )

#ifdef LITTLE_ENDIAN     
#  define MSB(Uint16)        (((Uint8* )&Uint16)[1])
#  define LSB(Uint16)        (((Uint8* )&Uint16)[0])
#else // BIG_ENDIAN         
#  define MSB(Uint16)        (((Uint8* )&Uint16)[0])
#  define LSB(Uint16)        (((Uint8* )&Uint16)[1])
#endif

Uint8 eusart_rx_error;

void euart_init()
{
	EUSART_enable();

	EUSART_enable_manchester();

	EUSART_enable_msb_first();

	EUSART_set_mubrr(F_CPU, 1200);
	EUSART_set_ubrr(F_CPU,  1200);

	EUSART_set_size(16, 8);

	UART_set_two_tx_stop();
	EUSART_set_two_rx_stop(); 

	EUSART_enable_rx();
	EUSART_enable_tx();

	EUSART_enable_rx_interrupt();
}

Bool euart_tx_done(void)
{
	if (EUSART_tx_complete()!=0)
    	{
       		EUSART_ack_tx();
        	return TRUE;
    	}
    	else return FALSE;
}

Uint16 euart_put(Uint16 ch)
{
	Union16 c;

    	c.w = ch;

    	EUSART_put_msb(MSB(c));     // Even if <= 8-bit data
    	EUSART_put_lsb(LSB(c));
    	return ch;
}

Bool euart_rx_ready(void)
{
	if (EUSART_rx_ready()!=0) return TRUE;
    	else return FALSE;
}

Uint8 euart_get(void)
{
	Uint16 eusart_data;
    	//- Wait RxC

    	//- Read Stop bits
    	eusart_rx_error = EUSART_get_stp();
    	//- Capture all error flags
    	eusart_rx_error |= EUSART_get_parity_error();   // Parity error
    	eusart_rx_error |= EUSART_get_ovr_error();      // Data overrun error
    	eusart_rx_error |= EUSART_get_fe_error();       // Level framing error
    	eusart_rx_error |= EUSART_get_fem_error();      // Manchester framing error
    	//- Read F16 interceptor ... then bit 17
    	eusart_rx_error |= EUSART_get_f1617();
    	eusart_rx_error |= EUSART_get_bit17();

    	//- Read data buffer
    	eusart_data  = (Uint16)(EUSART_get_msb() << 8);
    	eusart_data |= (Uint16)(EUSART_get_lsb());

    	return eusart_data;
}
