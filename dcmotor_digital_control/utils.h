#include <stdint.h>
#ifndef PWM_H

#define PWM_H


#define pwm_pin1 6
#define pwm_pin2 5


//by default pinmode is INPUT no need to set
#define enc_CHA 2
#define enc_CHB 3

#define enc_counts_per_rev 1024



float rad_angle;
volatile long enc_pulses_count = 0;

uint8_t pwm(uint8_t pin, float duty_cycle, float frequency);
float square_wave(float *amplitude, float duty_cycle, float frequency);
void cmd_dc_motor(float voltage, float *min_max_limit);
void pushBack(float x_new, float *x_array, uint8_t array_size);







#endif