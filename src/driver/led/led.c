/**
 ******************************************************************************
 * @author: InfinityVoid
 * @date:   15/02/2025
 ******************************************************************************
**/

#include "led.h"
#include "io_config.h"

void led_init(led_t* led, pf_led_ctrl pf_led_on, pf_led_ctrl pf_led_off) {

    led->blink_enable = LED_BLINK_DISABLE;
    led->status = LED_OFF;
    led->blink_duration = 0;
    led->poll_counter = 0;

    /* assign the pointer function to led driver */
    led->pf_led_on = pf_led_on;
    led->pf_led_off = pf_led_off;

    /* default status */
    led->status = LED_OFF;
    led->pf_led_off();
}

void led_on(led_t* led) {
    led->status = LED_ON;
    led->pf_led_on();
}

void led_off(led_t* led) {
    led->status = LED_OFF;
    led->pf_led_off();
}

void led_blink_reset(led_t* led) {
    led->blink_enable = LED_BLINK_DISABLE;
    led->blink_duration = 0;
    led->poll_counter = 0;

    led->pf_led_off();
}

void led_blink_set(led_t* led, uint16_t duration) {
    led->blink_enable = LED_BLINK_ENABLE;
    led->blink_duration = duration;
    led->poll_counter = 0;
}

void led_set_static_on(led_t* led, uint8_t color) {
    led->blink_enable = LED_BLINK_DISABLE;
    led->status = LED_ON;
    led_on(led);
}

void led_toggle(led_t* led) {
    if (led->status == LED_ON) {
        led->status = LED_OFF;
        led->pf_led_off();
    }
    else {
        led->status = LED_ON;
        led->pf_led_on();
    }
}

void led_polling(led_t* led, uint16_t polling_time) {
    if (led->blink_enable == LED_BLINK_ENABLE) {
        if (led->poll_counter == led->blink_duration) {
            if (led->status == LED_OFF) {
                led->status = LED_ON;
                led_on(led);
                led->poll_counter = 0;
            }
            else if (led->status == LED_ON) {
                led->status = LED_OFF;
                led_off(led);
                led->poll_counter = 0;
            }
        }
        else {
            led->poll_counter += polling_time;
        }
    }
}
