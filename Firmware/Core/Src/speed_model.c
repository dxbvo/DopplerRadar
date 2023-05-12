/*
 * speed_model.c
 *
 *  Created on: May 9, 2023
 *      Author: diegobelusky
 */

// Includes
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
#include "arm_cfft_init_f32.h"
#include "main.h"
#include "measuring.h"

#include <arm_math.h>
#include <stdio.h>
#include <stdlib.h>

// Defines
#define SPEED_OF_LIGHT 299792458.0
#define TRANSMIT_FREQUENCY 24000000000.0
#define FFT_SIZE 64
#define THRESHOLD 60

// Variables
float32_t dopplerFrequency;
arm_cfft_instance_f32 fftInstance;
float32_t max_value;
uint32_t max_index; // index at max value
extern float32_t HighVelocity = 0.0; // stores the highest Velocity

// Functions
void complex_fft(float32_t fft_input[]) {

    // Initialize the FFT instance
    arm_cfft_init_f32(&fftInstance, FFT_SIZE);

    // Perform the FFT, 0 indicates forward FFT, 1 enables bit reversal of output
    arm_cfft_f32(&fftInstance, cfft_inout, 0, 1);

}

void get_magnitude(void) {

    // magnitude calculation
    arm_cmplx_mag_f32(cfft_inout, testOutput, FFT_SIZE);

    // set DC value to 0 because we have an offset of 1.4V
    testOutput[0] = 0;
    testOutput[1] = 0;
    testOutput[2] = 0;
    testOutput[63] = 0;

    int size = 64;
    int mean = meanOfArray(testOutput, size);

    if(mean < THRESHOLD) {

    	for(int i = 0; i < size; i++) {
    		testOutput[i] = (float32_t) 0.0;
    	}
    }
}

uint32_t get_max_index(float32_t testOutput[]) {

    // get max value and corresponding index
    arm_max_f32(testOutput, FFT_SIZE, &max_value, &max_index);

    return max_index;

}

float32_t get_doppler_frequency(uint32_t max_index) {

    // calculate Doppler frequency by multiplying max_index by frequency resolution
    dopplerFrequency = (float32_t)max_index * ADC_FS / FFT_SIZE;

    // check if dopplerFrequency is in the second Nyquist zone -> > fs/2
    if (dopplerFrequency > (ADC_FS / 2)) {
    	dopplerFrequency = dopplerFrequency - ADC_FS;
    }

    return dopplerFrequency;

}

void update_high_velocity(float32_t velocity) {
	if (velocity > HighVelocity) {
		HighVelocity = velocity;
	}
}


float32_t calculate_speed(float32_t dopplerFrequency) {

    // Calculate velocity in m/s
    float32_t lambda = SPEED_OF_LIGHT / TRANSMIT_FREQUENCY;
    velocity = (dopplerFrequency*lambda) / 2.0f;

    // convert to m/s to km/h and round to accuracy +/- 0.3
    velocity = velocity*3.6;
    velocity = roundToAccuracy(velocity);

    // Update the high velocity
    update_high_velocity(velocity);

    return velocity;

}

int meanOfArray(float32_t testOutput[], int size) {

	int sum = 0;
	int average = 0;

	for(int i = 1; i < size ; i++) {
		sum += testOutput[i];
	}

	average = sum / size;

	return average;

}



