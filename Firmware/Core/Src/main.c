/** ***************************************************************************
 * @file
 * @brief Sets up the microcontroller, the clock system and the peripherals.
 *
 * Initialization is done for the system, the blue user button, the user LEDs,
 * and the LCD display with the touchscreen.
 * @n Then the code enters an infinite while-loop, where it checks for
 * user input and starts the requested demonstration.
 *
 * @author  Hanspeter Hochreutener, hhrt@zhaw.ch
 * @date	17.06.2021
 *****************************************************************************/


/******************************************************************************
 * Includes
 *****************************************************************************/
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
#include "arm_cfft_init_f32.h"
#include <stdio.h>

#include "main.h"
#include "pushbutton.h"
#include "menu.h"
#include "measuring.h"
#include <arm_math.h>


/******************************************************************************
 * Defines
 *****************************************************************************/
#define FFT_SIZE 64
#define SPEED_OF_LIGHT 299792458.0
#define TRANSMIT_FREQUENCY 24000000000.0


/******************************************************************************
 * Variables and Constants
 *****************************************************************************/
static arm_cfft_instance_f32 fftInstance;
float32_t velocity;
float32_t testArray[2*FFT_SIZE];
static float32_t testOutput[FFT_SIZE];

float32_t testData[] = {
		#include "testData.csv"
};

/******************************************************************************
 * Functions
 *****************************************************************************/
static void SystemClock_Config(void);	///< System Clock Configuration
static void gyro_disable(void);			///< Disable the onboard gyroscope

/** ***************************************************************************
 * @brief  Main function
 * @return not used because main ends in an infinite loop
 *
 * Initialization and infinite while loop
 *****************************************************************************/
int main(void) {
	HAL_Init();							// Initialize the system

	SystemClock_Config();				// Configure system clocks

#ifdef FLIPPED_LCD
	BSP_LCD_Init_Flipped();				// Initialize the LCD for flipped orientation
#else
	BSP_LCD_Init();						// Initialize the LCD display
#endif
	BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
	BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
	BSP_LCD_DisplayOn();
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());	// Touchscreen
	/* Uncomment next line to enable touchscreen interrupt */
	// BSP_TS_ITConfig();					// Enable Touchscreen interrupt

	PB_init();							// Initialize the user pushbutton
	PB_enableIRQ();						// Enable interrupt on user pushbutton

	BSP_LED_Init(LED3);					// Toggles in while loop
	BSP_LED_Init(LED4);					// Is toggled by user button

	MENU_draw();						// Draw the menu
	MENU_hint();						// Show hint at startup

	gyro_disable();						// Disable gyro, use those analog inputs

	MEAS_GPIO_analog_init();			// Configure GPIOs in analog mode
	MEAS_timer_init();					// Configure the timer

    // Initialize the FFT instance
    arm_cfft_init_f32(&fftInstance, FFT_SIZE);

	/* Infinite while loop */
	while (1) {							// Infinitely loop in main function
		BSP_LED_Toggle(LED3);			// Visual feedback when running

		if (MEAS_data_ready) {			// Show data if new data available
			MEAS_data_ready = false;

		    // Perform the FFT, 0 indicates forward FFT, 1 enables bit reversal of output
		    arm_cfft_f32(&fftInstance, testData, 0, 1);

		    // magnitude calculation
		    arm_cmplx_mag_f32(testData, testOutput, FFT_SIZE);

		    // set DC value to 0 because we have an offset of 1.4V
		    testOutput[0] = 0;

		    // get max value and corresponding index
		    float32_t max_value;
		    uint32_t max_index; // index at max value
		    arm_max_f32(testOutput, FFT_SIZE, &max_value, &max_index);

		    // Calculate Doppler frequency
		    float32_t dopplerFrequency;
		    // new cast
		    dopplerFrequency = max_index * ((float32_t)ADC_FS / FFT_SIZE);

		    if (max_index > (ADC_FS / 2)) {
		    	dopplerFrequency = ADC_FS - dopplerFrequency;
		    	// This is the same: dopplerFrequency = - dopplerFrequency;
		    }

//		    // print highest value in ADC_samples
//		    int arr_size = sizeof(testOutput) / sizeof(float32_t);
//		    float32_t max_val = testOutput[0];
//
//		    // get max value which corresponds to Doppler frequency
//		    for (int i = 1; i < arr_size; i++) {
//		        if (testOutput[i] > max_val) {
//		            max_val = testOutput[i];
//		        }
//		    }

		    // Calculate velocity in m/s
		    float32_t lambda = SPEED_OF_LIGHT / TRANSMIT_FREQUENCY;
		    velocity = (dopplerFrequency*lambda) / 2.0f;

		    // convert to m/s to km/h and round to accuracy +/- 0.3
		    velocity = velocity*3.6;
		    velocity = roundToAccuracy(velocity);

		    MEAS_show_data();
		}

		if (PB_pressed()) {				// Check if user pushbutton was pressed
			DAC_active = !DAC_active;	// Toggle DAC on/off
			if (DAC_active) {
				DAC_init();
				BSP_LED_On(LED4);
			} else {
				DAC_reset();
				BSP_LED_Off(LED4);
			}
		}

		/* Comment next line if touchscreen interrupt is enabled */
		MENU_check_transition();

		switch (MENU_get_transition()) {	// Handle user menu choice
		case MENU_NONE:					// No transition => do nothing
			break;
		case MENU_ZERO:
			ADC1_IN13_ADC2_IN11_dual_init(); // ADC initialize
			ADC1_IN13_ADC2_IN11_dual_start(); // star sampling
			DMA2_Stream4_IRQHandler(); // write samples in ADC_samples
			//MEAS_show_data(); // show ADC_samples I and Q of Doppler Radar
			break;
		case MENU_ONE:
			break;
		case MENU_TWO:
			break;
		case MENU_THREE:
			break;
		case MENU_FOUR:
			break;
		case MENU_FIVE:
			break;
		default:						// Should never occur
			break;
		}

		HAL_Delay(200);					// Wait or sleep
	}
}


