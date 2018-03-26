/*****main.c*******/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//////// masking
#define ISBIT(p,b)    (p & b)
#define SETBIT(p,b)   p |= b
#define CLRBIT(p,b)   p &= ~b

#define PI              314

typedef struct __MAIN_STRUCT
{
    U8  show_value;
    U32 rpm;
}MAIN_STRUCT;

extern MAIN_STRUCT m;

