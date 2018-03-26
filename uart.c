#include "uart.h"
#include "misc.h"
#include "typedefs.h"
#include "msp430g2553.h"
#include <stdlib.h>

char x1_ch=0;  // character from COM
char x1_str[256]; //Paneb praegu testiks lihtsalt 256 tähemärki.
int  x1_istr=0;
int  x1_event=0;

char abi[24];

void uart_init (void)
{
    P1SEL = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 = 104; // 1MHz 9600 (N=BRCLK/baudrate)
    UCA0BR1 = 0; // 1MHz 9600
    //UCA0BRS0=1;
    UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
    //IE1 |= URXIE0 + UTXIE0; //Enable TxD, RxD interrupts.
    UC0IE |= UCA0RXIE; //Enable RxD interrupt
}

#pragma vector=USCIAB0RX_VECTOR 
  __interrupt void uart_receive (void) //void UART1_RX_interrupt (void)
{
	//x1_ch = rGET (rUDR1);
	//uart_led = 5;   // for 50ms blink
	
	x1_ch=UCA0RXBUF;
	UCA0TXBUF=UCA0RXBUF;
        //if (x1_istr>5){x1_event = 1;}
	if (x1_ch == '\r' || x1_ch == '\n') x1_event = 1;
	
	x1_str[x1_istr++] = x1_ch;
	x1_str[x1_istr] = 0;
}
 
int x1_handle_data (void)
{
  char abi [6];
  int x;
  if (!x1_event){return 0;}
  
  //ECHO
  x1_string ("Re: ");
  x1_string (x1_str);
  
  if (!strncmp (x1_str,"PER",3))
  {
    x = atoi(x1_str + 3);
    if (x > 100)
    {
        x = 100;
    }
    else if (x < 0)
    {
        x = 0;
    }
    x1_string ("Period is now : ");
    long2string(x, abi);
    x1_string (abi);
    TA1CCR1 = x * 10;
  }

  x1_event = 0;
  x1_istr = 0;

  return (1);
}


void x1 (char c)
{
	while (ISBIT (UCA0STAT, BIT0));
        __delay_cycles(1500);
	UCA0TXBUF=c;
}

void x1_string (char *ps) { while (*ps) x1(*ps++);}
void x1_number (int n){long2string (n,abi); x1_string (abi);}
void x1_rn (void) {x1('\n');}


