/*
 * display_drv.h
 *
 *  Created on: 22. märts 2018
 *      Author: localpcadmin
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_


#include "typedefs.h"

#define DISP_HIGH 0
#define DISP_LOW  1

#define PADDING_ENABLED


typedef void (*disp_port_func)(unsigned char b);
typedef void (*disp_clk_func)(U16 t);

typedef struct
{
    disp_port_func port_csb;
    disp_port_func port_rs;
    disp_port_func port_si;
    disp_port_func port_clk;

    disp_clk_func delay_func;
}disp_config_struct;


void disp_set_conf (const disp_config_struct * conf);
void disp_init (void);
void disp_write_char (char c,U8 address, U8 line);
void disp_empty_char (U8 address, U8 line);
void disp_write_string (const char * string, U8 begin, U8 line);

void disp_clear_low     (void);
void disp_clear_high    (void);
void disp_shutdown      (void);



#endif /* DISPLAY_H_ */
