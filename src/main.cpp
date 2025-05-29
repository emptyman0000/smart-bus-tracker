#include <Arduino.h>
#include <app_signal.h>

/* lite thread include */
#include "lt_task.h"
#include "lt_message.h"
#include "lt_timer.h"
#include "lt_log.h"
#include "lt_config.h"

/* common include */
#include "student.h"
#include "bus_driver.h"

/* driver include */
#include "io_config.h"
#include "button.h"
#include "led.h"

/* app include */
#include "app_bsc.h"
#include "task_list.h"

extern "C" {
    #include "esp_task_wdt.h"
}

/* app initial */
static void app_start_timer();
static void app_task_init();

void setup() {
    /******************************************************************************
    * hardware init
    *******************************************************************************/
    peripherals_init();

    /******************************************************************************
    * lite thread init
    *******************************************************************************/
    task_init((lt_sys_thread_t*)&app_task_table);
    esp_task_wdt_init(10, true);

    /******************************************************************************
    * common init 
    *******************************************************************************/
    student_list_init();
    bus_driver_list_init();

    /******************************************************************************
    * driver init 
    *******************************************************************************/
    
    led_init(&led_life, led_on_func, led_off_func);
    button_init(&sos_button, button_read_func, button_callback);

    /* app inital */
    app_task_init();

    /******************************************************************************
    * start timer for application
    *******************************************************************************/
    app_start_timer();
}

/*****************************************************************************/
/* app initial function 
 */
/*****************************************************************************/
void app_task_init() {
  /* app inital */
  task_post_pure_msg(TASK_NETWORK_ID, NET_WIFI_INIT);
  task_post_pure_msg(TASK_GSM_ID, GSM_INIT);
  task_post_pure_msg(TASK_RFID_ID, RFID_INIT);
}

void app_start_timer() {
  timer_set(TASK_LIFE_ID, SYS_LIFE_SYSTEM_CHECK, 1000, TIMER_PERIODIC);
  timer_set(TASK_GPS_ID, GPS_INIT, 3000, TIMER_ONE_SHOT);

}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
