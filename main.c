#include "msp430g2553.h"
#include "display.h"
#include "register.h"
#include "misc.h"
#include "main.h"

typedef struct
{
    U16 meters;
    U16 kilometer;
} Distance_T;

static void add_dist(void);

MAIN_STRUCT m =
{
     .show_value =  1u,    // Set to 1, so we update at the start.
     .complete =    0u,
     .speed =       0u,
     .tick =        0u,
};

static Distance_T myDistance =
{
    .meters =      0u,    // Keeps track of passed distance in meters.
    .kilometer =   0u,    //Keeps track of passed distance in kilometers.
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
static U8  led_state = 0u;

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
        value2string (myDistance.meters, buf, 0,'m');
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
  set_led(led_state);

  /* So this updates the speed sensor automatically? TODO : Review, it might be unnecessary */
  m.show_value = 1u;

  if (m.tick > 400u)
  {
    m.tick = 0u;
    m.speed = 0u;
  }
}


static void add_dist (void)
{
    U32 x;
    x = tick_total_count * (PI * m.wheel_diameter);
    x = x / 10000;
    myDistance.meters = x;

    if (myDistance.meters > 1000u)
    {
      myDistance.meters = 0;
      myDistance.kilometer++;
    }
}

