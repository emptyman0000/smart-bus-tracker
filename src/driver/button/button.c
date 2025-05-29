/**
 ******************************************************************************
 * @author: InfinityVoid
 * @date:   15/02/2025
 * @file: button.c
 ******************************************************************************
**/

#include "button.h"

void button_init(button_t* button, pf_button_read_state read_state, pf_button_callback callback) {
	button->button_enable = BUTTON_ENABLE;
	button->button_state = BUTTON_STATE_RELEASE;
	button->poll_count = 0;

	button->callback = callback;
	button->read_state = read_state;
}

void button_polling(button_t* button, uint8_t unit_inc) {
	if (button->button_enable == BUTTON_ENABLE) {
		uint32_t hw_button_state;

		hw_button_state = button->read_state();

		if (hw_button_state == BUTTON_HARDWARE_PRESS) {
			button->poll_count += unit_inc;

			if ((button->poll_count >= BUTTON_DEBOUNCE_TIME) && (button->button_state == BUTTON_STATE_RELEASE)) {
				button->button_enable = BUTTON_DISABLE;
				button->button_state = BUTTON_STATE_PRESS;
				button->callback(&(button->button_state));
				button->button_enable = BUTTON_ENABLE;
			}
			else if ((button->poll_count >= BUTTON_LONG_PRESS_TIME) && (button->button_state == BUTTON_STATE_PRESS)) {
				button->button_enable = BUTTON_DISABLE;
				button->button_state = BUTTON_STATE_LONG_PRESS;
				button->callback(&(button->button_state));
				button->button_enable = BUTTON_ENABLE;
			}
		}
		else {
			if ((button->poll_count >= BUTTON_DEBOUNCE_TIME) && (button->button_state != BUTTON_STATE_RELEASE)) {
				button->button_enable = BUTTON_DISABLE;
				button->button_state = BUTTON_STATE_RELEASE;
				button->callback(&(button->button_state));
			}

			/* reset */
			button->poll_count = 0;
			button->button_enable = BUTTON_ENABLE;
		}
	}
}
