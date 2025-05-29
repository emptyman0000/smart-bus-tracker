/**
******************************************************************************
* @author: InfinityVoid
* @date:   05/05/2025
* @brief: Driver information initialization
* @file: driver.c
******************************************************************************
**/

#ifndef __BUS_DRIVER_H__
#define __BUS_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>

#include "bus_driver_1.h"

#ifdef __cplusplus
extern "C" {
#endif  

typedef enum {
    NOT_ON_BUS,
    ON_BUS,
    /* */
    CONFIRM_RESET_ALL,
} bus_driver_state_t;

typedef struct {
    const char* name;
    const char* phone;
    uint8_t uid[4]; 
    bus_driver_state_t state; 
} bus_driver_t;

extern void bus_driver_list_init();
extern void bus_driver_reset_state();
extern bus_driver_t* bus_driver_info_entry();


#ifdef __cplusplus
}
#endif

#endif /* __BUS_DRIVER_H__ */
