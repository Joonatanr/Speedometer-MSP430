#include "msp430g2553.h"
#include "display.h"
#include "register.h"
#include "misc.h"
#include "main.h"

U8  led_state = 0u;

void add_dist(void);

MAIN_STRUCT m =
{
     .show_value =  1u,
     .complete =    0u,
     .speed =       0u,
     .tick =        0u,
     .distance =    0u,    // Keeps track of passed distance in meters.
     .kilometer =   0u,    //Keeps track of passed distance in kilometers.
     .tick_cnt =    0u,
};

const disp_config_struct disp_conf =
{
     .port_clk = set_clk,
     .port_csb = set_cs,
     .port_rs = set_rs,
     .port_si = set_si,
     .delay_func = wait_msec
};

static char buf[64];

int main(void)
{
  m.wheel_diameter = 68u;

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  register_init();

  disp_set_conf(&disp_conf);

  disp_init ();

  disp_write_string("Speed", 0u, DISP_HIGH);
  while (1)
  {
    //Main cycle
    if (m.show_value)
    {
        m.show_value=0;
        value2string (m.speed, buf ,1,'k');
        addchar (buf,'m'); addchar (buf,'/'); addchar (buf,'h');
        addchar (buf, 0xA0); addchar (buf, 0xA0); addchar (buf, 0xA0);
        disp_write_string(buf, 6u, DISP_HIGH);
        add_dist();
        value2string (m.distance, buf, 0,'m');
        disp_write_string(buf, 0u, DISP_LOW);
    }
  }
}

void add_dist (void)
{
    unsigned long x;
    x = m.tick_cnt * (PI * m.wheel_diameter);
    x = x / 10000;
    m.distance = x;

    if (m.distance > 1000u)
    {
      m.distance = 0;
      m.kilometer++;
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
  set_led(led_state);
  m.show_value = 1u;

  if (m.tick > 400u)
  {
    m.tick = 0u;
    m.speed = 0u;
  }
}

