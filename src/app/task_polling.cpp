/**
 ******************************************************************************
 * @scr: GaoDen
 * @author: InfinityVoid
 * @date:   05/05/2025
 * @file: task_polling.cpp
 ******************************************************************************
**/

#include "task_polling.h"

#include "lt_task.h"
#include "lt_message.h"
#include "lt_timer.h"
#include "lt_log.h"

#include "io_config.h"
#include "app_signal.h"

#include "led.h"
#include "app_bsc.h"

#include "task_list.h"

#define HARDWARE_POLLING_PERIOD         (10)
#define COUNTER_UNIT                    (10)

void task_polling_handler(void* argv) {
    /* wait all task active */
    waiting_active_object_ready();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(HARDWARE_POLLING_PERIOD));

        /* hardware driver polling */
        led_polling(&led_life, COUNTER_UNIT);
        button_polling(&sos_button, COUNTER_UNIT);
    }
    
}
