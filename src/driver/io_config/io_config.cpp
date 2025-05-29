/**
 ******************************************************************************
 * @author: InfinityVoid
 * @date:   15/02/2025
 * @file: io_config.cpp
 ******************************************************************************
**/

#include "io_config.h"

/* peripherals setup */
void peripherals_init() {
    
    Serial.begin(SERIAL_BAUDRATE);
    
    analogReadResolution(12);

    /* init io mode */
    pinMode(BUTTON_IO_PIN, INPUT);
    pinMode(RELAY_IO_PIN, OUTPUT);
    pinMode(BUZZER_IO_PIN, OUTPUT);
    pinMode(LED_LIFE_IO_PIN, OUTPUT);

    /* init state output pin */
    digitalWrite(BUZZER_IO_PIN, LOW);
    digitalWrite(RELAY_IO_PIN, LOW);
}

/**********************************************************
  @note: Mapping Peripheral Objects
**********************************************************/

/* battery level function*/
uint8_t battery_percent_level() {

    uint16_t analog_value = analogRead(BATTERY_IO_PIN);

    float volt_in = analog_value * (3300.0 / 4095.0);

    float battery_volt = volt_in * 2;

    float percent_level = (battery_volt - BATTERY_MIN_VOLTAGE) / (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE) * 100;

    if (percent_level > 100.0) percent_level = 100.0;
    if (percent_level < 0.0)   percent_level = 0.0;

    return uint8_t(percent_level);
}

/* button control function */
uint32_t button_read_func() {
    if (digitalRead(BUTTON_IO_PIN)) {
        return (uint32_t)1;
    }
    else {
        return (uint32_t)0;
    }
}

/* led control function */
void led_on_func() {
    digitalWrite(LED_LIFE_IO_PIN, HIGH);
}

void led_off_func() {
    digitalWrite(LED_LIFE_IO_PIN, LOW);
}

/* buzzer control function */
void buzzer_on_func() {
    digitalWrite(BUZZER_IO_PIN, HIGH);
}

void buzzer_off_func() {
    digitalWrite(BUZZER_IO_PIN, LOW);
}

void buzzer_toggle_state(uint8_t number_toggle, uint32_t duration_toggle) {
    /* Toggle the buzzer on and off for a specified number of times */
    for (uint8_t i = 0; i < number_toggle; i++) {
        buzzer_on_func();                             /* Turn buzzer ON */
        vTaskDelay(pdMS_TO_TICKS(duration_toggle));  /* Wait half of the total toggle duration */

        buzzer_off_func();                            /* Turn buzzer OFF */
        vTaskDelay(pdMS_TO_TICKS(duration_toggle));  /* Wait the remaining half */
    }
}


void buzzer_alarm(uint16_t duration) {
    buzzer_on_func();
    vTaskDelay(duration);
    buzzer_off_func();
}

/* relay control function */
void relay_on_func() {
    digitalWrite(RELAY_IO_PIN, HIGH);
}

void relay_off_func() {
    digitalWrite(RELAY_IO_PIN, LOW);
}
