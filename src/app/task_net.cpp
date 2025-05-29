/**
******************************************************************************
* @author: InfinityVoid
* @date:   06/05/2025
* @file: task_net.cpp
******************************************************************************
**/

#include "task_net.h"
#include "task_gps.h"

void firebase_post_data_gps(lt_msg_t* msg);
void firebase_post_data_driver(lt_msg_t* msg);
void firebase_post_data_student(lt_msg_t* msg);
void check_firebase_result(lt_msg_t* msg);

FirebaseConfig fbConfig;
FirebaseAuth fbAuth;
FirebaseData fbData;

static cloud_status_t cloud_state = CLOUD_LOST;

static char time_scan[20];
static char date[20]; 



wifi_list_t wifi_list[] = {
    { "HoaGiay",            "17a17a17a"},
    { "SONG CA PHE",        "123456songcaphe" },
    { "Bamos Coffee",       "bamosxinchao"},
};

void task_net_handler(void* argv) {
    /* wait all task active */
    waiting_active_object_ready();

    /* queue message */
    uint8_t task_id = *(uint8_t*)argv;
    lt_msg_t msg;

    while (task_rev_msg(task_id, &msg) == LT_THREAD_OK) {
        switch (msg.signal) {
            case NET_WIFI_INIT: {
                printf("[task network] Wifi init\n");
                wifi_init();
            }
                break;

            case NET_FIREBASE_INIT: {
                printf("[task network] Firebase init!\n");
                firebase_init();
            }
                break;

            case NET_FIREBASE_POST_DATA_GPS: {
                firebase_post_data_gps(&msg);
            }
                break;

            case NET_FIREBASE_POST_DATA_DRIVER: {
                firebase_post_data_driver(&msg);
            }
                break;

            case NET_FIREBASE_POST_DATA_STUDENT: {
                 firebase_post_data_student(&msg);
            }
                    break;

            default:{

            }
                break;
        
        /* free msg data */
        task_free_msg(msg.data);
        }
    }
}

void wifi_init() {
    /* Start scanning for available WiFi networks */
    printf("[task network] Scanning WiFi networks...\n");

    /* WiFi scan */
    int n = WiFi.scanNetworks();  
    if (n == 0) {
        printf("[task network] No WiFi networks found.\n");
    } else {
        printf("[task network] %d networks found.\n", n);

        /* Iterate through all detected networks */
        for (int i = 0; i < n; i++) {
            String found_ssid = WiFi.SSID(i);
            printf("[task network] Found SSID: %s\n", found_ssid.c_str());

            /* Compare found SSID with predefined list */
            for (int j = 0; j < WIFI_LIST_SIZE; j++) {
                if (found_ssid == wifi_list[j].ssid) {
                    printf("[task network] Matching SSID found: %s\n", wifi_list[j].ssid);

                    /* Attempt to connect to the matched WiFi */
                    WiFi.begin(wifi_list[j].ssid, wifi_list[j].password);
                    WiFi.setAutoReconnect(true);

                    uint32_t start_time = millis();

                    /* Wait for connection or timeout */
                    while (WiFi.status() != WL_CONNECTED) {
                        printf("[task network] Connecting to %s...\n", wifi_list[j].ssid);
                        vTaskDelay(pdMS_TO_TICKS(1000));

                        if (millis() - start_time > INIT_TIMEOUT) {
                            printf("[task network] Timeout connecting to %s\n", wifi_list[j].ssid);
                            WiFi.disconnect(true);
                            break;
                        }
                    }

                    /* Connection successful */
                    if (WiFi.status() == WL_CONNECTED) {
                        printf("[task network] Connected to %s\n", WiFi.SSID().c_str());

                        /* Send signal to initialize Firebase */
                        task_post_pure_msg(TASK_NETWORK_ID, NET_FIREBASE_INIT);
                        return;
                    }
                }
            }
        }
    }

    /* restart the device */
    printf("[task network] No matching SSID found or all connections failed. Restarting...\n");
    ESP.restart();
}

void firebase_init() {
    /* config firebase */
    fbConfig.api_key = API_KEY;
    fbConfig.database_url = DATABASE_URL;

    fbAuth.user.email = FB_AUTH_EMAIL;
    fbAuth.user.password = FB_AUTH_PASS;

    Firebase.begin(&fbConfig, &fbAuth);
    Firebase.reconnectWiFi(true);

    uint32_t start_time = millis();

    while (!Firebase.ready()) {
        printf("[task network] Waiting for firebase ready...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        if (millis() - start_time > INIT_TIMEOUT) {  
            printf("[task network] firebase connect timeout!\n");
            break;
        }
    }

}

