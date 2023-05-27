/*
 * uart.h
 *
 *  Created on: 01.02.2022
 *      Author: patrickrennhard
 */

#ifndef INC_UART_H_
#define INC_UART_H_

void uart_init(void);
void debugPrintHexDumpUSART1(uint8_t *data, uint32_t length);
void debugPrintStrUSART1(char _out[]);
void uartSendData(char** data, uint32_t length);
#endif /* INC_UART_H_ */
