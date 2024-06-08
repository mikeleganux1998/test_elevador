#include <xc.h>
#include <stdio.h>

// Configuración del PIC18F4550
#pragma config FOSC = HS
#pragma config PWRT = ON
#pragma config BOR = ON
#pragma config BORV = 3
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config XINST = OFF
#pragma config CP0 = OFF
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config CP3 = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF

#define _XTAL_FREQ 20000000  // Frecuencia del oscilador

// Definición de pines (ajusta según tu configuración)
#define RS LATD0
#define EN LATD1
#define D4 LATD2
#define D5 LATD3
#define D6 LATD4
#define D7 LATD5

#define STEP LATB0
#define DIR LATB1

// Prototipos de funciones
void initLCD(void);
void lcdCommand(unsigned char cmd);
void lcdData(unsigned char data);
void lcdPrint(const char* str);
void initKeypad(void);
char readKeypad(void);
void initMotor(void);
void moveMotor(int steps, int direction);
void initUART(void);
void sendUART(char* data);

// Función principal
void main(void) {
    // Inicializaciones
    initLCD();
    initKeypad();
    initMotor();
    initUART();

    lcdPrint("Elevator Ready");

    while (1) {
        char key = readKeypad();

        if (key != '\0') {
            lcdCommand(0x01); // Limpiar pantalla
            lcdPrint("Nivel: ");
            lcdData(key);

            // Enviar nivel por UART
            char msg[16];
            sprintf(msg, "Nivel: %c\n", key);
            sendUART(msg);

            // Mover motor a pasos
            int level = key - '0';
            moveMotor(level * 100, 1); // Ejemplo: 100 pasos por nivel
        }
    }
}

// Funciones para el LCD
void initLCD(void) {
    // Inicialización del LCD (4 bits)
    TRISD = 0x00;
    __delay_ms(20);
    lcdCommand(0x02);
    lcdCommand(0x28);
    lcdCommand(0x0C);
    lcdCommand(0x06);
    lcdCommand(0x01);
}

void lcdCommand(unsigned char cmd) {
    RS = 0;
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
    RS = 1;
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

// Funciones para el teclado matricial
void initKeypad(void) {
    TRISC = 0xF0; // Columnas como salidas (C0-C3), filas como entradas (C4-C7)
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
        LATC = ~(1 << col);
        __delay_ms(10);

        for (int row = 0; row < 4; row++) {
            if (!(PORTC & (1 << (row + 4)))) {
                return keymap[row][col];
            }
        }
    }
    return '\0';
}

// Funciones para el motor a pasos
void initMotor(void) {
    TRISBbits.TRISB0 = 0; // STEP pin
    TRISBbits.TRISB1 = 0; // DIR pin
}

void moveMotor(int steps, int direction) {
    DIR = direction;
    for (int i = 0; i < steps; i++) {
        STEP = 1;
        __delay_ms(1);
        STEP = 0;
        __delay_ms(1);
    }
}

// Funciones para la UART
void initUART(void) {
    TRISCbits.TRISC6 = 1; // TX pin
    TRISCbits.TRISC7 = 1; // RX pin
    SPBRG = 129; // Baud rate 9600 para 20MHz
    TXSTAbits.BRGH = 1;
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
}

void sendUART(char* data) {
    while (*data) {
        while (!TXSTAbits.TRMT);
        TXREG = *data++;
    }
}
