#include "msp430g2553.h"
#include "register.h"
#include "main.h"
#include "uart.h"

#define SENSOR_DEBOUNCE 6u //Sensor debounce in 10msec increments.

void calculate_rpm(void);

static void port_init(void);
static void timer_init(void);

static inline void timer_10msec (void);
static inline void timer_1sec (void);

static U16 wait_10msec = 0u;                   // count 10 msec interrupts continuously
static U16 cTimer = 0u;                        // count 10 msec interrupts for 1 second calculation
static U8 msec_counter = 0u;

static U16   sensor_ms_counter = 0u;    // counts milliseconds between sensor ticks.
volatile U32 tick_total_count = 0u;

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

static void port_init (void)
{
  /* P1.0 --- Indicator LED           */

  /* P1.1 --- UART RxD                */
  /* P1.2 --- UART TxD                */

  /* P1.3 --- Display Backlight       */
  /* P1.5 --- Display Register Select */
  /* P1.6 --- Indicator LED2          */
  /* P2.0 --- Display Chip Select     */
  /* P2.1 --- Display Clock           */
  /* P2.2 --- Display SI              */
  /* P2.4 --- Sensor digital input    */

  /* Initialize LED pin */
  P1DIR |= BIT0;
  P1DIR |= BIT6;
  /* LCD needs 4 control outputs... */
  P1DIR |= BIT5;                /*  1.5  */
  P2DIR |= BIT0 + BIT1 + BIT2;  /*  2.0  2.1  2.2 */
  P1DIR |= BIT3;

  /* 2.4 - sensor input. */
  P2DIR &= ~BIT4;
  /* Enable interrupt on P2.4 */
  P2IE |=BIT4;
  //Enable pull-up on P2.4
  P2REN |=BIT4;
}

static void timer_init (void)
{
   DCOCTL = 0;                     // Select lowest DCOx and MODx settings 
   BCSCTL1 = CALBC1_1MHZ;          // Set DCO 
   DCOCTL = CALDCO_1MHZ; 

   // Configure Timer for cyclic interrupts
   CCTL0 = CCIE;                             // CCR0 interrupt enabled
   CCR0 = 1000u;                             // Set to 1 kHz
   TACTL = TASSEL_2 + MC_1;                  // SMCLK, continuous mode

   // enable interrupts
   _EINT();
  }


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    //1 msec interval
    sensor_ms_counter++;

    if (++msec_counter < 10u)
    {
        return;
    }
    msec_counter = 0u;

    ///////////// 10 msec interval
	timer_10msec();

	if (++cTimer < 100)
	{
	    return;
	}

	cTimer = 0;

	//////////// 1 second interval
    timer_1sec();
}


/* Timer functions.  */
inline static void timer_10msec (void)
{
    ++wait_10msec;
    set_led(isSensor());
}

inline static void timer_1sec (void)
{
  static U8  led_state = 0u;
  led_state = !led_state;
  //set_led2(led_state);

  /* So this updates the speed sensor automatically? TODO : Review, it might be unnecessary */
  m.show_value = 1u;

  if (sensor_ms_counter > 400u)
  {
    sensor_ms_counter = 0u;
    m.rpm = 0u;
  }
}


//Interrupt for sensor
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P2IE  &= ~BIT4;
  P2IFG &= ~BIT4;       // P1.4 IFG cleared

  if (isSensor())
  {
      calculate_rpm ();
      m.show_value = 1u;    /* Should we trigger a redraw every time? */
  }

  //Why is this necessary???
  P2IE |= BIT4;         // P1.4 interrupt enabled
}

/* TODO : This function is way too complex for such a simple operation. */
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


void calculate_rpm (void)
{
  U32 value;

  //Assume that this is debounce effect?
  if (sensor_ms_counter < SENSOR_DEBOUNCE ){ return ;}

  tick_total_count++;
  //m.tick is in 1msec intervals.
  value = 60000u / sensor_ms_counter;

  m.rpm = value;
  sensor_ms_counter = 0u;
}

U8 isSensor   (void){if (ISBIT (P2IN,BIT4)) {return (0u);}else {return (1u);}}

void set_led       (U8 b)   {if(b){SETBIT(P1OUT, BIT0);} else {CLRBIT(P1OUT,  BIT0);}}
void set_led2      (U8 b)   {if(b){SETBIT(P1OUT, BIT6);} else {CLRBIT(P1OUT,  BIT6);}}

void set_backlight (U8 b)   {if(b){SETBIT(P1OUT, BIT3);} else {CLRBIT(P1OUT,  BIT3);}}
void set_rs        (U8 b)   {if(b){SETBIT(P1OUT, BIT5);} else {CLRBIT(P1OUT,  BIT5);}}
void set_cs        (U8 b)   {if(b){SETBIT(P2OUT, BIT0);} else {CLRBIT(P2OUT,  BIT0);}}
void set_clk       (U8 b)   {if(b){SETBIT(P2OUT, BIT1);} else {CLRBIT(P2OUT,  BIT1);}}
void set_si        (U8 b)   {if(b){SETBIT(P2OUT, BIT2);} else {CLRBIT(P2OUT,  BIT2);}}

