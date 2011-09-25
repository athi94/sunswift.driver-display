#include <scandal/engine.h>
#include <scandal/message.h>
#include <scandal/leds.h>
#include <scandal/utils.h>
#include <scandal/uart.h>
#include <scandal/stdio.h>
#include <scandal/wdt.h>

#include <string.h>

#include <project/driver_config.h>
#include <project/target_config.h>
#include <arch/can.h>
#include <arch/uart.h>
#include <arch/timer.h>
#include <arch/gpio.h>
#include <arch/types.h>
#include <arch/i2c.h>

#include <project/lcd.h>

//We just blurt over port 3 to get this done
#define LCD_EN	0x1
#define LCD_DEN 0x5
#define LCD_IDLE 0x0 	

#define US_COUNT 4
//delay for GREATER than us us
//Nasty and not microseconds!
static inline void LCD_Delay(int us){
    volatile int i;
    for(i = 0; i < us * US_COUNT; i++);
}

void lcdInstr(uint8_t outbyte){
    LPC_GPIO[LCD_DATA_PORT]->DATA = outbyte;
    LPC_GPIO[LCD_ENABLE_PORT]->DATA = LCD_EN;
    LCD_Delay(2);
    LPC_GPIO[LCD_ENABLE_PORT]->DATA = LCD_IDLE;
    scandal_delay(1);
}
void lcdPutc(uint8_t outbyte){
    LPC_GPIO[LCD_DATA_PORT]->DATA = outbyte;
    LPC_GPIO[LCD_ENABLE_PORT]->DATA = LCD_DEN;
    LCD_Delay(2);
    LPC_GPIO[LCD_ENABLE_PORT]->DATA = LCD_IDLE;
    scandal_delay(1);
}

void lcdOn(void){
    GPIO_SetFunction(LCD_DATA_PORT, 0, GPIO_PIO);
    GPIO_SetFunction(LCD_DATA_PORT, 1, GPIO_PIO);
    GPIO_SetFunction(LCD_DATA_PORT, 2, GPIO_PIO);
    GPIO_SetFunction(LCD_DATA_PORT, 3, GPIO_PIO);
    GPIO_SetFunction(LCD_DATA_PORT, 4, GPIO_PIO);
    GPIO_SetFunction(LCD_DATA_PORT, 5, GPIO_PIO);
    GPIO_SetFunction(LCD_DATA_PORT, 6, GPIO_PIO);
    GPIO_SetFunction(LCD_DATA_PORT, 7, GPIO_PIO);
	GPIO_SetDir(LCD_DATA_PORT, 0, 1);
	GPIO_SetDir(LCD_DATA_PORT, 1, 1);
	GPIO_SetDir(LCD_DATA_PORT, 2, 1);
	GPIO_SetDir(LCD_DATA_PORT, 3, 1);
	GPIO_SetDir(LCD_DATA_PORT, 4, 1);
	GPIO_SetDir(LCD_DATA_PORT, 5, 1);
	GPIO_SetDir(LCD_DATA_PORT, 6, 1);
	GPIO_SetDir(LCD_DATA_PORT, 7, 1);
	LPC_GPIO[LCD_DATA_PORT]->DATA = 0x0;
	
    GPIO_SetFunction(LCD_ENABLE_PORT, LCD_ENABLE_PIN, GPIO_PIO);
	GPIO_SetDir(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 1);
	GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
	
    GPIO_SetFunction(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIO);
	GPIO_SetDir(LCD_RW_PORT, LCD_RW_PIN, 1);
    	GPIO_SetValue(LCD_RW_PORT, LCD_RW_PIN, 0);   
	
    GPIO_SetFunction(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIO);
	GPIO_SetDir(LCD_RS_PORT, LCD_RS_PIN, 1);
	GPIO_SetValue(LCD_RS_PORT, LCD_RS_PIN, 0);

    scandal_delay(100); //15ms power up delay
    lcdInstr(0x30);
    scandal_delay(10); //5ms mandated
    lcdInstr(0x30);
    //LCD_Delay(100); //100us mandated
    scandal_delay(5);
    lcdInstr(0x30);
    scandal_delay(5);
    lcdInstr(0x30);
}

void lcdInit(void){
    scandal_delay(5);
    lcdInstr(0x38); //2 line, 8 dots
    scandal_delay(5);
    lcdInstr(0x08); //Display off
    scandal_delay(5);
    lcdInstr(0x01); //Clear
    scandal_delay(5);
    lcdInstr(0x06); //Increment
    scandal_delay(5);
    lcdInstr(0x0f); //Screen,cursor,blink
#if 0
    uint8_t blah;
	for(blah='a'; blah <= 'k'; blah++){
    scandal_delay(5);
	lcdPutc(blah);
    }
#endif
}

void lcdPuts(char *str){
	while(*str != 0)
		lcdPutc(*str++);
}


