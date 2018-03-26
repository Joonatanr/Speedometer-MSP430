/*
 * uart.h
 *
 *  Created on: 26. märts 2018
 *      Author: localpcadmin
 */

#ifndef UART_H_
#define UART_H_



/*****uart.c******/

extern void uart_init (void);
extern void x1_string (char *ps);
extern int  x1_handle_data (void);
extern void x1 (char c);
extern void x1_number(int n);
extern void x1_rn (void);


#endif /* UART_H_ */
