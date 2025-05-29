
/**
 ******************************************************************************
 * @author: InfinityVoid
 * @date:   15/02/2025
 * @file: app_bsc.h
 ******************************************************************************
**/

#ifndef __APP_BSC_H__
#define __APP_BSC_H__

#include <stdint.h>

#include "button.h"

#include "task_list.h"
#include "app_signal.h"
#include "lt_task.h"

extern button_t sos_button;
extern uint8_t call_signal;

extern void button_callback(void* btn_callback);

#endif /* __APP_BSC_H__ */
