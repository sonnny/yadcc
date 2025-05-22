#ifndef DCC_H
#define DCC_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pulse.pio.h"

#define PREAMBLE 0xFFFE
#define SPEED_128 0x3F

void init_dcc();
void send_speed(uint8_t address, uint8_t speed);
void send_function(uint8_t address, uint8_t function);

#endif
