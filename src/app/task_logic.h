/**
******************************************************************************
* @author: InfinityVoid
* @date:   10/05/2025
* @file: task_logic.h
******************************************************************************
**/

#ifndef __TASK_LOGIC_H__
#define __TASK_LOGIC_H__

#include "app_signal.h"
#include "io_config.h"
#include "task_list.h"

#include "lt_task.h"
#include "lt_message.h"
#include "lt_timer.h"
#include "lt_log.h"
#include "lt_fsm.h"

#define UID_TAG_LENGTH              (4)

#define LOGIC_SIGNAL_FSM            (50)

/* internal state machine */
typedef enum {
    LOGIC_FSM_STUDENT_HANDLE = LOGIC_SIGNAL_FSM,
    LOGIC_FSM_BUS_DRIVER_HANDLE,
} logic_fsm_t;

extern void task_logic_handler(void* argv);

/* fsm logic */
extern fsm_t fsm_task_logic;
extern void fsm_logic_normal_hander(lt_msg_t* msg);
extern void fsm_logic_sos_hander(lt_msg_t* msg);
extern void fsm_logic_clr_all_state_handler(lt_msg_t* msg);

#endif /* __TASK_LOGIC_H__ */
