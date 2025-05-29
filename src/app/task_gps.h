/**
******************************************************************************
* @author: InfinityVoid
* @date:   07/05/2025
* @file: task_gps.h
******************************************************************************
**/

#ifndef __TASK_GPS_H__
#define __TASK_GPS_H__

#define TIMEZONE_VN           (7)
#define NO_LOCATION           double(0)

#include "TinyGPS++.h"
#include "HardwareSerial.h"

#include "app_signal.h"
#include "io_config.h"
#include "task_list.h"

#include "lt_task.h"
#include "lt_message.h"
#include "lt_timer.h"
#include "lt_log.h"

#define SAMPLING_PERIOD_SYNC_FIREBASE     (2500)
#define SAMPLING_PERIOD_SYNC_DISPLAY      (1000)

typedef enum {
    GPS_LOST,
    GPS_OK,
} gps_status_t;

typedef struct {
    char date[20];
    char time[20];
} gps_datetime_t;

typedef struct {
    double latitude;
    double longitude;
    char date[20];
    char time[20];
} gps_data_t;

extern void task_gps_handler(void* argv);
extern gps_status_t gps_get_status();
extern gps_datetime_t gps_get_datetime();

#endif /* __TASK_GPS_H__ */
