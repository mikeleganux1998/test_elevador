#include <xc.h>
#include "keypad.h"

void initKeypad(void) {
    TRISC = 0xF0; // Columnas (C0-C3) como salidas, filas (C4-C7) como entradas
    PORTC = 0x00;
}

char readKeypad(void) {
    const char keymap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (int col = 0; col < 4; col++) {
        LATC = ~(1 << col); // Enciende la columna actual
        __delay_ms(10);

        for (int row = 0; row < 4; row++) {
            if (!(PORTC & (1 << (row + 4)))) {
                return keymap[row][col];
            }
        }
    }
    return '\0'; // No se presionó ninguna tecla
}
