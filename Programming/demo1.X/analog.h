#ifndef ANALOG_H
#define	ANALOG_H

#include <xc.h>

void analog_init(int analog_port);
// Reads the state of each of the hall effect sensors.

unsigned int get_voltage(int port);
// Returns a 10-bit voltage value that is between 0V - 5V

unsigned int read_current(void);
// Returns the current through the motor as a value between 0 - 50 Amps

unsigned int read_phase1(void);
// Returns the voltage reading through Phase 1

unsigned int read_phase2(void);
// Returns the voltage reading through Phase 2

unsigned int read_phase3(void);
// Returns the voltage reading through Phase 3


#endif	/* HALL_EFFECT_H */

#endif	/* ANALOG_H */