void firebase_post_data_gps(lt_msg_t* msg) {

    if (Firebase.ready()) {

        /* decode gps msg data */
        gps_data_t* gps_data_rev = (gps_data_t*)get_data_common_msg(msg);

        snprintf(time_scan, sizeof(time_scan), "'%s'", gps_data_rev->time);
        snprintf(date, sizeof(date), "'%s'", gps_data_rev->date);

        strcpy(gps_data_rev->time, time_scan);
        strcpy(gps_data_rev->date, date);

        // printf("[task net] gps_data_time: %s | time_scan: %s | date: %s\n",gps_data_rev->time, time_scan, date);

        /* post & check success */
        Firebase.setFloat(fbData, "/GPS/LATITUDE_1", gps_data_rev->latitude);    
        Firebase.setFloat(fbData, "/GPS/LONGITUDE_1", gps_data_rev->longitude); 
        Firebase.setString(fbData, "/GPS/time_update", gps_data_rev->time);      
        Firebase.setString(fbData, "/GPS/date_update", gps_data_rev->date);

        /* led state */
        led_toggle(&led_life);
        cloud_state = CLOUD_OK;
    }
    else {
        cloud_state = CLOUD_LOST;
        printf("[task network] Firebase is busy!\n");
    }
}

cloud_status_t cloud_get_state() {
    return cloud_state;
}

void firebase_post_data_driver(lt_msg_t* msg) {
    if (!Firebase.ready()) {
        printf("[task network] Firebase is busy!\n");
        return;
    }

    /* Extract driver data */
    bus_driver_t* driver = (bus_driver_t*)get_data_common_msg(msg);

    printf("[task network] Driver: %s | Phone: %s | State: %s | Date: %s | Time: %s\n",
           driver->name, driver->phone,
           driver->state ? "ON_BUS" : "OFF_BUS",
           date, time_scan);

    /* Push base info */
    bool ok_name = Firebase.setString(fbData, "/VEHICLE_1/DRIVER_NAME", driver->name);
    bool ok_phone = Firebase.setString(fbData, "/VEHICLE_1/DRIVER_PHONE_NUMBER", driver->phone);

    /* Push time log */
    const char* time_path = driver->state
        ? "/VEHICLE_1/TIME_ON_BOARD"
        : "/VEHICLE_1/TIME_OFF_BOARD";

    char path_date[64], path_time[64];
    snprintf(path_date, sizeof(path_date), "%s/DATE", time_path);
    snprintf(path_time, sizeof(path_time), "%s/TIME", time_path);

    bool ok_date = Firebase.setString(fbData, path_date, date);
    bool ok_time = Firebase.setString(fbData, path_time, time_scan);

    if (ok_name && ok_phone && ok_date && ok_time) {
        printf("[task network] Post vehicle 1 data succeed!\n");
    } else {
        printf("[task network] Failed to post driver data: %s\n", fbData.errorReason());
    }
}


void firebase_post_data_student(lt_msg_t* msg) {
    if (!Firebase.ready()) {
        printf("[task network] Firebase is busy!\n");
        return;
    }

    student_info_t* student = (student_info_t*)get_data_common_msg(msg);
    if (student == NULL) {
        printf("[task network] Error: student is NULL!\n");
        return;
    }

    /* Build base path: e.g., /CHECKIN_HOME_1/STUDENT_3/ */
    char base_path[64];
    const char* student_state_str = student_state_string(student->state);
    snprintf(base_path, sizeof(base_path), "/%s_1/STUDENT_%d", student_state_str, student->id);

    // printf("[task_net] Posting student data to path: %s\n", base_path);

    /* Create full paths for each field */
    char path_name[48];
    char path_date[48];
    char path_time[48];

    snprintf(path_name, sizeof(path_name), "%s/NAME", base_path);
    snprintf(path_date, sizeof(path_date), "%s/DATE", base_path);
    snprintf(path_time, sizeof(path_time), "%s/TIME", base_path);

    /* Send each field separately */
    bool ok_name = Firebase.setString(fbData, path_name, student->name);
    bool ok_date = Firebase.setString(fbData, path_date, date);
    bool ok_time = Firebase.setString(fbData, path_time, time_scan);

    if (ok_name && ok_date && ok_time) {
        printf("[task_net] Student data posted: %s\n", student->name);
    } else {
        printf("[task_net] Failed to post student data: %s\n", fbData.errorReason());
    }
}
