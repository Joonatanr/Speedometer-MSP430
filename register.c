#include "msp430g2553.h"
#include "register.h"
#include "main.h"
#include "uart.h"

void calculate_speed(void);

static void port_init(void);
static void timer_init(void);

U16 wait_10msec = 0;     // count 10 msec interrupts continuously
U16 cTimer = 0;          // count 10 msec interrupts for 1 second calculation


void wait20mksec (U16 i)
{
    U16 x;
    for (x = 0; x < i; x++)
    {
      __delay_cycles (20);
    }
}

void wait_msec (U16 time)
{
  time = (time + 5) / 10;
  wait_10msec = 0;
  while (wait_10msec < time);
}

void register_init(void)
{
    port_init (); //Initalize port directions

    timer_init (); //Initialize timer

    uart_init ();
}


/*** Private functions start here **/

void port_init (void)
{
  /* P1.0 --- Indicator LED           */

  /* P1.1 --- UART RxD                */
  /* P1.2 --- UART TxD                */

  /* P1.4 --- Display Backlight       */
  /* P1.5 --- Display Register Select */
  /* P1.6 --- Indicator LED2          */
  /* P2.0 --- Display Chip Select     */
  /* P2.1 --- Display Clock           */
  /* P2.2 --- Display SI              */
  /* P2.4 --- Sensor digital input    */

  /* Initialize LED pin */
  P1DIR |= BIT0;
  /* LCD needs 4 control outputs... */
  P1DIR |= BIT5;                /*  1.5  */
  P2DIR |= BIT0 + BIT1 + BIT2;  /*  2.0  2.1  2.2 */
  P1DIR |= BIT4;

  /* 2.4 - sensor input. */
  P2DIR &= ~BIT4;
  P2IE |=BIT4;
  P2REN |=BIT4;
  //Enable pull-up on P2.4
  P2REN |=BIT4;

  /* P1 --- BIT0, BIT6 */
  /* P2 --- BIT3, BIT4, BIT5 */
}

void timer_init (void)
{
   DCOCTL = 0;                     // Select lowest DCOx and MODx settings 
   BCSCTL1 = CALBC1_1MHZ;          // Set DCO 
   DCOCTL = CALDCO_1MHZ; 
  // Configure Timer 
    CCTL0 = CCIE;                             // CCR0 interrupt enabled 
    CCR0 = 10000; 
    TACTL = TASSEL_2 + MC_1;                  // SMCLK, continuous mode  

   // enable interrupts
   _EINT();
  }

void timer2_init (void){
  CCTL1 = CCIE;
  TA1CTL = TASSEL_2 + MC_2 + ID_3; //Continuous, divide clock with 8
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{

         ///////////// 10 msec interval
	++wait_10msec;
	timer_10msec();
        //UCA0TXBUF = 50;  
	if (++cTimer < 100) return;
	cTimer = 0;

	//////////// 1 second interval
    timer_1sec();
}


//Interrupt for sensor
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P2IE  &= ~BIT4;
  P2IFG &= ~BIT4;       // P1.4 IFG cleared
  m.show_value = 1;
  calculate_speed ();
  m.tick = 0;           //Reset msec counter.
  P2IE |= BIT4;         // P1.4 interrupt enabled
}

U32 rnd (unsigned long x)
{
    int z = (int)fmod (x , 5);
    if (z > 2)
    {
        x += (5 - z);
    }
    else
    {
        x -= z;
    }
    return x;
}

void calculate_speed (void)
{
  unsigned long s;
  unsigned long v;
  if (m.tick < 5){return ;}
  s = m.wheel_diameter * PI;
  m.tick_cnt++;

  v = s / m.tick;
  v = v * 36;
  v = v / 100;
  v = rnd (v);

  if (v > 1500)
  {
      return;
  }
  m.speed = v;
}

U8 isSensor   (void){if (ISBIT (P2IN,BIT4)) {return (0u);}else {return (1u);}}

void set_led       (U8 b)   {if(b){SETBIT(P1OUT, BIT0);} else {CLRBIT(P1OUT,  BIT0);}}
void set_led2      (U8 b)   {if(b){SETBIT(P1OUT, BIT6);} else {CLRBIT(P1OUT,  BIT6);}}

void set_backlight (U8 b)   {if(b){SETBIT(P1OUT, BIT4);} else {CLRBIT(P1OUT,  BIT4);}}
void set_rs        (U8 b)   {if(b){SETBIT(P1OUT, BIT5);} else {CLRBIT(P1OUT,  BIT5);}}
void set_cs        (U8 b)   {if(b){SETBIT(P2OUT, BIT0);} else {CLRBIT(P2OUT,  BIT0);}}
void set_clk       (U8 b)   {if(b){SETBIT(P2OUT, BIT1);} else {CLRBIT(P2OUT,  BIT1);}}
void set_si        (U8 b)   {if(b){SETBIT(P2OUT, BIT2);} else {CLRBIT(P2OUT,  BIT2);}}

