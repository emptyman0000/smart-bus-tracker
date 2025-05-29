/**
******************************************************************************
* @author: InfinityVoid
* @date:   07/05/2025
* @file: task_rfid.h
******************************************************************************
**/

#ifndef __TASK_RFID_H__
#define __TASK_RFID_H__

#include "MFRC522.h"
#include "SPI.h"

#include "student.h"
#include "bus_driver.h"

#include "app_signal.h"
#include "io_config.h"
#include "task_list.h"

#include "lt_task.h"
#include "lt_message.h"
#include "lt_timer.h"
#include "lt_log.h"

#define TIME_RESET_INIT             (20000)

extern void task_rfid_handler(void* argv); 

enum {
    RFID_READ_TAG,
};

#endif /* __TASK_RFID_H__ */