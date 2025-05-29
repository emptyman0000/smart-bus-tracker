#include "task_logic.h"

bool sos_handler = false;
fsm_t fsm_task_logic;

void fsm_logic_normal_hander(lt_msg_t* msg);
void fsm_logic_sos_hander(lt_msg_t* msg);

static void logic_verify_uid_tag(lt_msg_t msg);
static uint8_t student_on_bus = 0;

void task_logic_handler(void* argv) {
    /* wait all task active */
    waiting_active_object_ready();

    /* queue message */
    uint8_t task_id = *(uint8_t*)argv;
    lt_msg_t msg;

    /* fsm init state */
    fsm_init(&fsm_task_logic, fsm_logic_normal_hander);

    while (task_rev_msg(task_id, &msg) == LT_THREAD_OK) {
        switch (msg.signal) {
            case LOGIC_VERIFY_UID_TAG: {
                logic_verify_uid_tag(msg);
            }
                break;

            case LOGIC_FSM_STUDENT_HANDLE: {
                printf("[task_logic] LOGIC_FSM_STUDENT_HANDLE\n");
                fsm_dispatch(&fsm_task_logic, &msg);
            }
                break;

            case LOGIC_SOS_TIMEOUT: {
                // printf("[task_logic][fsm_sos] LOGIC_SOS_TIMEOUT\n");
                // printf("[task_logic] SOS uid check timeout! return to normal uid check\n");
                fsm_trans(&fsm_task_logic, fsm_logic_normal_hander);

                task_post_pure_msg(TASK_DISPLAY_ID, DISPLAY_INIT);
            }
                break;

            default:
                break;
        }
    }
}

static bool uid_information_compare(const uint8_t* uid1, const uint8_t* uid2) {
    return memcmp(uid1, uid2, UID_TAG_LENGTH) == 0;
}

void logic_verify_uid_tag(lt_msg_t msg) {
    /* decode msg data */
    uint8_t* buffer = (uint8_t*)get_data_common_msg(&msg);
    uint8_t UID[4];

    memcpy(UID, buffer, sizeof(UID));

    student_t* student_list = student_list_entry();
    bus_driver_t* bus_driver = bus_driver_info_entry();

    printf("\n[task logic] Checking RFID tag...");

    /* prevent do all */
    bool tag_processed = false;

    /* Check if tag belongs to driver */
    if (uid_information_compare(UID, bus_driver->uid)) {
        switch (bus_driver->state)
        {
        case CONFIRM_RESET_ALL: {
            esp_restart();
        }
            break;

        case NOT_ON_BUS: {
            bus_driver->state = ON_BUS;
            printf("[task logic] Driver %s got on the bus!\n", bus_driver->name);

            task_post_pure_msg(TASK_DISPLAY_ID, DISPLAY_DRIVER_CHECKIN);
        }
            break;

        case ON_BUS: {
            if(student_on_bus > 0) {
                task_post_common_msg(TASK_DISPLAY_ID, DISPLAY_STUDENT_REMAIN, (uint8_t*)&student_on_bus, sizeof(student_on_bus));
                
                printf("[task logic] STILL STUDENT ON BUS!!! PLEASE RECHECK\n");
                buzzer_toggle_state(BUZZER_PERIODIC_TIMES, BUZZER_DURATION_TOGGLE);
            }
            else {
                bus_driver->state = NOT_ON_BUS;
                printf("[task logic] Driver %s got out the bus!\n", bus_driver->name);

                task_post_pure_msg(TASK_DISPLAY_ID, DISPLAY_DRIVER_CHECKIN);
            }
        }
            break;

        default: {

        }
            break;
    }
        /* for another tsk */
        task_post_common_msg(TASK_NETWORK_ID, NET_FIREBASE_POST_DATA_DRIVER, (uint8_t*)bus_driver, sizeof(bus_driver_t));

        /* update process */
        tag_processed = true;
    }

    /* Check if tag belongs to student */
    for (uint8_t index = 0; index < STUDENT_TOTAL && !tag_processed; index++) {
        student_t* stu = &student_list[index];

        if (uid_information_compare(UID, stu->uid)) {
            if (stu->state != ABSENT) {
                printf("[task logic] Student card detected!\n");
                
                /* fsm dispatch message */
                task_post_common_msg(TASK_LOGIC_ID, LOGIC_FSM_STUDENT_HANDLE, (uint8_t*)&index, sizeof(uint8_t));
            }
            else {
                printf("[task logic] Student card detected. But student absent!\n");
                student_log_data(stu);
            }

            /* update process */
            tag_processed = true;
        }

    }

    /* unknown UID */
    if (!tag_processed) {
        printf("[task logic] Invalid card! UID not recognized.\n");
        task_post_pure_msg(TASK_DISPLAY_ID, DISPLAY_WARNING);
    }

}