/** ***************************************************************************
 * @brief System Clock Configuration
 *
 *****************************************************************************/
static void SystemClock_Config(void){
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	/* Configure the main internal regulator output voltage */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* Initialize High Speed External Oscillator and PLL circuits */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* Initialize gates and clock dividers for CPU, AHB and APB busses */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
	/* Initialize PLL and clock divider for the LCD */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	/* Set clock prescaler for ADCs */
	ADC->CCR |= ADC_CCR_ADCPRE_0;
}


/** ***************************************************************************
 * @brief Disable the GYRO on the microcontroller board.
 *
 * @note MISO of the GYRO is connected to PF8 and CS to PC1.
 * @n Some times the GYRO goes into an undefined mode at startup
 * and pulls the MISO low or high thus blocking the analog input on PF8.
 * @n The simplest solution is to pull the CS of the GYRO low for a short while
 * which is done with the code below.
 * @n PF8 is also reconfigured.
 * @n An other solution would be to remove the GYRO
 * from the microcontroller board by unsoldering it.
 *****************************************************************************/
static void gyro_disable(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();		// Enable Clock for GPIO port C
	/* Disable PC1 and PF8 first */
	GPIOC->MODER &= ~GPIO_MODER_MODER1_Msk;	// Reset mode for PC1
	GPIOC->MODER |= 1UL << GPIO_MODER_MODER1_Pos;	// Set PC1 as output
	GPIOC->BSRR |= GPIO_BSRR_BR1;		// Set GYRO (CS) to 0 for a short time
	HAL_Delay(10);						// Wait some time
	GPIOC->MODER |= 3UL << GPIO_MODER_MODER1_Pos;	// Analog PC1 = ADC123_IN11
	__HAL_RCC_GPIOF_CLK_ENABLE();		// Enable Clock for GPIO port F
	GPIOF->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8_Msk;	// Reset speed of PF8
	GPIOF->AFR[1] &= ~GPIO_AFRH_AFSEL8_Msk;	// Reset alternate function of PF8
	GPIOF->PUPDR &= ~GPIO_PUPDR_PUPD8_Msk;	// Reset pulup/down of PF8
	HAL_Delay(10);						// Wait some time
	GPIOF->MODER |= 3UL << GPIO_MODER_MODER8_Pos; // Analog mode PF8 = ADC3_IN4
}
