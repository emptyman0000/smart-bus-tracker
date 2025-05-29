/**
 ******************************************************************************
 * @author: InfinityVoid
 * @date:   15/02/2025
 * @file: app_bsc.c
 ******************************************************************************
**/

#include "button.h"
#include "io_config.h"
#include "app_bsc.h"

button_t sos_button;

void button_callback(void* btn_callback) {

	uint8_t* button_state = (uint8_t*)(btn_callback);

	switch (*button_state)
	{
	case BUTTON_STATE_PRESS: {
		buzzer_alarm(100);

		/* sos req */
		fsm_trans(&fsm_task_logic, fsm_logic_sos_hander);
		timer_set(TASK_LOGIC_ID, LOGIC_SOS_TIMEOUT, 10000, TIMER_ONE_SHOT);
		
		/* trans dsp */
		task_post_pure_msg(TASK_DISPLAY_ID, BSP_BUTTON_CFG_PRESSED);
		task_post_pure_msg(TASK_GSM_ID, GSM_BREAK_CALL);
	}
		break;

	case BUTTON_STATE_LONG_PRESS: {
		buzzer_alarm(500);

		fsm_trans(&fsm_task_logic, fsm_logic_normal_hander);
        timer_remove(TASK_LOGIC_ID, LOGIC_SOS_TIMEOUT);

		/* clr req */
		bus_driver_t* bus_driver = bus_driver_info_entry();
		bus_driver->state = CONFIRM_RESET_ALL;
		/* may be this action to send signal to TFT call to number phone custom */   
		task_post_pure_msg(TASK_DISPLAY_ID, BSP_BUTTON_CFG_LONG_PRESSED);
	}
		break;
		
	case BUTTON_STATE_RELEASE: {

	}
		break;
	
	default: {
		
	}
		break;
	}
	
}
