/**
 ******************************************************************************
 * @author: GaoDen
 * @date:   17/02/2025
 * @brief: event-driven service built on top FreeRTOS
 ******************************************************************************
**/

#ifndef __LT_TASK_H__
#define __LT_TASK_H__

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lt_message.h"

#define LITE_THREAD_KERNEL_VERSION      "1.3"
#define PTHREAD_NULL                    ((pf_thread)0)

/* RTOS task re-define */
typedef TaskFunction_t pf_thread;
typedef QueueHandle_t task_queue_t;

typedef enum {
    TASK_PRIORITY_LEVEL_0 = tskIDLE_PRIORITY,
    TASK_PRIORITY_LEVEL_1,
    TASK_PRIORITY_LEVEL_2,
    TASK_PRIORITY_LEVEL_3,
    TASK_PRIORITY_LEVEL_4,
    TASK_PRIORITY_LEVEL_5,
    TASK_PRIORITY_LEVEL_6,
    TASK_PRIORITY_LEVEL_7,
} task_priority_t;

typedef struct {
    uint8_t task_id;
    pf_thread pthread;
    task_priority_t task_priority;
    uint32_t stack_size;
    uint16_t queue_size;
    const char* task_info;
    void* task_queue;
} lt_sys_thread_t;

/* task initinit */
extern void task_init(lt_sys_thread_t* task_table);
extern void waiting_active_object_ready();

/* task post message */
extern uint8_t task_post_pure_msg(uint8_t des_task_id, uint8_t signal);
extern uint8_t task_post_pure_msg_from_isr(uint8_t des_task_id, uint8_t signal);
extern uint8_t task_post_common_msg(uint8_t des_task_id, uint8_t signal, uint8_t* data, uint32_t size);
extern uint8_t task_post_common_msg_from_isr(uint8_t des_task_id, uint8_t signal, uint8_t* data, uint32_t size);
extern uint8_t task_rev_msg(uint8_t task_id, lt_msg_t* msg);
extern void task_free_msg(void *data);
extern uint8_t* get_data_common_msg(lt_msg_t* msg);

#ifdef __cplusplus
}
#endif

#endif /* __LT_TASK_H__ */
