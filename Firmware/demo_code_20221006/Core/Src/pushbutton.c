/** ***************************************************************************
 * @file
 * @brief USER pushbutton
 *
 * Initializes the GPIO for the pushbutton.
 * @n Enables the interrupt on the pushbutton.
 * @n Implements the interrupt handler for the pushbutton.
 *
 * @author  Hanspeter Hochreutener, hhrt@zhaw.ch
 * @date	16.04.2020
 *****************************************************************************/


/******************************************************************************
 * Includes
 *****************************************************************************/
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"

#include "pushbutton.h"


/******************************************************************************
 * Defines
 *****************************************************************************/


/******************************************************************************
 * Variables
 *****************************************************************************/
static bool PB_pressed_flag = false;	///< USER pushbutton pressed flag


/******************************************************************************
 * Functions
 *****************************************************************************/


/** ***************************************************************************
 * @brief Configure the GPIO for the USER pushbutton
 *
 * The USER pushbutton is connected to PA0.
 *****************************************************************************/
void PB_init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();		// Enable Clock for GPIO port A
	GPIOA->MODER |= (0u << GPIO_MODER_MODER0_Pos);	// Pin 0 of port A = input
}


/** ***************************************************************************
 * @brief Configure interrupt on rising edge for the USER pushbutton
 *
 * The USER pushbutton is connected to PA0.
 *****************************************************************************/
void PB_enableIRQ(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();		// Enable Clock for system config
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;	// EXTI multiplexer
	EXTI->RTSR |= EXTI_RTSR_TR0;		// Rising Trigger Select on int. line 0
	EXTI->IMR |= EXTI_IMR_MR0;			// Interrupt Mask enable on int. line 0
	NVIC_ClearPendingIRQ(EXTI0_IRQn);	// Clear pending interrupt on line 0
	NVIC_EnableIRQ(EXTI0_IRQn);			// Enable interrupt line 0 in the NVIC
}


/** ***************************************************************************
 * @brief Was the pushbutton pressed?
 *
 * @return true if pushbutton was pressed
 *****************************************************************************/
bool PB_pressed(void)
{
	bool pressed = PB_pressed_flag;		// Read/store value of flag
	PB_pressed_flag = false;			// Reset flag
	return pressed;
}


/** ***************************************************************************
 * @brief Interrupt handler for the USER pushbutton
 *
 * The interrupt handler for external line 0 is called.
 * @n The USER pushbutton is connected to PA0.
 *****************************************************************************/
void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR0) {		// Check if interrupt on line 0
		EXTI->PR |= EXTI_PR_PR0;		// Clear pending interrupt on line 0
		PB_pressed_flag = true;			// Set flag
	}
}

