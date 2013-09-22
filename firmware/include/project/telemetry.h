/** @mainpage Telemetry function prototypes, macros and defines*/

#ifndef _TELEMETRY_H_
#define _TELEMETRY_H_

/*! \brief Telemetry system will accept all CAN data, as it operates
 * as a sniffer. It does not use inbuilt scandal functions which 
 * implies that it is not assigned the node address. We define its 
 * own node address.
 */
#define TELEMETRY_NODE_ADDRESS 0

/*! \brief Analog switch control pins to choose between RF module & 
 * uC 
 */
#define TX_CNTL_PORT 2
#define TX_CNTL_PIN  6

#define RX_CNTL_PORT 3
#define RX_CNTL_PIN  2
	
#define CONNECT_TO_TRANSCEIVER_PROGRAMMER_INIT \
	GPIO_SetDir(RX_CNTL_PORT, RX_CNTL_PIN, 1); \
	GPIO_SetDir(TX_CNTL_PORT, TX_CNTL_PIN, 1);	
	
#define CONNECT_TO_PROGRAMMER \
	GPIO_SetValue(RX_CNTL_PORT, RX_CNTL_PIN, 1); \
	GPIO_SetValue(TX_CNTL_PORT, TX_CNTL_PIN, 1);
	
#define CONNECT_TO_TRANSCEIVER	\
	GPIO_SetValue(RX_CNTL_PORT, RX_CNTL_PIN, 0); \
	GPIO_SetValue(TX_CNTL_PORT, TX_CNTL_PIN, 0);

#define CONNECT_RX_TO_TRANSCEIVER_TX_TO_PROGRAMMER \
	GPIO_SetValue(RX_CNTL_PORT, RX_CNTL_PIN, 0); \
	GPIO_SetValue(TX_CNTL_PORT, TX_CNTL_PIN, 1); \

#endif
