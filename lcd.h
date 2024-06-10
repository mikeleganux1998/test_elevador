#ifndef LCD_H
#define LCD_H

#define _XTAL_FREQ 20000000  // Frecuencia del oscilador

void initLCD(void);
void lcdCommand(unsigned char cmd);
void lcdData(unsigned char data);
void lcdPrint(const char* str);

#endif
