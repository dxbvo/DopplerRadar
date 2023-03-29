/** ***************************************************************************
 * @file
 * @brief See measuring.c
 *
 * Prefixes MEAS, ADC, DAC
 *
 *****************************************************************************/

#ifndef MEAS_H_
#define MEAS_H_


/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdbool.h>


/******************************************************************************
 * Defines
 *****************************************************************************/
extern bool MEAS_data_ready;
extern uint32_t MEAS_input_count;
extern bool DAC_active;
//extern int ADC_NUMS;

// changed defined extern varibles
//extern int32_t pad_left[ADC_NUMS], pad_right[ADC_NUMS], coil_left[ADC_NUMS], coil_right[ADC_NUMS];


/******************************************************************************
 * Functions
 *****************************************************************************/
void MEAS_GPIO_analog_init(void);
void MEAS_timer_init(void);
void DAC_reset(void);
void DAC_init(void);
void DAC_increment(void);
void ADC_reset(void);
void ADC3_IN4_single_init(void);
void ADC3_IN4_single_read(void);
void ADC3_IN4_timer_init(void);
void ADC3_IN4_timer_start(void);
void ADC3_IN4_DMA_init(void);
void ADC3_IN4_DMA_start(void);
void ADC1_IN13_ADC2_IN5_dual_init(void);
void ADC1_IN13_ADC2_IN5_dual_start(void);
void ADC2_IN13_IN5_scan_init(void);
void ADC2_IN13_IN5_scan_start(void);
void ADC3_IN13_IN4_scan_init(void);
void ADC3_IN13_IN4_scan_start(void);

void MEAS_show_data(void);

// our added functions
void orderSamples(void);
double averageOfSamples(int[]);
int32_t rootMeanSquare(int32_t[]);
int32_t averageOfTwoArrays(int32_t, int32_t);
int32_t findDistance(int32_t);





#endif