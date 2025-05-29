/**
******************************************************************************
* @author: ZenVoid
* @date:   06/05/2025
* @file: app_signal.h
******************************************************************************
**/

#ifndef __APP_SIGNAL_H__
#define __APP_SIGNAL_H__

#include <stdint.h>
#include "lt_config.h"

enum {
    /* TASK LIFE SIGNALS */
    SYS_LIFE_SYSTEM_CHECK = LT_USER_DEFINE_SIGNAL,
    SYS_CTRL_REBOOT,
    SYS_SYSTEM_INIT, 

    /* TASK NETWORK SIGNALS */
    NET_WIFI_INIT, 
    NET_FIREBASE_INIT,
    NET_FIREBASE_POST_DATA_GPS,
    NET_FIREBASE_POST_DATA_DRIVER,
    NET_FIREBASE_POST_DATA_STUDENT,
    NET_FIREBASE_GET_DATA_STUDENT,
    NET_FIREBASE_GET_DATA,

    /* TASK GPS SIGNALS */
    GPS_INIT,
    GPS_SYNC_DATA_CLOUD,
    GPS_GET_DATE_TIME,
    
    /* TASK RFID SIGNALS */
    RFID_INIT,
    RFID_REINIT,

    /* TASK LOGIC SIGNALS */
    LOGIC_VERIFY_UID_TAG,
    LOGIC_SOS_TIMEOUT,

    /* TASK GSM SIGNAL */
    GSM_INIT,
    GSM_SEND_SMS,
    GSM_CALL_SOS,
	GSM_BREAK_CALL,
    

    /* TASK DISPLAY SIGNALS */
    DISPLAY_INIT,
    DISPLAY_SCREEN_MAIN_UPDATE,
    DISPLAY_SCREEN_MAIN_ANIMATION,
    DISPLAY_GPS,
    DISPLAY_WARNING,
    DISPLAY_STUDENT_REMAIN,
    DISPLAY_DRIVER_CHECKIN,
    DISPLAY_STUDENT_CHECKIN,
    
    DISPLAY_SOS_CALL_DRIVER,
    DISPLAY_SOS_CALL_PARENT,
    DISPLAY_CALL_ENDED,

    /* BSP */
    BSP_BUTTON_CFG_PRESSED,
    BSP_BUTTON_CFG_LONG_PRESSED,
    BSP_BUTTON_CFG_RELEASED,
};

#endif /* __APP_SIGNAL_H__ */
