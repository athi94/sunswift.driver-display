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


//TODO clean this shit up
//#include <arch/gpio.h>
//#include <scandal/utils.h>

#define US_COUNT 4


//delay for GREATER than us us
static inline void LCD_Delay(int us){
    volatile int i;
    for(i = 0; i < us * US_COUNT; i++);
}

//TODO ultra sleazy, work out how to write a byte
static inline void LCD_SetChar(char c){
    GPIO_SetValue(LCD_DATA_PORT, 0, (c >> 0) & 1);
    GPIO_SetValue(LCD_DATA_PORT, 1, (c >> 1) & 1);
    GPIO_SetValue(LCD_DATA_PORT, 2, (c >> 2) & 1);
    GPIO_SetValue(LCD_DATA_PORT, 3, (c >> 3) & 1);
    GPIO_SetValue(LCD_DATA_PORT, 4, (c >> 4) & 1);
    GPIO_SetValue(LCD_DATA_PORT, 5, (c >> 5) & 1);
    GPIO_SetValue(LCD_DATA_PORT, 6, (c >> 6) & 1);
    GPIO_SetValue(LCD_DATA_PORT, 7, (c >> 7) & 1);
}

static inline void LCD_Command(char c){
    GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
    LCD_SetChar(c);
    LCD_Delay(30);
    
    GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 1);
    LCD_Delay(30);
    GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
}

void LCD_Init(){
    GPIO_SetFunction(LCD_DATA_PORT, 0, GPIO_PIO);
	GPIO_SetDir(LCD_DATA_PORT, 0, 1);
    GPIO_SetFunction(LCD_DATA_PORT, 1, GPIO_PIO);
	GPIO_SetDir(LCD_DATA_PORT, 1, 1);
    GPIO_SetFunction(LCD_DATA_PORT, 2, GPIO_PIO);
	GPIO_SetDir(LCD_DATA_PORT, 2, 1);
    GPIO_SetFunction(LCD_DATA_PORT, 3, GPIO_PIO);
	GPIO_SetDir(LCD_DATA_PORT, 3, 1);
    GPIO_SetFunction(LCD_DATA_PORT, 4, GPIO_PIO);
	GPIO_SetDir(LCD_DATA_PORT, 4, 1);
    GPIO_SetFunction(LCD_DATA_PORT, 5, GPIO_PIO);
	GPIO_SetDir(LCD_DATA_PORT, 5, 1);
    GPIO_SetFunction(LCD_DATA_PORT, 6, GPIO_PIO);
	GPIO_SetDir(LCD_DATA_PORT, 6, 1);
    GPIO_SetFunction(LCD_DATA_PORT, 7, GPIO_PIO);
	GPIO_SetDir(LCD_DATA_PORT, 7, 1);
	
    GPIO_SetFunction(LCD_ENABLE_PORT, LCD_ENABLE_PIN, GPIO_PIO);
	GPIO_SetDir(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 1);
	
    GPIO_SetFunction(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIO);
	GPIO_SetDir(LCD_RW_PORT, LCD_RW_PIN, 1);
	
    GPIO_SetFunction(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIO);
	GPIO_SetDir(LCD_RS_PORT, LCD_RS_PIN, 1);
	
	GPIO_SetValue(LCD_RS_PORT, LCD_RS_PIN, 0);
    GPIO_SetValue(LCD_RW_PORT, LCD_RW_PIN, 0);   
	GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
	LCD_SetChar(0);
	scandal_delay(300);
	
	//function set
    LCD_Command(LCDC_RESET);
    scandal_delay(5);
    
    LCD_Command(LCDC_RESET);
    scandal_delay(5);
    
    LCD_Command(LCDC_RESET);
    scandal_delay(5);
     
    LCD_Command(LCDC_FUNC_SET);
    scandal_delay(5);
	
    LCD_Command(LCDC_OFF);
	scandal_delay(5);
	
	LCD_Command(LCDC_CLEAR);
	scandal_delay(5);
	
	LCD_Command(LCDC_EMS);
	scandal_delay(5);
	
	//turn the LCD on   
    LCD_Command(LCDC_ON_CURSOR);
    scandal_delay(10);
    
}

int LCD_Print(char * str, int row, int len, int offset){
    
    // Set the character pointer
    GPIO_SetValue(LCD_RS_PORT, LCD_RS_PIN, 0);
    GPIO_SetValue(LCD_RW_PORT, LCD_RW_PIN, 0);   
    LCD_SetChar((1 << 7) | 0);
    scandal_delay(5);
    GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 1);
    scandal_delay(5);
    GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
    scandal_delay(5);
    
    // select writing
    GPIO_SetValue(LCD_RS_PORT, LCD_RS_PIN, 1);   
    GPIO_SetValue(LCD_RW_PORT, LCD_RW_PIN, 0);
    
    GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
    scandal_delay(5);
    
    int i;
    for(i = 0; i < len; i++){
        if(i + offset > LCD_ROW_WIDTH || str[i] == '\0')
            break;
        
        LCD_SetChar(str[i]);
        scandal_delay(5);
        GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 1);
        scandal_delay(5);
        GPIO_SetValue(LCD_ENABLE_PORT, LCD_ENABLE_PIN, 0);
        scandal_delay(1);
    }
    
    return i;
}
