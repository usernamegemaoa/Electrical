#ifndef BLINK_LED_H
#define	BLINK_LED_H

#include <xc.h> // include processor files - each processor file is guarded.  

void led_blink(int led_temp);    /* blinks LED for 1 sec */

void led_blink_time(int led_temp, int time); /* blinks LED for input amount of time */

void led_on(int led_temp);    /* An example function declaration */

void led_off(int led_temp);    /* An example function declaration */

void hall_LED(void);

void hall_LED_state(void);


#endif	/* BLINK_LED_H */