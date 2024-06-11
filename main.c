#include <xc.h>
#include "common.h"
#include "lcd.h"
#include "motor_led.h"

#define _XTAL_FREQ 8000000  // Frecuencia del oscilador

// Configuración de los fusibles para el PIC18F4550
#pragma config FOSC = HSPLL_HS
#pragma config PLLDIV = 5
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 2
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = ON
#pragma config BOR = ON
#pragma config BORV = 3
#pragma config VREGEN = ON
#pragma config WDT = OFF
#pragma config WDTPS = 32768
#pragma config CCP2MX = ON
#pragma config PBADEN = OFF
#pragma config LPT1OSC = OFF
#pragma config MCLRE = ON
#pragma config STVREN = ON
#pragma config LVP = OFF
#pragma config ICPRT = OFF
#pragma config XINST = OFF
#pragma config DEBUG = OFF

// Definiciones para el teclado matricial
#define ROW1 PORTAbits.RA0
#define ROW2 PORTAbits.RA1
#define ROW3 PORTAbits.RA2
#define ROW4 PORTAbits.RA3
#define COL1 PORTBbits.RB0
#define COL2 PORTBbits.RB1
#define COL3 PORTBbits.RB2
#define COL4 PORTBbits.RB3

// Prototipos de funciones
char read_keypad(void);

void main(void) {
    TRISA = 0x0F;  // RA0-RA3 como entrada (filas del teclado)
    TRISB = 0xF0;  // RB0-RB3 como salida (columnas del teclado) y RB4-RB7 como entrada (sensores)
    
    lcd_init();  // Inicializar el LCD
    init_leds_and_motor();  // Inicializar LEDs y motor

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
            }
        }
    }
}

char read_keypad(void) {
    char key = '\0';
    for (int col = 0; col < 4; col++) {
        TRISB = 0xF0 | (1 << col);
        LATB = ~(1 << col);
        
        __delay_ms(20);  // Pequeño retardo para permitir la estabilización de la señal

        if (ROW1 == 0) key = '1' + col;
        if (ROW2 == 0) key = '4' + col;
        if (ROW3 == 0) key = '7' + col;
        if (ROW4 == 0) key = '*' + col;

        if (key != '\0') break;
    }
    return key;
}
