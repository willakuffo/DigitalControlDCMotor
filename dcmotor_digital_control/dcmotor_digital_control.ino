

#include <stdint.h>
#include "utils.h"

//discretized2nd order open loop dc motor coefficiencts
#define A0 1.0000
#define A1 -0.9983
#define A2 -0.001747
#define B0  0.02156
#define B1 0.04312
#define B2  0.02156

//simulation signals
float yk = 0, yk_1 = 0, yk_2 = 0, uk = 0, uk_1 = 0, uk_2 = 0;
float uks[3] = { 0, 0, 0 };
float yks[3] = { 0, 0, 0 };


void setup() {
  // put your setup code here, to run once:
  pinMode(pwm_pin1, OUTPUT);
  pinMode(pwm_pin2, OUTPUT);
  setup_encoder_ISR(enc_CHA, enc_CHB);


  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long start_time = micros();

  //uint8_t pwm_state = pwm(pwm_pin1,50,200);

  float max_min_amp[2] = { -5.0, 5.0 };
  uk = square_wave(max_min_amp, 50, 0.2);
  cmd_dc_motor(uk, max_min_amp);
 
  yk = (B0 / A0) * uks[0] + (B1 / A0) * uks[1] + (B2 / A0) * uks[2] - (A1 / A0) * yks[1] - (A2 / A0) * yks[2];
  pushBack(uk, uks, sizeof(uks) / sizeof(uks[0]));
  pushBack(yk, yks, sizeof(yks) / sizeof(yks[0]));

 //delay(50); 

  Serial.print(" ts[us]:");
  Serial.print(micros() - start_time);
  Serial.print(" u[k]:");
  Serial.print(uk);
  Serial.print(" y[k]-rad:");
  Serial.print(yk);
  Serial.print(" dc-rad:");
  Serial.println(((float)enc_pulses_count / (float)enc_counts_per_rev) * 2 * PI);
}
