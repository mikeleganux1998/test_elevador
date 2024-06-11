#include <xc.h>
#include "common.h"
#include "lcd.h"
#include "motor_led.h"
#include <stdio.h> // Para utilizar sprintf
#include <string.h> // Para utilizar strlen

// Define la velocidad de la UART
#define BAUDRATE 9600
#define BRGVAL ((unsigned int)(_XTAL_FREQ/(16UL * BAUDRATE) - 1))

// Prototipo de función para inicializar la UART
void init_uart(void);

// Función para enviar una cadena a través de la UART
void uart_send_string(const char* str);

char read_keypad(void) {
    char key = '\0';
    for (int col = 0; col < 4; col++) {
        TRISB = 0xF0 | (1 << col);
        LATB = ~(1 << col);
        
        __delay_ms(20);  // Pequeño retardo para permitir la estabilización de la señal

        if (ROW1 == 0) key = '1' + col - 0;
        if (ROW2 == 0) key = '4' + col - 1;
        if (ROW3 == 0) key = '7' + col - 2;
        if (ROW4 == 0) key = '*' + col - 3;

        if (key != '\0') break;
    }
    return key;
}

void main(void) {
    TRISA = 0x0F;  // RA0-RA3 como entrada (filas del teclado)
    TRISB = 0xF0;  // RB0-RB3 como salida (columnas del teclado) y RB4-RB7 como entrada (sensores)
    TRISD = 0x00;  // Puerto D como salida (motor paso a paso y LEDs)

    lcd_init();  // Inicializar el LCD
    init_leds_and_motor();  // Inicializar LEDs y motor
    init_uart(); // Inicializa la UART para la comunicación con el Data Visualizer

    int current_level = 0;
    display_level(current_level);  // Mostrar el nivel inicial en LEDs y LCD
    
    while (1) {
        char key = read_keypad();
        
        if (key != '\0') {
            int target_level = key - '0';  // Convertir el carácter a un número
            
            if (target_level >= 0 && target_level <= 9) {
                int steps = (target_level - current_level) * 100;  // Calcular los pasos necesarios, ajusta 100 según tu motor
                step_motor(steps);  // Mover el motor
                current_level = target_level;  // Actualizar el nivel actual
                display_level(current_level);  // Mostrar el nivel actual en los LEDs y LCD
                lcd_clear();
                lcd_set_cursor(1, 1);
                lcd_write_string("Nivel: ");
                lcd_write_char(current_level + '0');

                // Envía el nivel actual a través de la UART para el Data Visualizer
                char buffer[20];
                sprintf(buffer, "Nivel: %d\r\n", current_level);
                uart_send_string(buffer);
            }
        }
    }
}



void init_uart(void) {
    SPBRG = BRGVAL;             // Establece la velocidad de la UART
    TXSTAbits.BRGH = 1;         // Velocidad alta
    RCSTAbits.SPEN = 1;         // Habilita el puerto serie
    TXSTAbits.TXEN = 1;         // Habilita la transmisión
}

void uart_send_string(const char* str) {
    while (*str) {
        while (!TXSTAbits.TRMT);    // Espera a que el buffer de transmisión esté vacío
        TXREG = *str++;             // Envía un carácter
    }
}
