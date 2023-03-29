/** ***************************************************************************
 * @file
 * @brief See menu.c
 *
 * Prefix MENU
 *
 *****************************************************************************/

#ifndef MENU_H_
#define MENU_H_


/******************************************************************************
 * Includes
 *****************************************************************************/
#include "stdint.h"


/******************************************************************************
 * Defines
 *****************************************************************************/
#define MENU_ENTRY_COUNT		6		///< Number of menu entries


/******************************************************************************
 * Types
 *****************************************************************************/
/** Enumeration of possible menu items */
typedef enum {
	MENU_ZERO = 0, MENU_ONE, MENU_TWO, MENU_THREE, MENU_FOUR, MENU_FIVE, MENU_NONE
} MENU_item_t;
/** Struct with fields of a menu entry */
typedef struct {
	char line1[16];						///< First line of menu text
	char line2[16];						///< Second line of menu text
	uint32_t text_color;				///< Text color
	uint32_t back_color;				///< Background color
} MENU_entry_t;


/******************************************************************************
 * Functions
 *****************************************************************************/
void MENU_draw(void);
void MENU_hint(void);
void MENU_set_entry(const MENU_item_t item, const MENU_entry_t entry);
MENU_entry_t MENU_get_entry(const MENU_item_t item);
void MENU_check_transition(void);
MENU_item_t MENU_get_transition(void);


#endif

