/**
******************************************************************************
* @author: InfinityVoid
* @date:   10/05/2025
* @file: task_gps.h
******************************************************************************
**/

#ifndef __TASK_GSM_H__
#define __TASK_GSM_H__

#include "TinyGPS++.h"
#include "HardwareSerial.h"

#include "app_signal.h"
#include "io_config.h"
#include "task_list.h"

#include "lt_task.h"
#include "lt_message.h"
#include "lt_timer.h"
#include "lt_log.h"

typedef enum {
    CALL_DRIVER_ATTEMPT_1,
    CALL_DRIVER_ATTEMPT_2,
    CALL_PARENT,
} call_action_t;

#define WAIT_CALL_TIMEOUT       (90000)
extern void task_gsm_handler(void* argv);

#endif /* __TASK_GSM_H__ */
