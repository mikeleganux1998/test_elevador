#ifndef MOTOR_LED_H
#define MOTOR_LED_H

#define STEP1 LATDbits.LATD0
#define STEP2 LATDbits.LATD1
#define STEP3 LATDbits.LATD2
#define STEP4 LATDbits.LATD3

void init_leds_and_motor(void);
void step_motor(int steps);
void display_level(int level);

#endif  // MOTOR_LED_H
