/**
******************************************************************************
* @author: InfinityVoid
* @date:   05/05/2025
* @brief: Driver information initialization
* @file: driver.c
******************************************************************************
**/

#include "bus_driver.h"
static bus_driver_t* p_bus_driver_list;

/* driver info */
bus_driver_t bus_driver_info = {"Thanh_Nha", "+84xxxxxxxxx", {0x9F, 0x67, 0x27, 0x1F}, NOT_ON_BUS};

void bus_driver_list_init() {
    p_bus_driver_list = &bus_driver_info;
}

void bus_driver_reset_state() {
    bus_driver_info.state = NOT_ON_BUS;
}

bus_driver_t*  bus_driver_info_entry() {
    return p_bus_driver_list;    
}
