/**
******************************************************************************
* @author: InfinityVoid
* @date:   07/05/2025
* @file: task_gps.cpp
******************************************************************************
**/

#include "task_gps.h"

HardwareSerial gpsSerial(2);
TinyGPSPlus gps;


static gps_data_t gps_data;

static gps_status_t gps_status = GPS_LOST;

static gps_datetime_t date_time = {
    "00/00/0000",
    "00:00:00"
};

void gps_update_data();

void task_gps_handler(void* argv) {
    /* wait all task active */
    waiting_active_object_ready();
 
    /* queue message */
    uint8_t task_id = *(uint8_t*)argv;
    lt_msg_t msg;

    /* gps driver init */
    gpsSerial.begin(GPS_NEO_M10_BAUDRATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

    while(task_rev_msg(task_id, &msg) == LT_THREAD_OK) {
        switch (msg.signal)
        {
            case GPS_INIT: {
                /* timer update gps data */
                timer_set(TASK_GPS_ID, GPS_SYNC_DATA_CLOUD, SAMPLING_PERIOD_SYNC_FIREBASE , TIMER_PERIODIC);
                timer_set(TASK_GPS_ID, GPS_GET_DATE_TIME,   SAMPLING_PERIOD_SYNC_DISPLAY  , TIMER_PERIODIC);
            }
                break;

            case GPS_SYNC_DATA_CLOUD: {
                gps_update_data();
                /* send gps data to task network */
                task_post_common_msg(TASK_NETWORK_ID, NET_FIREBASE_POST_DATA_GPS, (uint8_t*)&gps_data, sizeof(gps_data));
            }
                break;

            case GPS_GET_DATE_TIME: {
                gps_update_data();
                /* send gps data to task display */
                task_post_pure_msg(TASK_DISPLAY_ID, DISPLAY_GPS);
            }
                break;

            default: {

            }

                break;
        }

    /* free msg data */
    task_free_msg(msg.data);
    }   
}

void gps_update_data() {
    /* checking data gps */
    while (gpsSerial.available()) {
        char c = gpsSerial.read();
        gps.encode(c);
    }

     /* check gps update */
    if (gps.location.isValid()) {
        gps_status = GPS_OK;
        
        if (gps.location.isUpdated()) {

            /* get location */
            gps_data.latitude = gps.location.lat();
            gps_data.longitude = gps.location.lng();

            /* UTC -> GMT+7 timezone */
            uint8_t vn_hour = (gps.time.hour() + TIMEZONE_VN) % 24;
            snprintf(gps_data.time, sizeof(gps_data.time), "%02u:%02u:%02u", vn_hour, gps.time.minute(), gps.time.second());
            snprintf(gps_data.date, sizeof(gps_data.date), "%02u/%02u/%04u", gps.date.day(), gps.date.month(), gps.date.year());

            memcpy(date_time.date, gps_data.date, sizeof(date_time.date));
            memcpy(date_time.time, gps_data.time, sizeof(date_time.time));

        }
        
    } 
    else {
        gps_status = GPS_LOST;

        gps_data.latitude = NO_LOCATION;
        gps_data.longitude = NO_LOCATION;

        snprintf(gps_data.time, sizeof(gps_data.time), "No time");
        snprintf(gps_data.date, sizeof(gps_data.date), "No date");

        memcpy(date_time.date, gps_data.date, sizeof(date_time.date));
        memcpy(date_time.time, gps_data.time, sizeof(date_time.time));
    }
    
}

gps_status_t gps_get_status() {
    return gps_status;
}

gps_datetime_t gps_get_datetime() {
    return date_time;
}
