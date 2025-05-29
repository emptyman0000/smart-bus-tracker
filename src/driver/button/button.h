/**
 ******************************************************************************
 * @author: InfinityVoid
 * @date:   15/02/2025
 * @file: button.h
 ******************************************************************************
**/

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>


#define BUTTON_DEBOUNCE_TIME	    (20)
#define BUTTON_LONG_PRESS_TIME	    (1500)

#define BUTTON_ENABLE			    (0x01)
#define BUTTON_DISABLE			    (0x00)

#define BUTTON_HARDWARE_PRESS	    (0x00)
#define BUTTON_STATE_RELEASE	    (0x00)
#define BUTTON_STATE_PRESS		    (0x01)
#define BUTTON_STATE_LONG_PRESS	    (0x02)

typedef uint32_t (*pf_button_read_state)();
typedef void (*pf_button_callback)(void*);

typedef struct {
	/* attribute of button */
	uint8_t button_enable;
	uint8_t button_state;

	/* pointer function button */
	pf_button_read_state read_state;
	pf_button_callback callback;

	/* for polling */
	uint32_t poll_count;
} button_t;

extern void button_init(button_t* button, pf_button_read_state read_state, pf_button_callback callback);
extern void button_polling(button_t* button, uint8_t unit_inc);

#ifdef __cplusplus
}
#endif

#endif /* _BUTTON_H_ */
