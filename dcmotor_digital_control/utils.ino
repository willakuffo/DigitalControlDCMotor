#include <stdint.h>
#include "utils.h"


uint8_t pwm(uint8_t pin, float duty_cycle, float frequency) {
  unsigned long pwm_period = (1 / frequency) * 1e6;
  unsigned long current_time = micros();
  static unsigned long start_time = current_time;
  static uint8_t pwm_state;

  unsigned long on_duration = (duty_cycle / 100) * pwm_period;
  unsigned long off_duration = pwm_period - on_duration;


  //(current_time - )%pwm_period >= 0? trigger_off_state = 1: trigger_on_state = 0;
  switch (pwm_state) {
    case 0:

      if ((current_time - start_time) >= off_duration) {
        digitalWrite(pin, HIGH);
        pwm_state = 1;
        start_time = current_time;
      }

      break;
    case 1:
      if ((current_time - start_time) >= on_duration) {
        digitalWrite(pin, LOW);
        pwm_state = 0;
        start_time = current_time;
      }
      break;
  }

  return pwm_state;
}

float square_wave(float *amplitude, float duty_cycle, float frequency) {
  unsigned long pwm_period = (1 / frequency) * 1e6;
  unsigned long current_time = micros();
  static unsigned long start_time = current_time;
  static float signal;
  static uint8_t sig_state;

  unsigned long on_duration = (duty_cycle / 100) * pwm_period;
  unsigned long off_duration = pwm_period - on_duration;


  //(current_time - )%pwm_period >= 0? trigger_off_state = 1: trigger_on_state = 0;
  sig_state = signal > (amplitude[0]+amplitude[1])/2;
  switch (sig_state) {
    case 0:

      if ((current_time - start_time) >= off_duration) {
        signal = amplitude[1];
        start_time = current_time;
      }

      break;
    case 1:
      if ((current_time - start_time) >= on_duration) {
        signal = amplitude[0];
        start_time = current_time;
      }
      break;
  }

  return signal;
}

void cmd_dc_motor(float voltage,float *min_max_limit) {
  uint8_t direction = voltage > 0 ? 1 : 0;
  switch (direction) {
    case 0:
      analogWrite(pwm_pin1,abs(voltage/min_max_limit[1])*255);
      //pwm(pwm_pin1, 100, 100);
      analogWrite(pwm_pin2, 0);

      break;

    case 1:
      analogWrite(pwm_pin2, abs(voltage/min_max_limit[0])*255);
      //pwm(pwm_pin1, 100, 100);
      analogWrite(pwm_pin1, 0);
      break;
  }
}

void pushBack(float x_new, float *x_array, uint8_t array_size) {
  for (uint8_t i = array_size - 1; i > 0; i = i - 1) {
    x_array[i] = x_array[i - 1];
  }

  x_array[0] = x_new;
};


void setup_encoder_ISR(uint8_t enc_pinA, uint8_t enc_pinB) {
  attachInterrupt(digitalPinToInterrupt(enc_pinA), CHA_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(enc_pinB), CHB_ISR, RISING);
}

void CHA_ISR() {
  digitalRead(enc_CHB) ? enc_pulses_count += 0 : enc_pulses_count += 1;
}


void CHB_ISR() {
  digitalRead(enc_CHA) ? enc_pulses_count += 0 : enc_pulses_count -= 1;
}
