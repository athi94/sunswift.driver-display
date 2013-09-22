/* --------------------------------------------------------------------------                                 
    Template project main
    File name: template.c
    Author: Etienne Le Sueur
    Description: The template main file. It provides a simple example of using
                 some standard scandal functions such as the UART library, the
                 CAN library, timers, LEDs, GPIOs.
                 It is designed to compile and work for the 3 micros we use on
                 the car currently, the MSP430F149 and MCP2515 combination and
                 the LPC11C14 and LPC1768 which have built in CAN controllers.

                 UART_printf is provided by the Scandal stdio library and 
                 should work well on all 3 micros.

                 If you are using this as the base for a new project, you
                 should first change the name of this file to the name of
                 your project, and then in the top level makefile, you should
                 change the CHIP and MAIN_NAME variables to correspond to
                 your hardware.

                 Don't be scared of Scandal. Dig into the code and try to
                 understand what's going on. If you think of an improvement
                 to any of the functions, then go ahead and implement it.
                 However, before committing the changes to the Scandal repo,
                 you should discuss with someone else to ensure that what 
                 you've done is a good thing ;-)

                 Keep in mind that this code is live to the public on
                 Google Code. No profanity in comments please!

    Copyright (C) Etienne Le Sueur, 2011

    Created: 07/09/2011
   -------------------------------------------------------------------------- */

/* 
 * This file is part of the Sunswift Template project
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

#undef 	DOUBLE_BUFFER_EXAMPLE
#undef 	IN_CHANNEL_EXAMPLE
#undef WAVESCULPTOR_EXAMPLE

#define BASE_YEL_LED_PORT 3
#define BASE_YEL_LED_BIT 1
#define BASE_RED_LED_PORT 3
#define BASE_RED_LED_BIT 0

#define CAN_MSG_LEN       15

#include <scandal/engine.h>
#include <scandal/message.h>
#include <scandal/leds.h>
#include <scandal/utils.h>
#include <scandal/uart.h>
#include <scandal/stdio.h>
#include <scandal/wdt.h>
#include <scandal/wavesculptor.h>
#include <scandal/error.h>
#include <scandal/uart.h>

#include <string.h>

#include <project/driver_config.h>
#include <project/target_config.h>
#include <arch/can.h>
#include <arch/uart.h>
#include <arch/timer.h>
#include <arch/gpio.h>
#include <arch/types.h>
#include <arch/i2c.h>

#include <project/display.h>

/* Do some general setup for clocks, LEDs and interrupts
 * and UART stuff on the MSP430 */
void setup(void) {
	GPIO_Init();
	GPIO_SetDir(BASE_RED_LED_PORT, BASE_RED_LED_BIT, OUTPUT);
	GPIO_SetDir(BASE_YEL_LED_PORT, BASE_YEL_LED_BIT, OUTPUT);
} // setup

/* This is your main function! You should have an infinite loop in here that
 * does all the important stuff your node was designed for */
int main(void) {    
        /* Initialise UART0 */
	UART_Init(115200);
        
        setup();
    
        scandal_init();
                
        CONNECT_TO_LCD_PROGRAMMER_INIT
        CONNECT_TO_LCD           
        
	/* Set LEDs to known states */
	red_led(0);
	yellow_led(1);

#ifdef DEBUG_MODE
     Coordinate s;
    __init__Coordinate__(&s, LCD_START_COORDINATE_X, LCD_START_COORDINATE_Y);     
    setNextWriteCoordinate(s);
#endif
        
    __init__all__();
                  
    /** Display eve Logo **/       
    clearScreen();    
    scandal_delay(100);
    
    displayeVeLogo();    
    scandal_delay(4000);
    
    clearScreen();    
    scandal_delay(100);
    
    int last_page_update_time = sc_get_timer();	
    int last_page_clear_time  = sc_get_timer();

    while (1) {
    /* This checks whether there are pending requests from CAN, and 
     * sends a heartbeat message. The heartbeat message encodes some
     * data in the first 4 bytes of the CAN message, such as the
     * number of errors and the version of scandal */		
        handle_scandal();
                
		if(sc_get_timer() - last_page_update_time >= LCD_PAGE_UPDATE_PERIOD)
                //if(sc_get_timer() - last_page_update_time >= 500) 
		{         
			last_page_update_time = sc_get_timer();
	        updateLcdScreen(&l);            			
		}
		
        if(sc_get_timer() - last_page_clear_time >= LCD_REFRESH_PERIOD) { 
            last_page_clear_time=sc_get_timer();
            clearScreen();
        }        
    }
    
}

void UART_SendByte(u08 c)
{
	UART_putchar((char)c);
}

