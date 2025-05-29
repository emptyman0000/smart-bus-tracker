/**
 ******************************************************************************
 * @author: InfinityVoid
 * @date:   10/05/2025
 * @file:   task_gsm.cpp
 * @brief:  GSM task for SMS and emergency call handling
 ******************************************************************************
 **/

#include "task_gsm.h"

HardwareSerial gsmSerial(1);

/* Buffer used for receiving GSM responses */
static char buffer[128];
static call_action_t call_action = CALL_DRIVER_ATTEMPT_1;
static bool break_call = false;

/* Forward declarations */
void gsm_read_response(char* dest, size_t max_len, uint16_t timeout_ms);
bool gsm_wait_call_status_and_result();
void gsm_init();
void gsm_send_sms(lt_msg_t msg);
void gsm_call_sos(lt_msg_t msg);

/* Main GSM task loop */
void task_gsm_handler(void* argv) {
    /* Wait until all tasks are active */
    waiting_active_object_ready();

    /* Extract task ID from argument */
    uint8_t task_id = *(uint8_t*)argv;
    lt_msg_t msg;

    while (task_rev_msg(task_id, &msg) == LT_THREAD_OK) {
        switch (msg.signal) {
            case GSM_INIT: {
                gsm_init();
            }
                break;

            case GSM_SEND_SMS: {
                printf("[task gsm] GSM_SEND_SMS received!\n");
                gsm_send_sms(msg);
            }
                break;

            case GSM_CALL_SOS: {
                printf("[task gsm] GSM_CALL_SOS signal received!\n");
                break_call = false;
                relay_on_func();
                gsm_call_sos(msg);
                relay_off_func();
            }
                break;
            case GSM_BREAK_CALL: {
                break_call = true;
            }
                break;

            default: {

            }
            break;
        }

        /* Free memory allocated for the message */
        task_free_msg(msg.data);
    }
}

/* Initialize GSM module and check SIM and signal status */
void gsm_init() {
    /* Initialize GSM UART */
    vTaskDelay(pdMS_TO_TICKS(2000));
    gsmSerial.begin(GSM_UART_BAUDRATE, SERIAL_8N1, GSM_RX_PIN, GSM_TX_PIN);
    vTaskDelay(pdMS_TO_TICKS(100));

    /* Check SIM card status */
    gsmSerial.println("AT+CPIN?");
    vTaskDelay(pdMS_TO_TICKS(100));
    gsm_read_response(buffer, sizeof(buffer), 1000);
    printf("[task gsm] Response to CPIN: %s", buffer);

    /* Check signal quality */
    gsmSerial.println("AT+CSQ");
    vTaskDelay(pdMS_TO_TICKS(100));
    gsm_read_response(buffer, sizeof(buffer), 1000);
    printf("[task gsm] Response to CSQ: %s", buffer);
}

/* Send SMS message to student's parent */
void gsm_send_sms(lt_msg_t msg) {
    student_t* student = (student_t*)get_data_common_msg(&msg);
    char stu_stt_str[128];
    char at_cmd_sms[32];

    snprintf(at_cmd_sms, sizeof(at_cmd_sms), "AT+CMGS=\"%s\"", student->parent_phone);
    snprintf(stu_stt_str, sizeof(stu_stt_str), "%s %s", student->name, (char*)(student_state_string(student->state)));

    gsmSerial.println("AT+CMGF=1");               /* Set SMS text mode */
    vTaskDelay(pdMS_TO_TICKS(100));

    gsmSerial.println(at_cmd_sms);                /* Send AT+CMGS with phone number */
    vTaskDelay(pdMS_TO_TICKS(100));

    gsmSerial.println(stu_stt_str);               /* Send SMS content */
    vTaskDelay(pdMS_TO_TICKS(100));

    gsmSerial.write(26);                          /* Send Ctrl+Z to end message */
    vTaskDelay(pdMS_TO_TICKS(100));
}

