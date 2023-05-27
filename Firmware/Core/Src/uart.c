//-----------------------------------------------------------------------------
// @file
// @brief Uart zum debugen
//
// ----------------------------------------------------------------------------
// @author Patrick Rennhard, renn@zhaw.ch
// @date 01.02.2022
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>

#include "string.h"
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
#include "stm32f4xx_hal_uart.h"
#include "uart.h"

UART_HandleTypeDef huart1;

void debugPrintStrUSART1(char _out[]) {
	HAL_UART_Transmit(&huart1, (uint8_t *) _out, strlen(_out), 10);
}

void debugPrintByteUSART1(uint8_t *data, uint32_t length) {
	HAL_UART_Transmit(&huart1, data, length, 100);
}

void debugPrintHexDumpUSART1(uint8_t *data, uint32_t length) {
	uint32_t n=0;
	char tmp[3];
	for(n=0; n<length; n++) {
		sprintf(tmp,"%02X",data[n]);
		HAL_UART_Transmit(&huart1, (uint8_t *) tmp, 2, 10);
	}
}

void uartSendData(char** data, uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    debugPrintStrUSART1(data[i]);
  }
}

void uart_init(void) {
	// Enable clock for GPIOA
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/**
	* Tell pins PA9 and PA10 which alternating function you will use
	* @important Make sure, these lines are before pins configuration!
	*/

	GPIO_InitTypeDef  GPIO_InitStruct;

	/*Configure GPIO pin : GPIO_PIN_9  TX */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : GPIO_PIN_10  RX */
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__HAL_RCC_USART1_CLK_ENABLE();

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);

	debugPrintStrUSART1("Renn Test\r\n"); // print

	//HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
	//HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart);
	//void HAL_UART_MspInit(UART_HandleTypeDef *huart);
	//void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);
}


