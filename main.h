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
    U16 tick; // to count 10msec intervals.
    U8  complete;
    U32 speed; //20.0 km/h - 200
    U32 wheel_diameter; //Currently set to 68 cm.
}MAIN_STRUCT;

extern MAIN_STRUCT m;

/****main.c****/
void timer_10msec (void);
void timer_1sec (void);

