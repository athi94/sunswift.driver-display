
/* 
 * This file is part of the Sunswift Driver-Display project.
 * 
 * This tempalte is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with the project.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <scandal/engine.h>
#include <scandal/message.h>
#include <scandal/utils.h>
#include <scandal/uart.h>
#include <scandal/stdio.h>
#include <scandal/wdt.h>

#include <string.h>
#include <stdio.h>

#include <project/driver_config.h>
#include <project/target_config.h>
#include <arch/can.h>
#include <arch/uart.h>
#include <arch/timer.h>
#include <arch/gpio.h>
#include <arch/types.h>
#include <arch/i2c.h>

#include <project/lcd.h>

typedef enum lcd_alignment{
    LCD_A_LEFT,
    LCD_A_RIGHT
} lcd_alignment_t;

/* 
 * Defines for each item displayed on the lcd. 
 * There is not just a nice array of channels that are displayed symmetrically because different 
 * channels are displayed differently in this display
 */
 
/* BusV               Speed           
 *   --------------------
 *  |120.00        100.00|
 *  |PC                CC|
 *   --------------------
 * Precharge    Cruise Cont
 */
#define SPEED_LOCATION      10
#define SPEED_ROW           0
#define SPEED_SIZE          10
#define SPEED_DECIMAL       2
#define SPEED_ALIGNMENT     LCD_A_RIGHT
static u32 speed_updated;

#define BUSV_LOCATION       0
#define BUSV_ROW            0
#define BUSV_SIZE           10
#define BUSV_DECIMAL        2
#define BUSV_ALIGNMENT      LCD_A_LEFT
static u32 busv_updated;

#define CC_LOCATION         18
#define CC_ROW              1
#define CC_MESSAGE_ON       "CC"
#define CC_MESSAGE_OFF      "  "
static u32 cc_updated;

#define PC_LOCATION         0
#define PC_ROW              1
#define PC_MESSAGE_ON       "PC"
#define PC_MESSAGE_OFF      "  "

/* Blinker Indicators */
#define BLINKER_L_PORT      1
#define BLINKER_L_BIT       8
#define BLINKER_R_PORT      1
#define BLINKER_R_BIT       9

static u32 blinker_l_updated;
static u32 blinker_r_updated;

//TODO these should go into the correct spot
#define SPEED_CHANNEL       1
#define BUSV_CHANNEL        2
#define CC_CHANNEL          3
#define BLINKER_L_CHANNEL   4
#define BLINKER_R_CHANNEL   5

/* Both lines of the LCD display are stored in this string */
static char message[LCD_ROW_LENGTH * LCD_ROWS];

/* 
 * Writes a string corresponding to the number num into the LCD display string.
 * The number is taken as an int but the decimal variable determines where the decimal place will be
 * in the final displayed string.
 */
static void updateNumber(int num, int loc, int row, int length, 
        lcd_alignment_t alignment, int decimal){
    //TODO using floats and loops is sleazy
    int i;
    int divisor = 1;
    for(i = 0; i < decimal; i++){
        divisor *= 10;
    }
    
    char buffer[LCD_ROW_LENGTH * LCD_ROWS];
    
    if(alignment == LCD_A_RIGHT){
        sprintf(buffer, "%*.*f", length, decimal, (float) num/divisor); 
    } else {
        sprintf(buffer, "%-*.*f", length, decimal, (float) num/divisor);
    }
    
    for(i = 0; i < length; i++){
        message[i + loc] = buffer[i]; 
    }
}

/*
 * Writes the string str into the LCD display string at loc.
 * Strings must be terminated by the null character.
 */
static void updateString(char * str, int loc, int row){
    int i;
    for(i = 0; loc + i < LCD_ROW_LENGTH; i++){
        if(str[i] == '\0')
            break;
        message[loc + i + row*LCD_ROW_LENGTH] = str[i];
    }
}

/*
 * Updates the message on the LCD to match the message string
 */
static void drawLCD(){
    lcdPuts(message, 0 , 0);
    lcdPuts(message + LCD_ROW_LENGTH, 0, 1);
}

#define WELCOME_MSG1 "SUNSWift IVy        "
#define WELCOME_MSG2 "      Driver Display"
/*
 * Simply displays a welcome message
 */
static void displayWelcome(){
    updateString(WELCOME_MSG1, 0, 0);
    updateString(WELCOME_MSG2, 0, 1);
    drawLCD();
}

void setup(void) {
	GPIO_Init();
	
	/* Blinkers gotta blink  */
	GPIO_SetFunction(BLINKER_L_PORT, BLINKER_L_BIT, GPIO_PIO, GPIO_MODE_NONE);
	GPIO_SetDir(BLINKER_L_PORT, BLINKER_L_BIT, 1);
	GPIO_SetValue(BLINKER_L_PORT, BLINKER_L_BIT, 1);
	
	GPIO_SetFunction(BLINKER_R_PORT, BLINKER_R_BIT, GPIO_PIO, GPIO_MODE_NONE);
    GPIO_SetDir(BLINKER_R_PORT, BLINKER_R_BIT, 1);
	GPIO_SetValue(BLINKER_R_PORT, BLINKER_R_BIT, 1);
	
	/*
	GPIO_SetDir(RED_LED_PORT, RED_LED_BIT, 1);
	GPIO_SetDir(YELLOW_LED_PORT, YELLOW_LED_BIT, 1);
	GPIO_SetValue(YELLOW_LED_PORT, YELLOW_LED_BIT, 1);
	GPIO_SetValue(RED_LED_PORT, RED_LED_BIT, 1);
    */
    
    WDT_Init(); 
	scandal_init();
	UART_Init(115200);
	
	lcdInit();
	lcdReset();
	displayWelcome();
	
	scandal_delay(1000);
	
	/* initialise the output string */
	memset(message, ' ', LCD_ROW_LENGTH * LCD_ROWS);
}

int main(void) {
	/* Initialise everything */
	setup();

    while (1) {
        handle_scandal();
		
		/* Update all the values to display, these are done on a case by case basis */
        if(scandal_get_in_channel_rcvd_time(SPEED_CHANNEL) > speed_updated){
            updateNumber(scandal_get_in_channel_value(SPEED_CHANNEL), SPEED_LOCATION, SPEED_ROW,
                SPEED_SIZE, SPEED_ALIGNMENT, SPEED_DECIMAL);
        }
        
        if(scandal_get_in_channel_rcvd_time(BUSV_CHANNEL) > busv_updated){
            updateNumber(scandal_get_in_channel_value(BUSV_CHANNEL), BUSV_LOCATION, BUSV_ROW,
                    BUSV_SIZE, BUSV_ALIGNMENT, BUSV_DECIMAL);
        }
        
        if(scandal_get_in_channel_rcvd_time(CC_CHANNEL) > cc_updated){
            if(scandal_get_in_channel_value(CC_CHANNEL)){
                //TODO updateString(
            } else {
           
            }
        }
        
		drawLCD();
		
		//TODO blinker stuffs
		
		/* Tickle the watchdog so we don't reset */
		WDT_Feed();
	}
}
