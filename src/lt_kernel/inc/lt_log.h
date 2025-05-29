/**
 ******************************************************************************
 * @author: GaoDen
 * @date:   17/02/2025
 * @brief: event-driven service built on top FreeRTOS
 ******************************************************************************
**/

#ifndef __LT_LOG_H__
#define __LT_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h> 
#include <stdint.h>

#include "lt_config.h"

#define LOG_ESCAPE                          "\033"
#define LOG_RED_COLOR                       LOG_ESCAPE "[0;31m"
#define LOG_YELLOW_COLOR                    LOG_ESCAPE "[0;33m"
#define LOG_GREEN_COLOR                     LOG_ESCAPE "[0;32m"
#define LOG_COLOR_RESET                     LOG_ESCAPE "[0m"

#define LOG(fmt, ...)                       printf(LOG_COLOR_RESET fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...)                      printf(LOG_RED_COLOR fmt LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOGW(fmt, ...)                      printf(LOG_YELLOW_COLOR fmt LOG_COLOR_RESET, ##__VA_ARGS__)
#define LOGI(fmt, ...)                      printf(LOG_GREEN_COLOR fmt LOG_COLOR_RESET, ##__VA_ARGS__)

#if (LT_LOG_EN == 1)
    #if (LT_LOG_INF == 1)
        #define LT_LOGI(fmt, ...)           LOG(fmt, ##__VA_ARGS__)
    #else
        #define LT_LOGI(fmt, ...)
    #endif

    #if (LT_LOG_KERNEL == 1)
        #define LT_LOGK(fmt, ...)           LOGI(fmt, ##__VA_ARGS__)
    #else    
        #define LT_LOGK(fmt, ...)
    #endif
    
    #if (LT_LOG_DBG == 1)
        #define LT_LOGD(fmt, ...)           LOGW(fmt, ##__VA_ARGS__)
    #else
        #define LT_LOGD(fmt, ...)
    #endif
#endif

#define SYS_ASSERT(cond)                                     \
    do                                                       \
    {                                                        \
        if (!(cond))                                         \
        {                                                    \
            LOGE("fatal error at file: %s line: %d\n", __FILE__, __LINE__); \
            esp_restart();			                         \
        }                                                    \
    } while (0); 

#ifdef __cplusplus
}
#endif

#endif /* __LT_LOG_H__ */
