/*
 * utils.h
 *
 *  Created on: 04/12/2023
 *      Author: micron
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal_data.h"

#ifndef UTILS_H_
#define UTILS_H_
typedef enum
{
   red = 1,
   green = 2,
   blue = 3,
} led_state_t;

void led_update(led_state_t led_state, e_bsp_io_level value);
void led_blink(led_state_t led_state, int times);
void led_blink(int times);

void error_blink(int times);

#endif /* UTILS_H_ */
