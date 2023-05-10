/*
 * speed_model.h
 *
 *  Created on: May 9, 2023
 *      Author: diegobelusky
 */

#ifndef INC_SPEED_MODEL_H_
#define INC_SPEED_MODEL_H_

void complex_fft(float32_t*);
void get_magnitude(void);
uint32_t get_max_index(float32_t*);
float32_t get_doppler_frequency(uint32_t);
float32_t calculate_speed(float32_t);

#endif /* INC_SPEED_MODEL_H_ */
