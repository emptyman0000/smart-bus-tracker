/**
******************************************************************************
* @author: InfinityVoid
* @date:   13/05/2025
* @file: task_display.h
******************************************************************************
**/

#ifndef __TASK_DISPLAY_H__
#define __TASK_DISPLAY_H__

#include "TFT_eSPI.h"

#include "student.h"
#include "bus_driver.h"

#include "app_signal.h"
#include "io_config.h"
#include "task_list.h"

#include "lt_task.h"
#include "lt_message.h"
#include "lt_timer.h"
#include "lt_log.h"

#define RETURN_TIMEOUT      (2500)

extern void task_display_handler(void* argv); 
extern TFT_eSPI tft;

#endif /* __TASK_DISPLAY_H__ */ 
