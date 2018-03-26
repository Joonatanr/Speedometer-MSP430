/*
 * measurements.c
 *
 *  Created on: Mar 26, 2018
 *      Author: Joonatan
 */

#include "measurements.h"

#define NUMBER_OF_SAMPLES 6u

/* TODO : Should replace this with a fifo... */
Private U16 samples_array[NUMBER_OF_SAMPLES];
Private U8  curr_ix = 0u;


void push_sample(U16 sample)
{
    samples_array[curr_ix++] = sample;

    if (curr_ix >= NUMBER_OF_SAMPLES)
    {
        curr_ix = 0u;
    }
}

U16 get_measurement_value(void)
{
    U32 avg = 0u;

    U8 ix;

    U16 min = 0xffffu;
    U16 max = 0u;

    for (ix = 0u; ix < NUMBER_OF_SAMPLES; ix++)
    {
        if (samples_array[ix] > max)
        {
            max = samples_array[ix];
        }
        else if (samples_array[ix] < min)
        {
            min = samples_array[ix];
        }

        avg += samples_array[ix];
    }

#if NUMBER_OF_SAMPLES > 2u
    /* We remove 2 of the worst samples. */
    avg -= max;
    avg -= min;
    avg = avg / (NUMBER_OF_SAMPLES - 2u);
#else
    avg = avg / NUMBER_OF_SAMPLES;
#endif

    return avg;
}



