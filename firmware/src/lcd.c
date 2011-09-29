#include <scandal/engine.h>
#include <scandal/message.h>
#include <scandal/leds.h>
#include <scandal/utils.h>
#include <scandal/uart.h>
#include <scandal/stdio.h>
#include <scandal/wdt.h>

#include <string.h>

#include <arch/timer.h>
#include <arch/gpio.h>

#include <project/lcd.h>

//We just blurt over port 3 to get this done
//#define LCD_EN	0x1
//#define LCD_DEN 0x5
//#define LCD_IDLE 0x0 	

static void lcdInstr(uint8_t outbyte);


#define US_COUNT 4
//delay for GREATER than us us
//Nasty and not microseconds!
static inline void LCD_Delay(int us){
    volatile int i;
    for(i = 0; i < us * US_COUNT; i++);
}

void lcdInstr(uint8_t outbyte){
    LPC_GPIO[LCD_DATA_PORT]->DATA = outbyte;
    GPIO_SetValue(LCD_RS_PORT, LCD_RS_PIN, 0);
	GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 1);
    LCD_Delay(2);
    GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
    scandal_delay(1);
}

void lcdPutc(uint8_t outbyte){
    LPC_GPIO[LCD_DATA_PORT]->DATA = outbyte;
    GPIO_SetValue(LCD_RS_PORT, LCD_RS_PIN, 1);
	GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 1);
    LCD_Delay(2);
    GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
    GPIO_SetValue(LCD_RS_PORT, LCD_RS_PIN, 0);
    scandal_delay(1);
}

void lcdInit(void){
    GPIO_SetFunction(LCD_DATA_PORT, 0, GPIO_PIO, GPIO_MODE_NONE);
    GPIO_SetFunction(LCD_DATA_PORT, 1, GPIO_PIO, GPIO_MODE_NONE);
    GPIO_SetFunction(LCD_DATA_PORT, 2, GPIO_PIO, GPIO_MODE_NONE);
    GPIO_SetFunction(LCD_DATA_PORT, 3, GPIO_PIO, GPIO_MODE_NONE);
    GPIO_SetFunction(LCD_DATA_PORT, 4, GPIO_PIO, GPIO_MODE_NONE);
    GPIO_SetFunction(LCD_DATA_PORT, 5, GPIO_PIO, GPIO_MODE_NONE);
    GPIO_SetFunction(LCD_DATA_PORT, 6, GPIO_PIO, GPIO_MODE_NONE);
    GPIO_SetFunction(LCD_DATA_PORT, 7, GPIO_PIO, GPIO_MODE_NONE);
	GPIO_SetDir(LCD_DATA_PORT, 0, 1);
	GPIO_SetDir(LCD_DATA_PORT, 1, 1);
	GPIO_SetDir(LCD_DATA_PORT, 2, 1);
	GPIO_SetDir(LCD_DATA_PORT, 3, 1);
	GPIO_SetDir(LCD_DATA_PORT, 4, 1);
	GPIO_SetDir(LCD_DATA_PORT, 5, 1);
	GPIO_SetDir(LCD_DATA_PORT, 6, 1);
	GPIO_SetDir(LCD_DATA_PORT, 7, 1);
	LPC_GPIO[LCD_DATA_PORT]->DATA = 0x0;
	
    GPIO_SetFunction(LCD_ENABLE_PORT, LCD_ENABLE_PIN, GPIO_PIO, GPIO_MODE_NONE);
	GPIO_SetDir(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 1);
	GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
	
    GPIO_SetFunction(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIO, GPIO_MODE_NONE);
	GPIO_SetDir(LCD_RW_PORT, LCD_RW_PIN, 1);
    	GPIO_SetValue(LCD_RW_PORT, LCD_RW_PIN, 0);   
	
    GPIO_SetFunction(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIO, GPIO_MODE_NONE);
	GPIO_SetDir(LCD_RS_PORT, LCD_RS_PIN, 1);
	GPIO_SetValue(LCD_RS_PORT, LCD_RS_PIN, 0);
}

void lcdReset(void){
    scandal_delay(100);
    lcdInstr(LCDC_RESET);
    scandal_delay(10);
    lcdInstr(LCDC_RESET);
    scandal_delay(5);
    lcdInstr(LCDC_RESET);
    scandal_delay(5);
    lcdInstr(LCDC_RESET);
    
    scandal_delay(5);
    lcdInstr(LCDC_FUNC_SET);
    scandal_delay(5);
    lcdInstr(LCDC_OFF);
    scandal_delay(5);
    lcdInstr(LCDC_CLEAR);
    scandal_delay(5);
    lcdInstr(LCDC_EMS);
    scandal_delay(5);
    lcdInstr(LCDC_ON);
}

void lcdPuts(char *str, int loc, int row){
    lcdInstr(LCDC_SET_DDRAM | (loc + row * LCD_ROW_MEM_OFFSET));
	scandal_ms_delay(1);
	int i;
	for(i = 0; i + loc < LCD_ROW_LENGTH; i++){
		lcdPutc(str[i]);
	}
}


