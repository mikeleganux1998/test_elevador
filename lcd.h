#ifndef LCD_H
#define LCD_H

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(int row, int col);
void lcd_write_string(const char* str);
void lcd_write_char(char c);

#endif  // LCD_H
