#include "msp430g2553.h"
#include "display.h"
#include "register.h"
#include "misc.h"
#include "main.h"


MAIN_STRUCT m =
{
     .show_value =  1u,    // Set to 1, so we update at the start.
     .complete =    0u,
     .rpm =         0u,
     .tick =        0u,
};

const disp_config_struct disp_conf =
{
     .port_clk = set_clk,
     .port_csb = set_cs,
     .port_rs = set_rs,
     .port_si = set_si,
     .delay_func = wait_msec
};

static char buf[32];
static U8  led_state = 0u;

int main(void)
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  register_init();

  disp_set_conf(&disp_conf);

  disp_init ();

  set_backlight(1u);

  disp_write_string("Speed", 0u, DISP_HIGH);
  while (1)
  {
    //Main cycle
    if (m.show_value)
    {
        m.show_value = 0;

        value2string (m.rpm, buf ,0 ,'R');
        addchar (buf,'P');
        addchar (buf,'M');

        addchar (buf, 0xA0);
        addchar (buf, 0xA0);
        addchar (buf, 0xA0);

        disp_write_string(buf, 6u, DISP_HIGH);

        long2string(tick_total_count, buf);
        disp_write_string(buf, 0u, DISP_LOW);
    }
  }
}


/* Timer functions.  */
void timer_10msec (void)
{
  set_led(isSensor());
  m.tick++;
}

void timer_1sec (void)
{
  led_state = !led_state;
  //set_led2(led_state);

  /* So this updates the speed sensor automatically? TODO : Review, it might be unnecessary */
  m.show_value = 1u;

  if (m.tick > 400u)
  {
    m.tick = 0u;
    m.rpm = 0u;
  }
}

