/** ***************************************************************************
 * @file
 * @brief The menu
 *
 * Initializes and displays the menu.
 * @n Provides the function MENU_check_transition() for polling user actions.
 * The variable MENU_transition is set to the touched menu item.
 * If no touch has occurred the variable MENU_transition is set to MENU_NONE
 * @n If the interrupt handler is enabled by calling BSP_TS_ITConfig();
 * the variable MENU_transition is set to the touched menu entry as above.
 * @n Either call once BSP_TS_ITConfig() to enable the interrupt
 * or MENU_check_transition() in the main while loop for polling.
 * @n The function MENU_get_transition() returns the new menu item.
 *
 * @author  Hanspeter Hochreutener, hhrt@zhaw.ch
 * @date	30.04.2020
 *****************************************************************************/


/******************************************************************************
 * Includes
 *****************************************************************************/
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"

#include "main.h"

#include "menu.h"


/******************************************************************************
 * Defines
 *****************************************************************************/
#define MENU_FONT				&Font12	///< Possible font sizes: 8 12 16 20 24
#define MENU_HEIGHT				40		///< Height of menu bar
#define MENU_MARGIN				2		///< Margin around a menu entry
/** Position of menu bar: 0 = top, (BSP_LCD_GetYSize()-MENU_HEIGHT) = bottom */
#define MENU_Y					(BSP_LCD_GetYSize()-MENU_HEIGHT)


/******************************************************************************
 * Variables
 *****************************************************************************/
static MENU_item_t MENU_transition = MENU_NONE;	///< Transition to this menu
static MENU_entry_t MENU_entry[MENU_ENTRY_COUNT] = {
		{"sin-",	"gle",		LCD_COLOR_BLACK,	LCD_COLOR_LIGHTBLUE},
		{"Timer",	"+IRQ",		LCD_COLOR_BLACK,	LCD_COLOR_LIGHTGREEN},
		{"DMA",	    "+IRQ",		LCD_COLOR_BLACK,	LCD_COLOR_LIGHTRED},
		{"DMA",	    "dual",		LCD_COLOR_BLACK,	LCD_COLOR_LIGHTCYAN},
		{"DMA",	    "scan",		LCD_COLOR_BLACK,	LCD_COLOR_LIGHTMAGENTA},
		{"DMA",	    "scan_",	LCD_COLOR_BLACK,	LCD_COLOR_LIGHTYELLOW}
};										///< All the menu entries


/******************************************************************************
 * Functions
 *****************************************************************************/


/** ***************************************************************************
 * @brief Draw the menu onto the display.
 *
 * Each menu entry has two lines.
 * Text and background colors are applied.
 * @n These attributes are defined in the variable MENU_draw[].
 *****************************************************************************/
void MENU_draw(void)
{
	BSP_LCD_SetFont(MENU_FONT);
	uint32_t x, y, m, w, h;
	y = MENU_Y;
	m = MENU_MARGIN;
	w = BSP_LCD_GetXSize()/MENU_ENTRY_COUNT;
	h = MENU_HEIGHT;
	for (uint32_t i = 0; i < MENU_ENTRY_COUNT; i++) {
		x = i*w;
		BSP_LCD_SetTextColor(MENU_entry[i].back_color);
		BSP_LCD_FillRect(x+m, y+m, w-2*m, h-2*m);
		BSP_LCD_SetBackColor(MENU_entry[i].back_color);
		BSP_LCD_SetTextColor(MENU_entry[i].text_color);
		BSP_LCD_DisplayStringAt(x+3*m, y+3*m,
				(uint8_t *)MENU_entry[i].line1, LEFT_MODE);
		BSP_LCD_DisplayStringAt(x+3*m, y+h/2,
				(uint8_t *)MENU_entry[i].line2, LEFT_MODE);
	}
}


/** ***************************************************************************
 * @brief Shows a hint at startup.
 *
 *****************************************************************************/
void MENU_hint(void)
{
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(5,10, (uint8_t *)"DEMO-CODE", LEFT_MODE);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(5, 60, (uint8_t *)"Touch a menu item", LEFT_MODE);
	BSP_LCD_DisplayStringAt(5, 80, (uint8_t *)"to start an ADC demo", LEFT_MODE);
	BSP_LCD_DisplayStringAt(5, 110, (uint8_t *)"Switch DAC on/off", LEFT_MODE);
	BSP_LCD_DisplayStringAt(5, 130, (uint8_t *)"with blue pushbutton", LEFT_MODE);
	BSP_LCD_DisplayStringAt(5, 160, (uint8_t *)"(c) hhrt@zhaw.ch", LEFT_MODE);
	BSP_LCD_DisplayStringAt(5, 160, (uint8_t *)"Version 27.09.2022", LEFT_MODE);
}


