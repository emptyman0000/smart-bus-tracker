/**
******************************************************************************
* @author: InfinityVoid
* @date:   06/05/2025
* @file: task_net.h
******************************************************************************
**/

#ifndef __TASK_NET_H__
#define __TASK_NET_H__

#include "WiFi.h"
#include "FirebaseESP32.h"

#include "app_signal.h"
#include "io_config.h"
#include "task_list.h"

#include "lt_task.h"
#include "lt_message.h"
#include "lt_timer.h"
#include "lt_log.h"

#define INIT_TIMEOUT            (5000)

typedef enum {
    CLOUD_LOST,
    CLOUD_OK,
} cloud_status_t;

typedef struct {
    const char* ssid;
    const char* password;
} wifi_list_t;

#define WIFI_LIST_SIZE          (sizeof(wifi_list)/sizeof(wifi_list[0]))

extern void task_net_handler(void* argv);
extern cloud_status_t cloud_get_state();

void wifi_init();
void firebase_init();
void check_firebase_result(lt_msg_t* msg);

#endif /* __TASK_NET_H__ */
