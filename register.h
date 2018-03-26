/*
 * register.h
 *
 *  Created on: 26. märts 2018
 *      Author: localpcadmin
 */

#ifndef REGISTER_H_
#define REGISTER_H_

#include "typedefs.h"

/* Globally used variables */
extern volatile U8 redraw_display_measurement_flag;
extern volatile U32 rotation_total_count; /* This variable contains the total number of ticks that have been recorded. */

/* Public functions */
extern void register_init(void);
extern U16 get_rpm_measurement(void);

extern void set_backlight (U8 b);
extern void set_si        (U8 b);
extern void set_clk       (U8 b);
extern void set_rs        (U8 b);
extern void set_cs        (U8 b);

extern void set_led       (U8 b);
extern void set_led2      (U8 b);

extern U8 isSensor(void);

extern void wait20mksec (U16 i);
extern void wait_msec (U16 time);


#endif /* REGISTER_H_ */
