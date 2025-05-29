/**
******************************************************************************
* @author: InfinityVoid
* @date:   06/05/2025
* @file: task_list.h
******************************************************************************
**/

#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include "lt_task.h"

#include "task_life.h"
#include "task_net.h"
#include "task_gps.h"
#include "task_rfid.h"
#include "task_gsm.h"
#include "task_logic.h"
#include "task_display.h"
#include "task_polling.h"

enum {
    /* APP TASK */
    TASK_LIFE_ID,
    TASK_NETWORK_ID,
    TASK_DISPLAY_ID,
    TASK_GPS_ID,
    TASK_RFID_ID,
    TASK_GSM_ID,
    TASK_LOGIC_ID,
    TASK_POLLING_ID,
    /* END OF TASK TABLE */
    EOT_TASK_ID,
};

extern lt_sys_thread_t app_task_table[];

#endif /* __TASK_LIST_H__ */
