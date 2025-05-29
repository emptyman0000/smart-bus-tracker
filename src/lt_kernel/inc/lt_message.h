/**
 ******************************************************************************
 * @author: GaoDen
 * @date:   17/02/2025
 * @brief: event-driven service built on top FreeRTOS
 ******************************************************************************
**/

#ifndef __LT_MESSAGE_H__
#define __LT_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h> 
#include <stdint.h>
#include <string.h>

typedef enum {
    PURE_MSG_TYPE = 0x01,
    COMMON_MSG_TYPE = 0x02,
} lt_msg_type_t;

typedef struct {
    lt_msg_type_t type;
    uint8_t signal;
    uint8_t* data;
    uint32_t len;
} lt_msg_t;

#ifdef __cplusplus
}
#endif

#endif /* __LT_MESSAGE_H__ */