/* Handle emergency call process */
void gsm_call_sos(lt_msg_t msg) {
    student_t* student = (student_t*)get_data_common_msg(&msg);
    bus_driver_t bus_driver = *bus_driver_info_entry();
    char at_cmd[32];

    switch (call_action) {
        case CALL_DRIVER_ATTEMPT_1: {
            task_post_pure_msg(TASK_DISPLAY_ID, DISPLAY_SOS_CALL_DRIVER);

            printf("[task gsm] Calling driver %s [attempt 1]:\n", bus_driver.name);
            snprintf(at_cmd, sizeof(at_cmd), "ATD%s;\r\n", bus_driver.phone);
            gsmSerial.print(at_cmd);

            bool call_success = gsm_wait_call_status_and_result();
            call_action = call_success ? CALL_DRIVER_ATTEMPT_1 : CALL_DRIVER_ATTEMPT_2;
            
            /* return main display */
            timer_set(TASK_DISPLAY_ID, DISPLAY_CALL_ENDED, 200, TIMER_ONE_SHOT);

        }
        break;

        case CALL_DRIVER_ATTEMPT_2: {
            task_post_pure_msg(TASK_DISPLAY_ID, DISPLAY_SOS_CALL_DRIVER);

            printf("[task gsm] Calling driver %s [attempt 2]:\n", bus_driver.name);
            snprintf(at_cmd, sizeof(at_cmd), "ATD%s;\r\n", bus_driver.phone);
            gsmSerial.print(at_cmd);

            bool call_success = gsm_wait_call_status_and_result();
            call_action = call_success ? CALL_DRIVER_ATTEMPT_1 : CALL_PARENT;

            /* return main display */
            timer_set(TASK_DISPLAY_ID, DISPLAY_CALL_ENDED, 200, TIMER_ONE_SHOT);
        }
        break;

        case CALL_PARENT: {
            task_post_pure_msg(TASK_DISPLAY_ID, DISPLAY_SOS_CALL_PARENT);

            printf("[task gsm] Calling to student %s's parent!\n", student->name);
            snprintf(at_cmd, sizeof(at_cmd), "ATD%s;\r\n", student->parent_phone);
            gsmSerial.print(at_cmd);
            bool call_success = gsm_wait_call_status_and_result();
            call_action = CALL_DRIVER_ATTEMPT_1;

            timer_set(TASK_DISPLAY_ID, DISPLAY_CALL_ENDED, 200, TIMER_ONE_SHOT);
        }
        break;

        default: {
            relay_off_func();
            call_action = CALL_DRIVER_ATTEMPT_1;
        }
        break;
    }
}

/* Read GSM response with timeout to prevent fragmentation issues */
void gsm_read_response(char* dest, size_t max_len, uint16_t timeout_ms) {
    size_t index = 0;
    uint32_t start_time = millis();
    char line[64];
    size_t line_idx = 0;

    dest[0] = '\0'; /* Clear destination buffer */

    while ((millis() - start_time) < timeout_ms && index < max_len - 1) {
        while (gsmSerial.available()) {
            char ch = gsmSerial.read();

            if (ch == '\n' || ch == '\r') {
                if (line_idx > 0) {
                    line[line_idx] = '\0';
                    /* Append this line to dest buffer */
                    strncat(dest, line, max_len - strlen(dest) - 1);
                    strncat(dest, "\n", max_len - strlen(dest) - 1);
                    index = strlen(dest);
                    line_idx = 0;
                }
            } else {
                if (line_idx < sizeof(line) - 1) {
                    line[line_idx++] = ch;
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


/* Wait for GSM call to complete, returns false if call failed or no answer */
bool gsm_wait_call_status_and_result() {
    char response[128] = {0};
    bool call_success = false;
    uint32_t start_time = millis();

    printf("[task gsm] --- Start monitoring call ---\n");

    while (millis() - start_time < WAIT_CALL_TIMEOUT ) {
        if (break_call) {
            break;
        }
        gsm_read_response(response, sizeof(response), 1000);

        if (strlen(response) > 0) {
            printf("[task gsm] GSM response: %s", response);

            if (strstr(response, "NO CARRIER") || strstr(response, "BUSY") || strstr(response, "ERROR")) {
                printf("[task gsm] Ending call.\n");
                break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }

    gsmSerial.println("ATH");  /* End call */
    printf("[task gsm] --- Call ended ---\n");

    return call_success;
}