void fsm_logic_normal_hander(lt_msg_t* msg) {
    switch (msg->signal) {
    case LOGIC_FSM_STUDENT_HANDLE: {
        printf("[task logic][fsm_normal] normal uid checked. Send data to post\n");
        
        /* get student index and pointer */
        uint8_t index = *(uint8_t*)get_data_common_msg(msg);
        student_t* student = &student_list_entry()[index];

        
        /* update student state */
        switch (student->state) {
            case CHECKOUT_HOME: {
                student->state = CHECKIN_SCHOOL;
                student_on_bus++;
            }
                break;

            case CHECKIN_SCHOOL: {
                student->state = CHECKOUT_SCHOOL;
                student_on_bus--;
                task_post_common_msg(TASK_GSM_ID, GSM_SEND_SMS, (uint8_t*)student, sizeof(student_t));
            }
                break;

            case CHECKOUT_SCHOOL: {
                student->state = CHECKIN_HOME;
                student_on_bus++;
            }
                break;

            case CHECKIN_HOME: {
                student->state = CHECKOUT_HOME;
                student_on_bus--;
                task_post_common_msg(TASK_GSM_ID, GSM_SEND_SMS, (uint8_t*)student, sizeof(student_t));
            }  
                break;

            default: {

            }
                break;
        }

        /* log data student after updated */
        // student_log_data(student);
        printf("[task logic] Number of student on bus now: %hhu\n", student_on_bus);
        
        /* for tsk fb */
        student_info_t student_info = {
            .id = student->id,
            .name = student->name,
            .state = student->state
        };
        task_post_common_msg(TASK_NETWORK_ID, NET_FIREBASE_POST_DATA_STUDENT, (uint8_t*)&student_info, sizeof(student_info));

        /* for tsk dp */
        student_display_t display = {
            .id = student->id,
            .name = student->name,
            .state = student->state,
            .image = student->image
        };
        task_post_common_msg(TASK_DISPLAY_ID, DISPLAY_STUDENT_CHECKIN, (uint8_t*)&display, sizeof(display));
        }
        break;
    
    default: {
    }
        break;
    }
}

void fsm_logic_sos_hander(lt_msg_t* msg) {
    switch (msg->signal) {
    case LOGIC_FSM_STUDENT_HANDLE: {
        printf("[task logic][fsm_sos] SOS uid check succeed!\n");

        /* clear timeout */
        timer_remove(TASK_LOGIC_ID, LOGIC_SOS_TIMEOUT);

        /* handle sos request */
        uint8_t index = *(uint8_t*)get_data_common_msg(msg);
        student_t* student = &student_list_entry()[index];

        /* for tsk gsm to call sos */
        task_post_common_msg(TASK_GSM_ID, GSM_CALL_SOS, (uint8_t*)student, sizeof(student_t));

        /* switching to normal */
        fsm_trans(&fsm_task_logic, fsm_logic_normal_hander);
    }
        break;
    
    default: {
    }
        break;
    }
}
