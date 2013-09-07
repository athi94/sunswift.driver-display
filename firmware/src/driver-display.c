
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
#define SPEED_LOCATION      11
#define SPEED_ROW           0
#define SPEED_SIZE          6
#define SPEED_DECIMAL       2
#define SPEED_ALIGNMENT     LCD_A_LEFT
static u32 speed_updated;

#define LTOPSCROLL_LOCATION       4
#define LTOPSCROLL_ROW            0
#define LTOPSCROLL_SIZE           3
#define LTOPSCROLL_DECIMAL  	  0
#define LTOPSCROLL_ALIGNMENT      LCD_A_RIGHT

#define LBOTTOMSCROLL_LOCATION	4
#define LBOTTOMSCROLL_ROW	1
#define LBOTTOMSCROLL_SIZE	3
#define LBOTTOMSCROLL_DECIMAL   0
#define LBOTTOMSCROLL_ALIGNMENT	LCD_A_RIGHT

#define CC_LOCATION         18
#define CC_ROW              1
#define CC_MESSAGE_ON       "CC"
#define CC_MESSAGE_OFF      "  "
static u32 cc_updated;

#define PC_LOCATION         11
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
	if(row == 0) {
        	message[i + loc] = buffer[i]; 
	} else {
        	message[i + loc + 20] = buffer[i]; 
	}
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

#define WELCOME_MSG1 "SUNSWIFT eVe        "
#define WELCOME_MSG2 "   wear sunscreen   "
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
    
    //WDT_Init(); 
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
	sc_time_t timer = 0;
	int displaycount = -1;

	setup();
	timer=sc_get_timer();

    while (1) {
        handle_scandal();
	if(sc_get_timer()>(timer+2000)){
		displaycount=(displaycount + 1)%4;
		if (displaycount==0){
			updateString("BUS", 0, 0);
            		updateNumber(/*scandal_get_in_channel_value(SPEED_CHANNEL)*/100, LTOPSCROLL_LOCATION, LTOPSCROLL_ROW, LTOPSCROLL_SIZE, LTOPSCROLL_ALIGNMENT, LTOPSCROLL_DECIMAL);
			updateString("V", 7, 0);
			
		}
		else if (displaycount==1){
			updateString("BAT", 0, 0);
			updateNumber(/*scandal_get_in_channel_value(SPEED_CHANNEL)*/60, LTOPSCROLL_LOCATION, LTOPSCROLL_ROW, LTOPSCROLL_SIZE, LTOPSCROLL_ALIGNMENT, LTOPSCROLL_DECIMAL);
			updateString("V", 7, 0);
		}
		else if (displaycount==2){
			updateString("BAT", 0, 0);
			updateNumber(/*scandal_get_in_channel_value(SPEED_CHANNEL)*/123, LTOPSCROLL_LOCATION, LTOPSCROLL_ROW, LTOPSCROLL_SIZE, LTOPSCROLL_ALIGNMENT, LTOPSCROLL_DECIMAL);
			updateString("A", 7, 0);
		}
		else if (displaycount==3){
			updateString("CHG", 0, 0);
			updateNumber(/*scandal_get_in_channel_value(SPEED_CHANNEL)*/40, LTOPSCROLL_LOCATION, LTOPSCROLL_ROW, LTOPSCROLL_SIZE, LTOPSCROLL_ALIGNMENT, LTOPSCROLL_DECIMAL);
			updateString("%", 7, 0);
		}

		if(displaycount == 0 || displaycount == 1) {
			updateNumber(/*scandal_get_in_channel_value(SPEED_CHANNEL)*/40, LBOTTOMSCROLL_LOCATION, LBOTTOMSCROLL_ROW, LBOTTOMSCROLL_SIZE, LBOTTOMSCROLL_ALIGNMENT, LBOTTOMSCROLL_DECIMAL);
			updateString("BAT", 0, 1);
			updateString("C", 7, 1);
		}
		else if(displaycount == 2 || displaycount == 3) {
			updateNumber(/*scandal_get_in_channel_value(SPEED_CHANNEL)*/1, LBOTTOMSCROLL_LOCATION, LBOTTOMSCROLL_ROW, LBOTTOMSCROLL_SIZE, LBOTTOMSCROLL_ALIGNMENT, LBOTTOMSCROLL_DECIMAL);
			updateString("MCT", 0, 1);
			updateString("C", 7, 1);
		}
		timer = sc_get_timer();
        }

		updateNumber(scandal_get_in_channel_value(SPEED_CHANNEL), SPEED_LOCATION, SPEED_ROW,
                SPEED_SIZE, SPEED_ALIGNMENT, SPEED_DECIMAL);

		updateString("CC", CC_LOCATION, CC_ROW);
		updateString("KMH", 17, 0);
		updateString("PC", PC_LOCATION, PC_ROW);

            
        
		drawLCD();
		
		//TODO blinker stuffs
		
		/* Tickle the watchdog so we don't reset */
		///WDT_Feed();
	}
}
