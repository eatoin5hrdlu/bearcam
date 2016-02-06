/*
 *	ISERIAL.C
 *
 *  Interrupt-driven serial output for 16Cxx mid-range PIC devices.
 *
 *	Copyright (C) 2008 Museum of Life and Science.
 *	Author: Peter Reintjes <peter.reintjes@ncmls.org>	
 *	Comments:
 *	
 *   I'm using this code to send commands to remote cameras,
 *   based upon switch inputs so there is no need for serial
 *   input, allowing for higher baud rates than usual I/O.
 *   In fact, this is so fast that we have to set the timer
 *   twice from an 8-bit timer value (twice the baud rate).
 * 
 *   It also toggles RB4 on every interrupt to for the charge
 *   pump that creates a negative voltage for RS232/422/485.
 *   Modified March 2009 for 20 Mhz to support 9600 baud.
 */

#include	"camera.h"

#define TxData    RA3      /* Transmit port bit */
#define	XTAL	  20000000 /* Crystal frequency (Hz). */
#define CLOCK     XTAL/4   /* Actual Clock to Timers */
#define MAXBAUD   76800    /* Theoretically possible prescale=0*/
#define	BRATE	  2400	   /* Baud rate. */

/*
 * At 2400 baud, adding 33 to the *exact* timer calculation puts
 * the timing halfway between the rates that dropped characters.
 * Since sucessive baud rates of doubled, we select different
 * baud rates by changing the pre-scaler power of two.
 */


#define DELTA 32       /* The BAUD rates need a tiny nudge */
#define PRESCALE2  1
#define PRESCALE1  0
#define PRESCALE0  1

#define TIMER_VALUE   ( 256 - (CLOCK/MAXBAUD - DELTA) )

/*
 * Previously 13 bits 2-stop lots of margin, but this works
 */

#define TRANSMIT_NUM_BITS	13  // 1 start, 8 data, 1 stop + margin

static unsigned char	sendbuffer; // Output character buffer
static unsigned char	send_bitno; 
static bit 	            tx_next_bit;

/*
 * Initialize I/O, serial port timings and camera switches (pullups)
 */

void
initialize(void)
{
  // INPUT OUTPUT PIN DIRECTIONS
   // CMCON = 0x07; // turn off comparator module(for 16F627A)
   // VRCON = 0;
	TRISA = 0x10;  // RA0-1 video cntl, RA2=pulse, RA3=xmit
	TRISB = 0xFF;  // RB0-7 are inputs
  // DEFAULT VIDEO SWITCHER SETTINGS
	RA0 = RA1 = 0; 

	T0CS = 0;               // Set timer mode for Timer0
	TMR0 = TIMER_VALUE;
	T0IE = 1;	        // Enable the Timer0 interrupt.
	GIE = 1;
    PSA = 0;  // Prescaler to Timer0
    PS2 = PRESCALE2;
    PS1 = PRESCALE1;
    PS0 = PRESCALE0;
  // ACTIVATE PULL-UP RESISTORS ON PORTB
   RBPU = 0;
}

void
putch(char c)
{
	while(send_bitno)
		continue;
	tx_next_bit = 0;
	sendbuffer = c;
	send_bitno = TRANSMIT_NUM_BITS;
}

/*
 * Serial Interrupt Service Routine serial_isr
 * Transmits characters given to putch();
 * 
 * The Interrupt runs at sixteen times the BRATE
 * because the timer value is only eight bits.
 */

interrupt void
serial_isr(void)
{
    TMR0 = TIMER_VALUE;
    T0IF = 0;              /* Re-enable timer */
     /* Pulses for the negative voltage generator/charge-pump */
    // RA2 = !RA2;

    /* TRANSMIT */
    if(send_bitno) {
      TxData = !tx_next_bit;
      tx_next_bit = sendbuffer & 1;
      sendbuffer = (sendbuffer >> 1) | 0x80;
      send_bitno--;
    }
}
