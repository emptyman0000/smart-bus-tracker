/**
 ******************************************************************************
 * @author: InfinityVoid
 * @date:   15/02/2025
 * @file: io_config.h
 ******************************************************************************
**/

#ifndef _IO_CONFIG_H_
#define _IO_CONFIG_H_

#include "Arduino.h"
#include "esp32-hal-gpio.h"

/* parameters definition */
#define INCREASE_TICK               (1)
#define SERIAL_BAUDRATE             (115200)
#define GPS_NEO_M10_BAUDRATE        (38400)
#define GPS_NEO_M8_BAUDRATE         (9600)
#define GSM_UART_BAUDRATE           (9600)

/* firebase config definition */
#define API_KEY                     "YOUR_API_KEY"
#define DATABASE_URL                "YOUR_DATABASE_URL"
#define FB_AUTH_EMAIL               "YOUR_FIREBASE_AUTH_EMAIL"
#define FB_AUTH_PASS                "YOUR_FIREBASE_AUTH_PASS"

/* GPIO BUTTON definition */
#define BUTTON_IO_PIN 	            (39)

/* LED io definition */
#define LED_LIFE_IO_PIN             (32)
// #define LED_LIFE_IO_PIN             (-1) /* disabled if have not any led life */

/* BUZZER io definition */
#define BUZZER_IO_PIN 	            (25)
#define BUZZER_PERIODIC_TIMES       (10)
#define BUZZER_DURATION_TOGGLE      (200)       
// #define BUZZER_IO_PIN 	            (-1)

/* RELAY io definition */
#define RELAY_IO_PIN                (33)

/* BAT PIN io definition */
#define BATTERY_IO_PIN              (36)
#define BATTERY_MAX_VOLTAGE         (4200)
#define BATTERY_MIN_VOLTAGE         (3000)

/* MFRC522 io definition */
#define VSPI_MISO_PIN               (19)
#define VSPI_MOSI_PIN               (23)
#define VSPI_SCK_PIN                (18)
#define VSPI_SS_PIN                 (05)   
#define VSPI_RST_PIN                (21)    

/* GPS NEO M10 io definition */
#define GPS_RX_PIN                  (2)
#define GPS_TX_PIN                  (4)  

/* GSM SIM800A io definition */
#define GSM_RX_PIN                  (16)
#define GSM_TX_PIN                  (17)  

/***************************************
Extern void function
***************************************/
/* peripherals setup */
extern void peripherals_init();

/* battery level function*/
extern uint8_t battery_percent_level();

/* button control function */
extern uint32_t button_read_func();

/* led control function */
extern void led_on_func();
extern void led_off_func();

/* buzzer control function */
extern void buzzer_on_func();
extern void buzzer_off_func();
extern void buzzer_toggle_state(uint8_t number_toggle, uint32_t duration_toggle);
extern void buzzer_alarm(uint16_t duration);

/* relay control function */
extern void relay_on_func();
extern void relay_off_func();

#endif /* _IO_CONFIG_H_ */