/** ***************************************************************************
 * @brief Set a menu entry.
 * @param [in] item number of menu bar
 * @param [in] entry attributes for that item
 *
 * @note Call MENU_draw() to update the display.
 *****************************************************************************/
void MENU_set_entry(const MENU_item_t item, const MENU_entry_t entry)
{
	if ((0 <= item) && (MENU_ENTRY_COUNT > item)) {
		MENU_entry[item] = entry;
	}
}


/** ***************************************************************************
 * @brief Get a menu entry.
 * @param [in] item number of menu bar
 * @return Menu_entry[item] or Menu_entry[0] if item not in range
 *****************************************************************************/
MENU_entry_t MENU_get_entry(const MENU_item_t item)
{
	MENU_entry_t entry = MENU_entry[0];
	if ((0 <= item) && (MENU_ENTRY_COUNT > item)) {
		entry = MENU_entry[item];
	}
	return entry;
}


/** ***************************************************************************
 * @brief Get menu selection/transition
 *
 * @return the selected MENU_item or MENU_NONE if no MENU_item was selected
 *
 * MENU_transition is used as a flag.
 * When the value is read by calling MENU_get_transition()
 * this flag is cleared, respectively set to MENU_NONE.
 *****************************************************************************/
MENU_item_t MENU_get_transition(void)
{
	MENU_item_t item = MENU_transition;
	MENU_transition = MENU_NONE;
	return item;
}


/** ***************************************************************************
 * @brief Check for selection/transition
 *
 * If the last transition has been consumed (MENU_NONE == MENU_transition)
 * and the touchscreen has been touched for a defined period
 * the variable MENU_transition is set to the touched item.
 * @note  Evalboard revision E (blue PCB) has an inverted y-axis
 * in the touch controller compared to the display.
 * Uncomment or comment the <b>\#define EVAL_REV_E</b> in main.h accordingly.
 *****************************************************************************/
void MENU_check_transition(void)
{
	static MENU_item_t item_old = MENU_NONE;
	static MENU_item_t item_new = MENU_NONE;
	static TS_StateTypeDef  TS_State;	// State of the touch controller
	BSP_TS_GetState(&TS_State);			// Get the state


// Evalboard revision E (blue) has an inverted y-axis in the touch controller
#ifdef EVAL_REV_E
	TS_State.Y = BSP_LCD_GetYSize() - TS_State.Y;	// Invert the y-axis
#endif
	// Invert x- and y-axis if LCD ist flipped
#ifdef FLIPPED_LCD
	TS_State.X = BSP_LCD_GetXSize() - TS_State.X;	// Invert the x-axis
	TS_State.Y = BSP_LCD_GetYSize() - TS_State.Y;	// Invert the y-axis
#endif


/*
	#if (defined(EVAL_REV_E) && !defined(FLIPPED_LCD)) || (!defined(EVAL_REV_E) && defined(FLIPPED_LCD))
	TS_State.Y = BSP_LCD_GetYSize() - TS_State.Y;	// Invert the y-axis
#endif
#ifdef EVAL_REV_E
#endif
*/
	if (TS_State.TouchDetected) {		// If a touch was detected
		/* Do only if last transition not pending anymore */
		if (MENU_NONE == MENU_transition) {
			item_old = item_new;		// Store old item
			/* If touched within the menu bar? */
			if ((MENU_Y < TS_State.Y) && (MENU_Y+MENU_HEIGHT > TS_State.Y)) {
				item_new = TS_State.X	// Calculate new item
						/ (BSP_LCD_GetXSize()/MENU_ENTRY_COUNT);
				if ((0 > item_new) || (MENU_ENTRY_COUNT <= item_new)) {
					item_new = MENU_NONE;	// Out of bounds
				}
				if (item_new == item_old) {	// 2 times the same menu item
					item_new = MENU_NONE;
					MENU_transition = item_old;
				}
			}
		}
	}
}



/** ***************************************************************************
 * @brief Interrupt handler for the touchscreen
 *
 * @note BSP_TS_ITConfig(); must be called in the main function
 * to enable touchscreen interrupt.
 * @note There are timing issues when interrupt is enabled.
 * It seems that polling is the better choice with this evaluation board.
 * @n Call MENU_check_transition() from the while loop in main for polling.
 *
 * The touchscreen interrupt is connected to PA15.
 * @n The interrupt handler for external line 15 to 10 is called.
 *****************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR15) {		// Check if interrupt on touchscreen
		EXTI->PR |= EXTI_PR_PR15;		// Clear pending interrupt on line 15
		if (BSP_TS_ITGetStatus()) {		// Get interrupt status
			BSP_TS_ITClear();				// Clear touchscreen controller int.
			MENU_check_transition();
		}
		EXTI->PR |= EXTI_PR_PR15;		// Clear pending interrupt on line 15
	}
}

