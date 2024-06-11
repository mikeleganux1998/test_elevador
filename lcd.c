#include <xc.h>
#include "common.h"
#include "lcd.h"

#define _XTAL_FREQ 8000000  // Frecuencia del oscilador

#define RS PORTCbits.RC0
#define EN PORTCbits.RC1
#define D4 PORTCbits.RC2
#define D5 PORTCbits.RC3
#define D6 PORTCbits.RC4
#define D7 PORTCbits.RC5

void lcd_command(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_strobe(void);

void lcd_init(void) {
    TRISC = 0x00;  // Puerto C como salida para el LCD
    __delay_ms(20);  // Tiempo de inicio para el LCD

    lcd_command(0x02);  // Modo de 4 bits
    lcd_command(0x28);  // Modo de 2 líneas, 5x7 matriz
    lcd_command(0x0C);  // Encender el LCD, sin cursor
    lcd_command(0x06);  // Incrementar el cursor
    lcd_command(0x01);  // Limpiar el LCD
    __delay_ms(2);
}

void lcd_clear(void) {
    lcd_command(0x01);  // Limpiar el LCD
    __delay_ms(2);  // Tiempo para la limpieza del LCD
}

void lcd_set_cursor(int row, int col) {
    int address = 0;
    if (row == 1) {
        address = 0x80 + col - 1;
    } else if (row == 2) {
        address = 0xC0 + col - 1;
    }
    lcd_command(address);
}

void lcd_write_string(const char* str) {
    while (*str) {
        lcd_data(*str++);
    }
}

void lcd_write_char(char c) {
    lcd_data(c);
}

void lcd_command(unsigned char cmd) {
    RS = 0;  // Comando
    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    lcd_strobe();
    D4 = cmd & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    lcd_strobe();
}

void lcd_data(unsigned char data) {
    RS = 1;  // Datos
    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    lcd_strobe();
    D4 = data & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    lcd_strobe();
}

void lcd_strobe(void) {
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(1);
}
