/**
******************************************************************************
* @author: InfinityVoid
* @date:   13/05/2025
* @file: task_tft.cpp
******************************************************************************
**/

#include "task_display.h"

#include "screen.h"

TFT_eSPI tft = TFT_eSPI();

void display_init();
void display_main_screen();
void display_gps();
void display_invalid_tag();
void display_driver_checkin();
void display_sos_detect();
void display_calling_driver();
void display_calling_parent();
void display_call_ended();
void display_require_reset();  
void display_student_checkin(lt_msg_t* msg); 
void display_student_remain(lt_msg_t* msg); 

void task_display_handler(void* argv) {
    /* wait all task active */
    waiting_active_object_ready();

    /* queue */
    uint8_t task_id = *(uint8_t*)argv;
    lt_msg_t msg;

    /* driver init */
    display_init();
    timer_set(TASK_DISPLAY_ID, DISPLAY_INIT, 2000, TIMER_ONE_SHOT);
    
    while (task_rev_msg(task_id, &msg) == LT_THREAD_OK) {
        switch (msg.signal)
            {
            case DISPLAY_INIT: { 
                display_main_screen();

                // timer_remove(TASK_DISPLAY_ID, DISPLAY_WARNING);
                // timer_remove(TASK_DISPLAY_ID, DISPLAY_DRIVER_CHECKIN);
                // timer_remove(TASK_DISPLAY_ID, DISPLAY_STUDENT_CHECKIN);
                // timer_remove(TASK_DISPLAY_ID, DISPLAY_SOS_CALL_DRIVER);
                // timer_remove(TASK_DISPLAY_ID, DISPLAY_SOS_CALL_PARENT);
                // timer_remove(TASK_DISPLAY_ID, DISPLAY_CALL_ENDED);
                // timer_remove(TASK_DISPLAY_ID, BSP_BUTTON_CFG_PRESSED);
                // timer_remove(TASK_DISPLAY_ID, BSP_BUTTON_CFG_LONG_PRESSED);
            }
                break;
            
            case DISPLAY_GPS: {
                display_gps();
            }
                break;

            case DISPLAY_WARNING: {
                display_invalid_tag();

                timer_set(TASK_DISPLAY_ID, DISPLAY_INIT, RETURN_TIMEOUT, TIMER_ONE_SHOT);

            }
                break;
            
            case DISPLAY_STUDENT_REMAIN: {
                display_student_remain(&msg);

                timer_set(TASK_DISPLAY_ID, DISPLAY_INIT, RETURN_TIMEOUT, TIMER_ONE_SHOT);

            }
                break;

            case DISPLAY_DRIVER_CHECKIN: {
                display_driver_checkin();

                timer_set(TASK_DISPLAY_ID, DISPLAY_INIT, RETURN_TIMEOUT, TIMER_ONE_SHOT);
            }
                break;

            case DISPLAY_STUDENT_CHECKIN: {
                display_student_checkin(&msg);
  
                timer_set(TASK_DISPLAY_ID, DISPLAY_INIT, RETURN_TIMEOUT, TIMER_ONE_SHOT);
            }
                break;

            case DISPLAY_SOS_CALL_DRIVER: {
                display_calling_driver();
            }
                break;

            case DISPLAY_SOS_CALL_PARENT: {
                display_calling_parent();
            }
                break;
            
            case DISPLAY_CALL_ENDED: {
                display_call_ended();

                timer_set(TASK_DISPLAY_ID, DISPLAY_INIT, RETURN_TIMEOUT, TIMER_ONE_SHOT);

            }
                break;

            case BSP_BUTTON_CFG_PRESSED: {
                display_sos_detect();
            }
                break;

            case BSP_BUTTON_CFG_LONG_PRESSED: {
                display_require_reset();

                timer_set(TASK_DISPLAY_ID, DISPLAY_INIT, RETURN_TIMEOUT, TIMER_ONE_SHOT);
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

void display_init() {
    tft.begin();
    tft.setRotation(3); 
    tft.setSwapBytes(true);

    /* hcmute background */
    tft.pushImage(0, 0, 480, 320, hcmute_background); 
}

void show_spinner_animation() {
    int centerX = 240;
    int centerY = 160;
    int radius = 20;
    int dot_radius = 3;

    tft.fillScreen(TFT_WHITE);
    
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(170, 80);
    tft.print("LOADING...");

    for (int frame = 0; frame < 15; frame++) {
        for (int i = 0; i < 12; i++) {
            /* Calculate angle */
            float angle = i * 30 * 3.1416 / 180.0;

            /* Calculate position */
            int x = centerX + radius * cos(angle);
            int y = centerY + radius * sin(angle);

            /* Draw active or passive dot */
            if (i == frame % 12) {
                tft.fillCircle(x, y, dot_radius + 1, TFT_BLACK);  // active
            } else {
                tft.fillCircle(x, y, dot_radius, TFT_LIGHTGREY);  // passive
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void display_main_screen() {
    show_spinner_animation(); 

    tft.fillScreen(TFT_WHITE);     
    tft.pushImage(130, 70, 220, 220, bus_background); 
    tft.pushImage(0, 0, 50, 50, logo_ute1);  
    tft.pushImage(419, 0, 61, 50, logo_ddt);

    /* Clear area and show final message */
    tft.setCursor(160, 45);
    tft.setTextColor(TFT_BLUE);
    tft.setTextSize(2);
    tft.printf("SYSTEM READY!");

}

void display_gps() {
    gps_datetime_t date_time = gps_get_datetime();

    /* gps img state */
    if (gps_get_status() == GPS_OK) {
        tft.pushImage(51, 0, 30, 30, gps_ok);  
    }
    else {
        tft.pushImage(51, 0, 30, 30, gps_lost);  
    }
    
    /* firebase img state */
    if (cloud_get_state() == CLOUD_OK) {
        tft.pushImage(389, 0, 30, 30, cloud_ok);
    }
    else {
        tft.pushImage(389, 0, 30, 30, cloud_lost);
    }

    /* battery level */
    uint8_t level = battery_percent_level(); 

    if (level >= 80) {
        tft.pushImage(220, 0, 40, 24, battery_100);
    }
    else if (level >= 60) {
        tft.pushImage(220, 0, 40, 24, battery_80);
    }
    else if (level >= 40) {
        tft.pushImage(220, 0, 40, 24, battery_60);
    }
    else if (level >= 20) {
        tft.pushImage(220, 0, 40, 24, battery_40);
    }
    else {
        tft.pushImage(220, 0, 40, 24, battery_20);
    }

    /* show date */
    tft.setCursor(5, 300);
    tft.setTextColor(TFT_BLUE, TFT_WHITE);
    tft.setTextSize(2);
    tft.printf("%s", date_time.date);

    /* show time */
    tft.setCursor(380, 300);
    tft.setTextColor(TFT_BLUE, TFT_WHITE);
    tft.setTextSize(2);
    tft.printf("%s", date_time.time);
}

void display_driver_checkin() {
    bus_driver_t* bus_driver = bus_driver_info_entry();

    tft.fillScreen(TFT_WHITE);

    /* Draw image centered */
    tft.pushImage(129, 70, 221, 220, bus_driver_1);

    /* Text over image */
    tft.setCursor(130, 35);
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.setTextSize(2);
    if (bus_driver->state == ON_BUS) {
        tft.printf("HELLO! %s", bus_driver->name);
    } else {
        tft.printf("GOODBYE! %s", bus_driver->name);
    }
}

void display_student_checkin(lt_msg_t* msg) {
    student_display_t student = *(student_display_t*)get_data_common_msg(msg);

    tft.fillScreen(TFT_WHITE);

    if (student.image.data != NULL) {
        /* Center image horizontally */
        int x = (tft.width() - student.image.width) / 2;
        int y = (tft.height() - student.image.height + 40) / 2;
        tft.pushImage(x, y, student.image.width, student.image.height, student.image.data);
    }

    tft.setCursor(140, 30);
    tft.setTextColor(TFT_BLUE, TFT_WHITE);
    tft.setTextSize(2);
    if ((student.state == CHECKIN_SCHOOL) || (student.state == CHECKIN_HOME)) {
    tft.printf("HELLO! %s",student.name);
    }
    else {
    tft.printf("GOODBYE! %s",student.name);
    }

    tft.setCursor(150, 50);
    tft.setTextColor(TFT_BLUE, TFT_WHITE);
    tft.setTextSize(2);
    tft.printf("STUDENT ID: %03d",student.id);

}

void display_invalid_tag() {
    tft.fillScreen(TFT_WHITE);

    /* Draw image centered */
    tft.pushImage(130, 80, 220, 220, invalid_tag);

    /* Text over image */
    tft.setCursor(190, 30);
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.setTextSize(2);
    tft.printf("WARNING!");

    tft.setCursor(125, 55);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextSize(2);
    tft.printf("PRESENT CARD INVALID!");

    tft.setCursor(100, 75);
    tft.printf(" SERVICE NOT REGISTERED!");

}

void display_sos_detect() {
    tft.fillScreen(TFT_WHITE);

    /* Centered image */
    tft.pushImage(130, 57, 220, 206, sos);

    /* Text over image */
    tft.setCursor(150, 35);
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.printf("SOS DETECTED!");
}

void display_calling_driver() {
    bus_driver_t* bus_driver = bus_driver_info_entry();

    tft.fillScreen(TFT_WHITE);
    tft.pushImage(129, 70, 221, 220, bus_driver_1);

    /* Text over image */
    tft.setCursor(80, 35);
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.printf("Calling Driver %s...", bus_driver->name);
}

void display_calling_parent() {

    tft.fillScreen(TFT_WHITE);
    tft.pushImage(115, 70, 250, 195, call_parent);

    /* Text over image */
    tft.setCursor(110, 35);
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.printf("Calling Your Parent...");
}

void display_call_ended () {
    tft.fillScreen(TFT_WHITE);
    tft.pushImage(115, 45, 250, 250, call_ending);
}

void display_require_reset() {
    tft.fillScreen(TFT_WHITE);

    /* Center image */
    tft.pushImage(129, 70, 220, 220, req_scan);

    /* Text over image */
    tft.setCursor(180, 35);
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.setTextSize(3);
    tft.printf("WARNING!");

}

void display_student_remain(lt_msg_t* msg) {
    uint8_t student_on_bus = *(uint8_t*)get_data_common_msg(msg);

    tft.fillScreen(TFT_WHITE);

    /* Center image */
    tft.pushImage(165, 120, 150, 150, warning);

    /* Text over image */
    tft.setCursor(180, 35);
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.setTextSize(3);
    tft.printf("WARNING!");

    tft.setCursor(30, 65);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextSize(3);
    tft.printf("STILL  STUDENTS ON BUS!",student_on_bus);

    tft.setCursor(128, 65);
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.setTextSize(3);
    tft.printf("%d",student_on_bus);

}
