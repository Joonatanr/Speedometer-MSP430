/*
 * misc.h
 *
 *  Created on: 26. märts 2018
 *      Author: localpcadmin
 */

#ifndef MISC_H_
#define MISC_H_

#include "typedefs.h"

void long2string (long nr, char *s);
U8 addchar (char *s, char c);
void format_volt (U16 volt, char *s, U8 dec, char unit);

#endif /* MISC_H_ */
