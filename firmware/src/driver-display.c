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

#undef 	IN_CHANNEL_EXAMPLE

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

/* Do some general setup for clocks, LEDs and interrupts
 * and UART stuff on the MSP430 */
void setup(void) {
	GPIO_Init();
	GPIO_SetDir(RED_LED_PORT, RED_LED_BIT, 1);
	GPIO_SetDir(YELLOW_LED_PORT, YELLOW_LED_BIT, 1);
	GPIO_SetValue(YELLOW_LED_PORT, YELLOW_LED_BIT, 1);
	GPIO_SetValue(RED_LED_PORT, RED_LED_BIT, 1);


    //TODO clean up
	GPIO_SetDir(1, 8, 1);
	GPIO_SetValue(1, 8, 0);
	GPIO_SetDir(1, 9, 1);
	GPIO_SetValue(1, 9, 0);

} // setup


/* This is your main function! You should have an infinite loop in here that
 * does all the important stuff your node was designed for */
int main(void) {
	setup();

	/* Initialise the watchdog timer. If the node crashes, it will restart automatically */
	WDT_Init(); 

	/* Initialise Scandal, registers for the config messages, timesync messages and in channels */
	scandal_init();

	/* Set LEDs to known states */
	//red_led(0);
	//yellow_led(1);

	/* Initialise UART0 */
	UART_Init(115200);

	/* Wait until UART is ready */
	scandal_delay(100);

	/* Display welcome header over UART */
	UART_printf("Driver Display\n\r");

	sc_time_t one_sec_timer = sc_get_timer();

    LCD_Init();

    LCD_Print("hello world", 0, 11, 0);
    
	/* This is the main loop, go for ever! */
	while (1) {
		/* This checks whether there are pending requests from CAN, and sends a heartbeat message.
		 * The heartbeat message encodes some data in the first 4 bytes of the CAN message, such as
		 * the number of errors and the version of scandal */
		handle_scandal();

		/* Send a UART and CAN message and flash an LED every second */
		if(sc_get_timer() >= one_sec_timer + 1000) {
			/* Send the message */
			UART_printf("1 second timer %u, red led: %d yellow led: %d\n\r", (unsigned int)sc_get_timer(),
				GPIO_GetValue(RED_LED_PORT, RED_LED_BIT), GPIO_GetValue(YELLOW_LED_PORT, YELLOW_LED_BIT));

			/* Send a channel message with a blerg value at low priority on channel 0 */
			scandal_send_channel(TELEM_LOW, /* priority */
									0,      /* channel num */
									0xaa   /* value */
			);

			/* Twiddle the LEDs */
			toggle_red_led();
			toggle_yellow_led();
			GPIO_SetValue(BLINK_L_PORT, BLINK_L_PIN, 1);
            GPIO_SetValue(YELLOW_LED_PORT, YELLOW_LED_BIT, 1);
            
			/* Update the timer */
			one_sec_timer = sc_get_timer();
		}
		
		/* Tickle the watchdog so we don't reset */
		WDT_Feed();
	}
}
