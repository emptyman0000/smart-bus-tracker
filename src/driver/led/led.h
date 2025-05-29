/**
 ******************************************************************************
 * @author: InfinityVoid
 * @date:   15/02/2025
 ******************************************************************************
**/

#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define LED_BLINK_ENABLE        (0x01)
#define LED_BLINK_DISABLE       (0x00)
#define LED_ON                  (0x01)
#define LED_OFF                 (0x00)

#define DURATION_LED_BLINK        (200)

typedef void (*pf_led_ctrl)(void);

typedef struct {
    /* led attributes */
    uint8_t blink_enable;
    uint8_t status;
    uint16_t blink_duration;

    /* led control functions */
    pf_led_ctrl pf_led_on;
    pf_led_ctrl pf_led_off;

    /* for polling */
    uint16_t poll_counter;
} led_t;

/* led functions */
extern void led_init(led_t* led, pf_led_ctrl pf_led_on, pf_led_ctrl pf_led_off); 
extern void led_toggle(led_t* led);
extern void led_set_static_on(led_t* led, uint8_t color);
extern void led_blink_set(led_t* led, uint16_t duration);
extern void led_blink_reset(led_t* led);
extern void led_polling(led_t* led, uint16_t polling_time);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H__ */
