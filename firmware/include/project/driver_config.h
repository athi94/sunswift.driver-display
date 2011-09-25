/*
 * driver_config.h
 *
 *  Created on: Aug 31, 2010
 *      Author: nxp28548
 */

#ifndef DRIVER_CONFIG_H_
#define DRIVER_CONFIG_H

#if defined(lpc11c14)
#include <cmsis/LPC11xx.h>
#endif

#if defined(lpc1768)
#include <cmsis/LPC17xx.h>
#endif

#undef CAN_UART_DEBUG

#define CONFIG_ENABLE_DRIVER_CRP						1
#define CONFIG_CRP_SETTING_NO_CRP						1

#define CONFIG_ENABLE_DRIVER_TIMER32					1
#define CONFIG_TIMER32_DEFAULT_TIMER32_0_IRQHANDLER		1

#define CONFIG_ENABLE_DRIVER_GPIO						1

#define CONFIG_ENABLE_DRIVER_UART						1
#define CONFIG_UART_DEFAULT_UART_IRQHANDLER				1
#define CONFIG_UART_ENABLE_INTERRUPT					1

#define CONFIG_ENABLE_DRIVER_CAN						1
#define CONFIG_CAN_DEFAULT_CAN_IRQHANDLER				1

//I2
#define CONFIG_ENABLE_DRIVER_I2C						1
#define CONFIG_I2C_DEFAULT_I2C_IRQHANDLER				1

/* LCD Specific */
#define ROW 16
#define LINE 16
#define D_PLACES		3			// since milliblah
#define DIGITS			5			// number of characters for each cell
#define CELL_MAX_VAL	100000
#define BYTE			8
#define WELCOME_DELAY	5000
#define SHORT_DELAY			200
#define MSG_TIME		10000		// amount of time to display text message for
#define CELL_OLD		3000		// cells are considered old if they haven't been updated for a while


 /* DRIVER_CONFIG_H_ */
#endif
