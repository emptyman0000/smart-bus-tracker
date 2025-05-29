/**
 ******************************************************************************
 * @author: GaoDen
 * @date:   19/02/2025
 ******************************************************************************
**/

#include "lt_timer.h"

#include "lt_task.h"
#include "lt_message.h"
#include "lt_log.h"

/* lt timer list */
static lt_timer_node_t* lt_timer_list_head = LT_TIMER_NODE_NULL;

void timer_callback(os_timer_handle_t timer_handle) {
    SYS_ASSERT(timer_handle != NULL);
    lt_timer_node_t* get_timer = (lt_timer_node_t*)pvTimerGetTimerID(timer_handle);
    if (get_timer == NULL) {
        LOGE("[timer callback] get_timer NULL!");
        return;
    }

    /* dispatch message */
    SYS_ASSERT(task_post_pure_msg(get_timer->lt_timer.des_task_id, get_timer->lt_timer.signal) == LT_THREAD_OK);

    /* timer list update */
    if (get_timer->lt_timer.type == TIMER_ONE_SHOT) {
        timer_remove(get_timer->lt_timer.des_task_id, get_timer->lt_timer.signal);
    }
}

void timer_set(uint8_t task_id, uint8_t signal, uint32_t period, timer_type_t type) {
    lt_timer_node_t* current_node = lt_timer_list_head;

    while (current_node != LT_TIMER_NODE_NULL) {
        if (current_node->lt_timer.des_task_id == task_id && current_node->lt_timer.signal == signal) {
            xTimerChangePeriod(current_node->os_timer_handle, pdMS_TO_TICKS(period), 0);
            return;
        }
        current_node = current_node->next;
    }

    /* timer node allocate */
    lt_timer_node_t* new_node = (lt_timer_node_t*)malloc(sizeof(lt_timer_node_t));
    SYS_ASSERT(new_node != NULL);
    new_node->lt_timer.des_task_id = task_id;
    new_node->lt_timer.signal = signal;
    new_node->lt_timer.type = type;
    new_node->lt_timer.period = period;

    /* timer node start */
    new_node->os_timer_handle = xTimerCreate("lt_timer", pdMS_TO_TICKS(period), (type == TIMER_PERIODIC) ? pdTRUE : pdFALSE, (void*)new_node, timer_callback);
    xTimerStart(new_node->os_timer_handle, pdMS_TO_TICKS(100));

    /* timer list uppdate */
    new_node->next = lt_timer_list_head;
    lt_timer_list_head = new_node;
}

void timer_remove(uint8_t task_id, uint8_t signal) {
    lt_timer_node_t* current_node = lt_timer_list_head;
    lt_timer_node_t* previous_node = LT_TIMER_NODE_NULL;

    while (current_node != LT_TIMER_NODE_NULL) {
        if ((current_node->lt_timer.des_task_id == task_id) && (current_node->lt_timer.signal == signal)) {
            if (previous_node == LT_TIMER_NODE_NULL) {
                lt_timer_list_head = current_node->next;
            } 
            else {
                previous_node->next = current_node->next;
            }

            /* timer node delete */
            xTimerDelete(current_node->os_timer_handle, 0);
            vPortFree(current_node);
            return;
        }
        previous_node = current_node;
        current_node = current_node->next;
    }
}
