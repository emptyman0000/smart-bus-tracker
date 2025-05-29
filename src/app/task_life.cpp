/**
 ******************************************************************************
 * @scr: GaoDen
 * @author: InfinityVoid
 * @date:   05/05/2025
 * @file: task_life.cpp
 ******************************************************************************
**/

#include "task_life.h"

#include "lt_task.h"
#include "lt_message.h"
#include "lt_timer.h"
#include "lt_log.h"

#include "io_config.h"

#include "app_signal.h"
#include "task_list.h"

led_t led_life;

void task_life_handler(void* argv) {
    /* wait all task active */
    waiting_active_object_ready();
    
    /* queue message */
    uint8_t task_id = *(uint8_t*)argv;
    lt_msg_t msg;

    while (task_rev_msg(task_id, &msg) == LT_THREAD_OK) {
        switch (msg.signal) {
        case SYS_LIFE_SYSTEM_CHECK: {
            // led_toggle(&led_life);
            
            // printf("[task_life] SYS_LIFE_SYSTEM_CHECK\n");
        }
            break;

        case SYS_CTRL_REBOOT: {
            LOG("[task_life] SYS_CTRL_REBOOT\n");
            esp_restart();
        }
            break;
        
        default:
            break;
        }

        /* free message */
        task_free_msg(msg.data);
    }
}
