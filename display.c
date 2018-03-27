/*
 * display.c
 *
 *  Created on: 17.01.2016
 *      Author: Joonatan
 */

#include "display.h"
#include "string.h"


////Display driver for EA-DOG162, designed to work with software SPI.
#define DL_MODE                 (U8)0x01u
#define NUMBER_OF_LINES_MODE    (U8)0x01u
#define DH_MODE                 (U8)0x00u

const disp_config_struct * priv_disp_conf_ptr;

#define DELAY_MS(t) (priv_disp_conf_ptr->delay_func((U16)(t)))


/*Private scope forward declarations. */

static void disp_function_set   (U8 dl,U8 n,U8 f, U8 it);
static void disp_on_off_set     (U8 d, U8 c, U8 b);
static void DDR_address_set     (U8 address);
static void write_charcode      (U8 code);
static void execute_LCD         (U8 register_select, U8 data);



/*********************************/
/* Public function declarations  */
/*********************************/

void disp_set_conf (const disp_config_struct * conf)
{
    /* This function must be called before initing display. */
    priv_disp_conf_ptr = conf;
}

void disp_init (void)
{
    priv_disp_conf_ptr->port_csb(1);
    DELAY_MS(240u);
    execute_LCD (0x00u,0x38u);
    DELAY_MS(20u);
    disp_function_set (DL_MODE,NUMBER_OF_LINES_MODE,DH_MODE,1u);
    DELAY_MS(20u);
    execute_LCD (0x00u,0x14u);
    DELAY_MS (20u);
    execute_LCD (0x00u,0x55u);
    DELAY_MS(20u);
    execute_LCD (0x00u,0x6Du);
    DELAY_MS(200);
    execute_LCD (0x00u,0x78u);
    DELAY_MS(20u);

    disp_function_set (DL_MODE,NUMBER_OF_LINES_MODE,DH_MODE,0u);
    DELAY_MS(20u);

    disp_on_off_set (1u,0u,0u);
    DELAY_MS(20u);
    execute_LCD (0x00u,0x01u);
    DELAY_MS(20u);
    execute_LCD (0x00u,0x06u);
}

void disp_reset(void)
{
    execute_LCD (0u,0x01u);
    DELAY_MS(300u); //wait for display to stabilize.
}


void disp_write_char (char c,U8 address, U8 line)
{
   //Funktsiooni idee on see, et kirjuta see character sinna kohta, ilma displayd clearimata
   /*write_disp_mem (c,address);*/
   if (address > 15u)
   {
       return;
   }
   if (line == DISP_LOW)
   {
       address += 64u;
   }

   DDR_address_set(address);
   write_charcode(c);
}


void disp_empty_char (U8 address, U8 line)
{
   disp_write_char(0xA0u, address, line);
}


void disp_write_string (const char * string, U8 begin, U8 line)
{
    U8 end = (strlen (string)) + begin;
    U8 i = begin;
    U8 str_pos = 0u;

    if (end > 16u)
    {
        end = 16u;
    }
    for (i = begin; i < end; i++)
    {
        disp_write_char(string[str_pos++],i, line);
    }
#ifdef PADDING_ENABLED
    for (; i < 16u; i++)
    {
        disp_empty_char (i, line);
    }
#endif
}


void disp_clear_low (void)
{
    U8 i;
    for(i = 0u; i < 16u; i++)
    {
        disp_empty_char(i + 64u, DISP_LOW);
    }
}


void disp_clear_high(void)
{
   U8 i;
   for(i = 0u; i < 16u; i++)
   {
       disp_empty_char(i, DISP_HIGH);
   }
 }


/**********************************/
/* Private function declarations  */
/**********************************/

static void execute_LCD (U8 register_select, U8 data)
{
    U8 x;
    U8 abi = 0x80;
    priv_disp_conf_ptr->port_rs(register_select);

    priv_disp_conf_ptr->port_csb(0u);
    for (x = 0u; x < 8u; x++)
    {
      priv_disp_conf_ptr->port_clk(0u);
      priv_disp_conf_ptr->port_si(data & abi);
      abi = abi >> 1u;

      priv_disp_conf_ptr->port_clk(1u);
    }
}


static void DDR_address_set (U8 address)
{
   address = address | 0x80;
   execute_LCD (0u,address);
}


static void write_charcode (U8 code)
{
  execute_LCD (1u,code);
}


static void disp_function_set (U8 dl,U8 n,U8 f, U8 it)
{
  //n - line number, f - font
  U8 cmnd = 0x20u;
  if (it > 0x02u) {return;}
  cmnd += dl << 4u;
  cmnd += n  << 3u;
  cmnd += f  << 2u;
  cmnd += it;
  execute_LCD (0u, cmnd);
}


void disp_shutdown (void)
{
    disp_on_off_set (0u,0u,0u);
}


static void disp_on_off_set (U8 d, U8 c, U8 b)
{
    U8 cmnd = 0x00u;
    cmnd += 0x08u;
    cmnd += d << 2u;
    cmnd += c<< 1u;
    cmnd += b;
    execute_LCD (0u,cmnd);
}


