#include <xc.h>
#include "common.h"
#include "motor_led.h"

// Inicialización de pines para LEDs y motor
void init_leds_and_motor(void) {
    TRISD = 0x00;  // Puerto D como salida (motor paso a paso y LEDs)
    // Asegurarse de que los pines del motor y los LEDs estén apagados inicialmente
    LATD = 0x00;
}

// Función para mover el motor paso a paso
void step_motor(int steps) {
    int direction = (steps > 0) ? 1 : -1;
    if (steps < 0) {
        steps = -steps;
    }

    for (int i = 0; i < steps; i++) {
        if (direction > 0) {
            // Secuencia para mover el motor hacia adelante
            STEP1 = 1; STEP2 = 0; STEP3 = 0; STEP4 = 0; __delay_ms(10);
            STEP1 = 0; STEP2 = 1; STEP3 = 0; STEP4 = 0; __delay_ms(10);
            STEP1 = 0; STEP2 = 0; STEP3 = 1; STEP4 = 0; __delay_ms(10);
            STEP1 = 0; STEP2 = 0; STEP3 = 0; STEP4 = 1; __delay_ms(10);
        } else {
            // Secuencia para mover el motor hacia atrás
            STEP1 = 0; STEP2 = 0; STEP3 = 0; STEP4 = 1; __delay_ms(10);
            STEP1 = 0; STEP2 = 0; STEP3 = 1; STEP4 = 0; __delay_ms(10);
            STEP1 = 0; STEP2 = 1; STEP3 = 0; STEP4 = 0; __delay_ms(10);
            STEP1 = 1; STEP2 = 0; STEP3 = 0; STEP4 = 0; __delay_ms(10);
        }
    }
}

// Función para mostrar el nivel actual en LEDs y LCD
void display_level(int level) {
    // Mostrar el nivel actual en LEDs (ejemplo simple, ajustar según tu hardware)
    LATD = (level & 0x0F);  // Usar 4 LEDs para mostrar el nivel (nivel de 0 a 15)
}
