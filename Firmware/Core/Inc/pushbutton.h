/** ***************************************************************************
 * @file
 * @brief See pushbutton.c
 *
 * Prefix: PB
 *
 *****************************************************************************/

#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_


/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdbool.h>


/******************************************************************************
 * Defines
 *****************************************************************************/


/******************************************************************************
 * Functions
 *****************************************************************************/
void PB_init(void);
void PB_enableIRQ(void);
bool PB_pressed(void);


#endif
