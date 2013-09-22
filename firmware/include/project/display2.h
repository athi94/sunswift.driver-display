/** @mainpage Driver Display function prototypes, macros and defines */


#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <string.h>
#include <stdio.h>
			
/*! \brief Code to precede all serial commands that are transmitted 
 * to LCD*/		
#define LCD_COMMAND_BYTE 124 //0x7C

/*! \brief Code to be transmitted when setting the X location of the 
 * next data byte to be displayed on the LCD*/			
#define LCD_COMMAND_BYTE_SET_X 24 //0x18
	
/*! \brief Code to be transmitted when setting the Y location of the
 * next byte to be displayed on the LCD*/			
#define LCD_COMMAND_BYTE_SET_Y 25 //0x19		
	
/*! \brief Data from a can message and the time on which the data
 *  was received */
typedef struct	{ float value; int time; } data_record;	

/*! \brief Speed sources, and LCD message and location for display */
#define SPEED_SRC1 0  
data_record speed1;	 	
#define SPEED_SRC2 1
data_record speed2; 	
#define SPEED_X    1
#define SPEED_Y    1 
#define SPEED_MSG "Speed"
#define SPEED_UNIT "km/h"


/*! \brief Battery Voltage sources, and LCD message and location for
*  display*/
#define BV_SRC1  2 
data_record bv1;	
#define BV_SRC2  3
data_record bv2;    
#define BV_X     90
#define BV_Y     1 
#define BV_MSG "Battery Voltage"
#define BV_UNIT "V"


/*! \brief Cruise Control sources, and LCD message and location for
*  display*/
#define CC_SRC1  4
data_record cc1;       
#define CC_SRC2  5 
data_record cc2;    
#define CC_X     1
#define CC_Y     45
#define CC_MSG "Cruise Control"
#define CC_UNIT ""

/*! \brief Precharge sources, and LCD message and LCD x,y location  
 *  to display*/
#define PCH_SRC1 6
data_record pch1;    
#define PCH_SRC2 7
data_record pch2;       
#define PCH_X    90 
#define PCH_Y    45 
#define PCH_MSG  "Precharge"    
#define PCH_UNIT ""

/*! \brief If the data received from source 2 is fresher (arrived 
* later) than the data from source 1 by THRESHOLD_TIME_DIFF, then
* display data from source 2 */
#define THRESHOLD_TIME_DIFF 5000

/*! \brief Defines all channel message interrupt handler. When the data
 * is received, the following channel handlers are called 
 * For each function, we have the following parameters: 
 * \param value - Value received from the CAN 
 * \param src_time - Time on the scandal when the data was received 
 * from CAN
 */
void in_channel_0_handler(int32_t value, uint32_t src_time){
	speed1.value = value;
	speed1.time  = src_time;
}
void in_channel_1_handler(int32_t value, uint32_t src_time){
	speed2.value = value;
	speed2.time  = src_time;
}
void in_channel_2_handler(int32_t value, uint32_t src_time){
	bv1.value = value;
	bv1.time  = src_time;
}
void in_channel_3_handler(int32_t value, uint32_t src_time){
	bv2.value = value;
	bv2.time  = src_time;
}
void in_channel_4_handler(int32_t value, uint32_t src_time){
	cc1.value = value;
	cc1.time  = src_time;
}
void in_channel_5_handler(int32_t value, uint32_t src_time){
	cc2.value = value;
	cc2.time  = src_time;
}
void in_channel_6_handler(int32_t value, uint32_t src_time){
	pch1.value = value;
	pch1.time  = src_time;
}
void in_channel_7_handler(int32_t value, uint32_t src_time){
	pch2.value = value;
	pch2.time  = src_time;
}

/*! \brief Choose most accurate record based on update last time the data 
 * was updated. If the opt1 data is fresher, then it must be dispalyed. 
 * Otherwise, opt2 should be displayed. 
 * 
 * \param opt1 The preferred dataset
 * \param opt2 The non-preferred dataset
 * */
data_record choose_record(data_record opt1, data_record opt2)
{
	/* Option1 should be the latest update to be chosen for display
	 * on LCD*/
	if(opt2.time - opt1.time > THRESHOLD_TIME_DIFF)
	{	
		return opt2;
	} else
	{
		return opt1;
	}
}

