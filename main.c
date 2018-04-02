#include "msp430g2553.h"
#include "display.h"
#include "register.h"
#include "misc.h"
#include "measurements.h"

volatile U8 redraw_display_measurement_flag = 0u; /* External global variable, set by register.c */

const disp_config_struct disp_conf =
{
     .port_clk = set_clk,
     .port_csb = set_cs,
     .port_rs = set_rs,
     .port_si = set_si,
     .delay_func = wait_msec
};

static char buf[32];

int main(void)
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  register_init();

  disp_set_conf(&disp_conf);

  set_measurements_zero();

  disp_init ();

  set_backlight(1u);

  disp_write_string("Speed", 0u, DISP_HIGH);
  while (1)
  {
    //Main cycle
    if (redraw_display_measurement_flag == 1u)
    {
        redraw_display_measurement_flag = 0;

        /*value2string (get_rpm_measurement(), buf ,0 ,'R');*/
        value2string (get_measurement_value(), buf ,0 ,' ');

        addchar (buf,'R');
        addchar (buf,'P');
        addchar (buf,'M');

        /* Add blanks to the end. */
        addchar (buf, 0xA0);
        addchar (buf, 0xA0);
        addchar (buf, 0xA0);

        disp_write_string(buf, 6u, DISP_HIGH);

        long2string(rotation_total_count, buf);
        disp_write_string(buf, 0u, DISP_LOW);
    }
  }
}

