#ifndef HALL_EFFECT_H
#define	HALL_EFFECT_H

#include <xc.h>

void read_hall(int *hall_A, int *hall_B, int *hall_C);
// Reads the state of each of the hall effect sensors.

int get_hall_state(void);
// Returns the numerical state corresponding to the Hall effect sensor position

int get_direction(void);
// Returns 0,1,2 corresponding to the position of the motor. 

#endif	/* HALL_EFFECT_H */

