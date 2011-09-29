#ifndef _LCD_H_
#define _LCD_H_

#define LCD_DATA_PORT       2

#define LCD_ENABLE_PORT     3
#define LCD_ENABLE_PIN      0

#define LCD_RW_PORT         3
#define LCD_RW_PIN          1

#define LCD_RS_PORT         3
#define LCD_RS_PIN          2

#define LCDC_CLEAR          ((1 << 0))
#define LCDC_RETURN         ((1 << 1))
#define LCDC_ON_CURSOR      ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))
#define LCDC_ON             ((1 << 3) | (1 << 2) | (1 << 1))
#define LCDC_OFF            ((1 << 3))
#define LCDC_RESET          ((1 << 5) | (1 << 4))
#define LCDC_FUNC_SET       ((1 << 5) | (1 << 4) | (1 << 3))
#define LCDC_EMS            ((1 << 2) | (1 << 1))
#define LCDC_SET_DDRAM      ((1 << 7))

#define LCD_ROW_LENGTH      20
#define LCD_ROWS            2
#define LCD_ROW_MEM_OFFSET  0x40

void lcdInit(void);
void lcdReset(void);
void lcdPutc(uint8_t outbyte);
void lcdPuts(char *str, int loc, int row);

#endif //_LCD_H_
