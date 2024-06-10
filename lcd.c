#include <xc.h>
#include "lcd.h"

#define _XTAL_FREQ 20000000  // Frecuencia del oscilador

// Definición de pines para el LCD
#define RS LATD0
#define EN LATD1
#define D4 LATD2
#define D5 LATD3
#define D6 LATD4
#define D7 LATD5

void initLCD(void) {
    // Inicialización del LCD en modo de 4 bits
    TRISD = 0x00; // Configurar el puerto D como salida
    __delay_ms(20);
    lcdCommand(0x02);
    lcdCommand(0x28);
    lcdCommand(0x0C);
    lcdCommand(0x06);
    lcdCommand(0x01);
}

void lcdCommand(unsigned char cmd) {
    RS = 0; // Modo comando
    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(200);
    D4 = cmd & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_ms(2);
}

void lcdData(unsigned char data) {
    RS = 1; // Modo datos
    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(200);
    D4 = data & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_ms(2);
}

void lcdPrint(const char* str) {
    while (*str) {
        lcdData(*str++);
    }
}
