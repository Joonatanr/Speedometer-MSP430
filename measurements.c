/*
 * measurements.c
 *
 *  Created on: Mar 26, 2018
 *      Author: Joonatan
 */

#include "measurements.h"
#include "msp430g2553.h"

#define NUMBER_OF_SAMPLES_TOTAL     8u
#define NUMBER_OF_SAMPLES_HIGH      8u
#define NUMBER_OF_SAMPLES_MEDIUM    4u
#define NUMBER_OF_SAMPLES_LOW       1u

#define HI_THRESHOLD                240u //We begin the high sample rate at   240 RPM
#define MED_THRESHOLD               120u //We begin the medium sample rate at 120RPM

#define ENTER_CRITICAL_SECTION() _DINT()
#define EXIT_CRITICAL_SECTION() _EINT()

typedef struct __measurement__
{
    U16 value;
    struct __measurement__ * next;
} measurement_T;

static U8 get_samples(U8 number_of_samples, U16 * dest);


Private measurement_T samples_array[NUMBER_OF_SAMPLES_TOTAL];
Private measurement_T * priv_head_ptr;

Private U8  curr_ix = 0u; /* Direct memory index... */

/***************************** public function definitions *********************************/
void push_sample(U16 sample)
{
    samples_array[curr_ix].value = sample;
    samples_array[curr_ix].next = priv_head_ptr;
    priv_head_ptr = &samples_array[curr_ix];

    curr_ix++;
    if (curr_ix >= NUMBER_OF_SAMPLES_TOTAL)
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

    U16 last_measurement;
    U8  number_of_samples;
    U16 samples[NUMBER_OF_SAMPLES_TOTAL];

    /* 1. We get the latest measurement value */
    if (priv_head_ptr != NULL)
    {
        last_measurement = priv_head_ptr->value;
    }
    else
    {
        return 0u;
    }

    /* 2. Determine if we are moving at lo, medium or high speed. */
    if (last_measurement >= HI_THRESHOLD)
    {
        number_of_samples = NUMBER_OF_SAMPLES_HIGH;
    }
    else if (last_measurement >= MED_THRESHOLD)
    {
        number_of_samples = NUMBER_OF_SAMPLES_MEDIUM;
    }
    else
    {
        number_of_samples = NUMBER_OF_SAMPLES_LOW;
    }

    /* 3. Retrieve the desired amount of samples. */
    number_of_samples = get_samples(number_of_samples, samples);

    for (ix = 0u; ix < number_of_samples; ix++)
    {
        if (samples[ix] > max)
        {
            max = samples[ix];
        }
        else if (samples[ix] < min)
        {
            min = samples[ix];
        }

        avg += samples[ix];
    }

    /* 4. Apply filter if applicable. */
    if (number_of_samples > 2u)
    {
        /* We remove 2 of the worst samples. */
        avg -= max;
        avg -= min;
        number_of_samples -= 2u;
    }

    avg = avg / number_of_samples;

    return avg;

}


/****************  Private function definitions ********************/

static U8 get_samples(U8 number_of_samples, U16 * dest)
{
    /* Get the last N samples */
    /* Returns number of samples that were successfully retrieved */

    U8 res = 0u;
    measurement_T * sample_ptr;

    ENTER_CRITICAL_SECTION();
    sample_ptr = priv_head_ptr;
    while((sample_ptr != NULL) && (res < number_of_samples))
    {
        dest[res] = sample_ptr->value;
        res++;
        sample_ptr = sample_ptr->next;
    }
    EXIT_CRITICAL_SECTION();

    return res;
}