/*! \brief Choose most accurate record based on update last time the data 
 * was updated
 * \param msg The message to be displayed 
 * \param x LCD location to display the value
 * \param y LCD location to display the value
 * \param record To display the recordv value. E.g. speed record
 * \param unit The unit to suffix on the record value
 * */	
/* Writes a record value prepended by a message to the serial LCD */
void write_string_to_lcd(char* msg, int x, int y, data_record record, 
  char* unit)	
{
	UART_putchar((char)LCD_COMMAND_BYTE);
	UART_putchar((char)LCD_COMMAND_BYTE_SET_X);
	UART_putchar((char)x);
	
	UART_putchar((char)LCD_COMMAND_BYTE);
	UART_putchar((char)LCD_COMMAND_BYTE_SET_Y);
	UART_putchar((char)y);
	
	UART_printf(msg);
	UART_printf(": ");
	UART_printf("%f", record.value);		
	UART_printf(unit);
}

/*!
 *  \brief We define the source addresses and input address and 
 *  channels below, instead of using USB-CAN
 */
#define SCANDAL_ADDRESS_OVERRIDE_ENABLE 0 
#define SCANDAL_ADDRESS_OVERRIDE 0

#define SCANDAL_IN_CHANNEL_0_OVERRIDE_ENABLE	0
#define SCANDAL_IN_CHANNEL_0_OVERRIDE_ADDRESS 0 
#define SCANDAL_IN_CHANNEL_0_OVERRIDE_CHANNEL 0

#define SCANDAL_IN_CHANNEL_1_OVERRIDE_ENABLE	0
#define SCANDAL_IN_CHANNEL_1_OVERRIDE_ADDRESS 0
#define SCANDAL_IN_CHANNEL_1_OVERRIDE_CHANNEL 0

#define SCANDAL_IN_CHANNEL_2_OVERRIDE_ENABLE 0	
#define SCANDAL_IN_CHANNEL_2_OVERRIDE_ADDRESS 0
#define SCANDAL_IN_CHANNEL_2_OVERRIDE_CHANNEL 0

#define SCANDAL_IN_CHANNEL_3_OVERRIDE_ENABLE	 0
#define SCANDAL_IN_CHANNEL_3_OVERRIDE_ADDRESS 0
#define SCANDAL_IN_CHANNEL_3_OVERRIDE_CHANNEL 0

#define SCANDAL_IN_CHANNEL_4_OVERRIDE_ENABLE	 0
#define SCANDAL_IN_CHANNEL_4_OVERRIDE_ADDRESS 0
#define SCANDAL_IN_CHANNEL_4_OVERRIDE_CHANNEL 0

#define SCANDAL_IN_CHANNEL_5_OVERRIDE_ENABLE	 0
#define SCANDAL_IN_CHANNEL_5_OVERRIDE_ADDRESS 0
#define SCANDAL_IN_CHANNEL_5_OVERRIDE_CHANNEL 0

#define SCANDAL_IN_CHANNEL_6_OVERRIDE_ENABLE	
#define SCANDAL_IN_CHANNEL_6_OVERRIDE_ADDRESS 0
#define SCANDAL_IN_CHANNEL_6_OVERRIDE_CHANNEL 0

#define SCANDAL_IN_CHANNEL_7_OVERRIDE_ENABLE	
#define SCANDAL_IN_CHANNEL_7_OVERRIDE_ADDRESS 0
#define SCANDAL_IN_CHANNEL_7_OVERRIDE_CHANNEL	 0	

/*! \brief Analog switch control pin to choose between the LCD and uC 
 */
#define TX_CNTL_PORT 1
#define TX_CNTL_PIN  11

/*! \brief Connect TX Line to the LCD. Since FT311D is not present,
 *  we short lines 2 and 3 of the U4 analog switch
 */
#define CONNECT_TO_LCD_PROGRAMMER_INIT GPIO_SetDir(TX_CNTL_PORT, TX_CNTL_PIN, 1);
#define CONNECT_TO_LCD GPIO_SetValue(TX_CNTL_PORT, TX_CNTL_PIN, 0); 
	
#define CONNECT_TO_PROGRAMMER GPIO_SetValue(TX_CNTL_PORT, TX_CNTL_PIN, 1);

#endif
