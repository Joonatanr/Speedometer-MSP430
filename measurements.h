/*
 * measurements.h
 *
 *  Created on: Mar 26, 2018
 *      Author: Joonatan
 */

#ifndef MEASUREMENTS_H_
#define MEASUREMENTS_H_

#include "typedefs.h"

void set_measurements_zero(void);
void push_sample(U16 sample);
U16 get_measurement_value(void);


#endif /* MEASUREMENTS_H_ */
