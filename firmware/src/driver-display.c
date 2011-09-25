
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

#define WELCOME_MSG1 "SUNSWIFT IVy    "
#define WELCOME_MSG2 "    ...Hopefully"

/* LCD displays whatever is in these strings */
char upper[ROW+1];				// displayed on the LCD top row
char lower[ROW+1];				// displayed on the LCD bottom row if there is no message
char message[LINE+1];			// displayed on the LCD bottom row if message is 'new'

/* Do some general setup for clocks, LEDs and interrupts
 * and UART stuff on the MSP430 */
void setup(void) {
	GPIO_Init();
	GPIO_SetDir(RED_LED_PORT, RED_LED_BIT, 1);
	GPIO_SetDir(YELLOW_LED_PORT, YELLOW_LED_BIT, 1);
	GPIO_SetValue(YELLOW_LED_PORT, YELLOW_LED_BIT, 1);
	GPIO_SetValue(RED_LED_PORT, RED_LED_BIT, 1);

	GPIO_SetDir(1, 8, 1);	//Left and Right Blinkers
	GPIO_SetValue(1, 8, 1);
	GPIO_SetDir(1, 9, 1);
	GPIO_SetValue(1, 9, 1);

} // setup

/*	=============================
		LCD Code starts here
	============================= */
/* inserts a data packet into the lcd strings */
int displayCell(int pos, s32 value)
{
	int i, neg, last_pos, d_places = D_PLACES;
	char* string;

	/* is cell position on the upper line or lower line? */
	if(pos > LINE/DIGITS) {
		string = lower;
		pos -= LINE/DIGITS;
	}
	else
		string = upper;
	
	last_pos = pos*DIGITS;						// position of the last character to be stored in the string
	// value /= ((3-D_PLACES)*10);					// we knock off one decimal place (now have 2)

	/* detects negative values */
	if((neg = (value < 0)) == 1)
	{
		value *= -1;
		}

	/* drops off decimal places to attempt to display a big number */
	while(value >= CELL_MAX_VAL) {
		if(d_places <= 0) {			// the number is too big
			return 1;				// error
		}
		d_places--;
		value /= 10;
	}
	if(neg && (value >= CELL_MAX_VAL/10))
	{
		if(d_places <= 0)			// the number is too big
			return 1;				// error
		d_places--;
		value /= 10;
	}
/*	
	if(d_places && (value>=10000))
	{
		d_places--;
		value /= 10;
	}
*/
	if(d_places && (value>=1000))
	{
		d_places--;
		value /=10;
	}

	/* if there are decimal places, set them and the '.' */
	if(d_places > 0) {
		for(i = last_pos; i > last_pos - d_places; i--) {
			string[i-1] = (value%10) + '0';
			value /= 10;
		}
		string[i-1] = '.';
		d_places++;				// shift next digit along (since there is a '.')
	}

	/* set the integer part */
	for(i = last_pos - d_places; i > last_pos - DIGITS; i--) {
		if(value == 0) {
			if(neg == 1) {			// place a minus sign infront of the number
				string[i-1] = '-';
				neg = 0;
			}
			/*
			else if((d_places==D_PLACES) && (i==(last_pos - DIGITS +1)))
				string[i-1] = '0';
				*/
			else
				string[i-1] = ' ';
		}
		else
			string[i-1] = (value%10) + '0';
		value /= 10;
	}
	
	return 0;
}


/* inserts a leading '!' into a cell */
void oldCell(int pos)
{
	char* string;
	/* is cell position on the upper line or lower line? */
	if(pos > LINE/DIGITS) {
		string = lower;
		pos -= LINE/DIGITS;
	}
	else
		string = upper;

	string[pos*DIGITS - 1] = '!';
}


/* inserts a message packet into the message string */
void displayMessage(s32 chars)
{
	int i;
	for(i=0; i <= LINE - (LINE/DIGITS); i++)
		message[i] = message[i+4];
	for(i=1; i<=4; i++)
		message[LINE-i] = (char) (chars >> (i-1)*BYTE);
	message[LINE] = '\0';
}


/* prints the right strings to the LCD */
void updateLCD(sc_time_t msg_time)
{
	lcdInstr(0x02);									// returns cursor to home
	scandal_ms_delay(SHORT_DELAY);

	if(sc_get_timer() > msg_time + MSG_TIME) {		// display the message for MSG_TIME milliseconds
		lcdPuts(upper);
		lcdPuts(lower);
		memset(message, ' ', ROW*sizeof(char));					// clear the message string
	}
	else {
		lcdPuts(upper);
		lcdPuts(message);
	}
}

void lcdWelcome()
{
	lcdInstr(0x01);
	//lcdInstr(0x02);			// returns cursor to home
	scandal_ms_delay(SHORT_DELAY);
	scandal_ms_delay(SHORT_DELAY);
	scandal_ms_delay(SHORT_DELAY);
	lcdPuts(WELCOME_MSG1);
	lcdPuts(WELCOME_MSG2);
}

void scottee_handler(uint32_t value, uint32_t timestamp){
	displayCell(3, value);
	toggle_red_led();
}
/* This is your main function! You should have an infinite loop in here that
 * does all the important stuff your node was designed for */
int main(void) {
	int i,a;
	u32 timer;
	u32 channel_updated[MSPLCD_NUM_IN_CHANNELS - 1];
	memset(channel_updated, 0, (MSPLCD_NUM_IN_CHANNELS-1)*sizeof(u32));
	u32 message_updated = 0 - MSG_TIME;

	setup();

	WDT_Init(); 
	scandal_init();
	UART_Init(115200);

	sc_time_t one_sec_timer = sc_get_timer();

	/* initialise the output strings */
	memset(upper, ' ', ROW*sizeof(char));
	upper[ROW] = '\0';
	memset(lower, ' ', ROW*sizeof(char));
	lower[ROW] = '\0';
	memset(message, ' ', ROW*sizeof(char));
	message[ROW] = '\0';

	lcdOn();
	lcdInit();
	lcdWelcome();

	message_updated = sc_get_timer() - MSG_TIME;
	a=5;

	scandal_register_in_channel_handler(2, &scottee_handler);

	while (1) {
		handle_scandal();

		for(i=MSPLCD_CHANNEL_1; i<=MSPLCD_CHANNEL_10; i++) {
			if(scandal_get_in_channel_rcvd_time(i) > channel_updated[i]) { 
				displayCell(i+1, scandal_get_in_channel_value(i));
				channel_updated[i] = scandal_get_in_channel_rcvd_time(i);
				toggle_red_led();
			}
	}

		if(scandal_get_in_channel_rcvd_time(MSPLCD_MESSAGE) > message_updated) { 
			displayMessage(scandal_get_in_channel_value(MSPLCD_MESSAGE));
			message_updated = scandal_get_in_channel_rcvd_time(MSPLCD_MESSAGE);
		}
		if(sc_get_timer() >= one_sec_timer + 1000) {
			one_sec_timer = sc_get_timer();

			/* Send a channel message with a blerg value at low priority on channel 0 */
			scandal_send_channel(TELEM_LOW, /* priority */
					0,      /* channel num */
					0xaa   /* value */
					);

			/* Twiddle the LEDs */
			toggle_yellow_led();
			
			displayCell(1, a);
			a+=33;
		updateLCD(message_updated);

		}

		/* Tickle the watchdog so we don't reset */
		WDT_Feed();
	}
}
