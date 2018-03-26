/*
 * typedefs.h
 *
 *  Created on: 22. märts 2018
 *      Author: localpcadmin
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <string.h>
#include <stdio.h>

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

//////// masking
#define ISBIT(p,b)    (p & b)
#define SETBIT(p,b)   p |= b
#define CLRBIT(p,b)   p &= ~b

#define BIT_0  0x01u
#define BIT_1  0x02u
#define BIT_2  0x04u
#define BIT_3  0x08u
#define BIT_4  0x10u
#define BIT_5  0x20u
#define BIT_6  0x40u
#define BIT_7  0x80u

#define Private static

#endif /* TYPEDEFS_H_ */
