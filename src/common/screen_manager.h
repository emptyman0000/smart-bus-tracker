/**
 ******************************************************************************
 * @author: GaoDen
 * @date:   06/05/2025
 ******************************************************************************
**/

#ifndef __SCREEN_MANAGER_H__
#define __SCREEN_MANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h> 
#include <stdint.h>

#include "lt_message.h"
#include "lt_fsm.h"

#define SCREEN_ENTRY            (0xFE)

extern void screen_manager_init(fsm_state_handler screen_initial);
extern void screen_manager_dispatch(lt_msg_t* msg);
extern void screen_manager_trans(fsm_state_handler screen_target);

#ifdef __cplusplus
}
#endif

#endif /* __SCREEN_MANAGER_H__ */
