#include <xc.h>
#include <stdio.h>
#include "lcd.h"
#include "keypad.h"

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

// Definición de pines para el motor a pasos
#define STEP LATB0
#define DIR LATB1

// Prototipos de funciones
void initMotor(void);
void moveMotor(int steps, int direction);
void initUART(void);
void sendUART(const char* data);

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
            int level = key - '0'; // Convertir carácter a número
            moveMotor(level * 100, 1); // Ejemplo: 100 pasos por nivel
        }
    }
}

// Funciones para el motor a pasos
void initMotor(void) {
    TRISBbits.TRISB0 = 0; // Configurar pin STEP como salida
    TRISBbits.TRISB1 = 0; // Configurar pin DIR como salida
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
    TRISCbits.TRISC6 = 1; // Configurar pin TX como entrada
    TRISCbits.TRISC7 = 1; // Configurar pin RX como entrada
    SPBRG = 129; // Baud rate 9600 para 20MHz
    TXSTAbits.BRGH = 1;
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
}

void sendUART(const char* data) {
    while (*data) {
        while (!TXSTAbits.TRMT); // Esperar a que el registro esté vacío
        TXREG = *data++;
    }
}
