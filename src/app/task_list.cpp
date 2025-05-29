/**
 ******************************************************************************
 * @scr: GaoDen
 * @author: InfinityVoid
 * @date:   25/03/2025
 * @file: task_list.cpp
 ******************************************************************************
**/

#include "task_list.h"

lt_sys_thread_t app_task_table[] = {
    /**************************************************************************************************************************/
    /* APP TASK */
    /**************************************************************************************************************************/
    {TASK_LIFE_ID,      task_life_handler,      TASK_PRIORITY_LEVEL_5,      4096,   4,   "task life",       NULL},
    {TASK_NETWORK_ID,   task_net_handler,       TASK_PRIORITY_LEVEL_4,      32767,  16,   "task network",   NULL},
    {TASK_DISPLAY_ID,   task_display_handler,   TASK_PRIORITY_LEVEL_4,      16384,  16,   "task tft",       NULL},
    {TASK_GPS_ID,       task_gps_handler,       TASK_PRIORITY_LEVEL_2,      4096,    8,   "task gps",       NULL},
    {TASK_RFID_ID,      task_rfid_handler,      TASK_PRIORITY_LEVEL_2,      4096,    4,   "task rfid",      NULL},
    {TASK_GSM_ID,       task_gsm_handler,       TASK_PRIORITY_LEVEL_4,      4096,    4,   "task gsm",       NULL},
    {TASK_LOGIC_ID,     task_logic_handler,     TASK_PRIORITY_LEVEL_3,      8196,   16,   "task logic",     NULL},
    {TASK_POLLING_ID,   task_polling_handler,   TASK_PRIORITY_LEVEL_2,      2048,    0,   "task polling",   NULL},

    /**************************************************************************************************************************/
    /* END OF TABLE */
    /**************************************************************************************************************************/
    {EOT_TASK_ID,         PTHREAD_NULL,         TASK_PRIORITY_LEVEL_0,       0,     0,   "end task list",  NULL},
};
