/**
 ******************************************************************************
 * @author: GaoDen
 * @date:   06/05/2025
 ******************************************************************************
**/

#include "screen_manager.h"

fsm_t screen_manager;
static lt_msg_t msg_entry;

void screen_manager_init(fsm_state_handler screen_initial) {
    fsm_init(&screen_manager, screen_initial);
    msg_entry.signal = SCREEN_ENTRY;
    fsm_dispatch(&screen_manager, &msg_entry);
}

void screen_manager_dispatch(lt_msg_t* msg) {
    fsm_dispatch(&screen_manager, msg);
}

void screen_manager_trans(fsm_state_handler screen_target) {
    fsm_trans(&screen_manager, screen_target);
    msg_entry.signal = SCREEN_ENTRY;
    fsm_dispatch(&screen_manager, &msg_entry);
}
