/**
******************************************************************************
* @author: InfinityVoid
* @date:   07/05/2025
* @file: task_rfid.cpp
******************************************************************************
**/

#include "task_rfid.h"

void rfid_init();
void rfid_read_tag();

/* RFID module on VSPI */
MFRC522 mfrc522(VSPI_SS_PIN, VSPI_RST_PIN);

void task_rfid_handler(void* argv) {
    /* Wait all task active */
    waiting_active_object_ready();

    /* Queue message */
    uint8_t task_id = *(uint8_t*)argv;
    lt_msg_t msg;

    while (task_rev_msg(task_id, &msg) == LT_THREAD_OK) {
        switch (msg.signal) {
            case RFID_INIT: {
                rfid_init();
                
            }
                break;

            case RFID_REINIT: {
                mfrc522.PCD_Init();           // reinit MFRC522   
            }
                break;

            case RFID_READ_TAG:
                rfid_read_tag();
                break;

            default:
                break;
        }

        /* Prevent watchdog trigger */
        vTaskDelay(pdMS_TO_TICKS(5)); 

        /* Free message data */
        task_free_msg(msg.data);
    }
}

void rfid_init() {
    /* Setup RFID */
    SPI.begin(VSPI_SCK_PIN, VSPI_MISO_PIN, VSPI_MOSI_PIN, VSPI_SS_PIN);
    mfrc522.PCD_Init();           // Init MFRC522
    vTaskDelay(pdMS_TO_TICKS(100));
    mfrc522.PCD_DumpVersionToSerial();

    /* Start periodic scan */
    timer_set(TASK_RFID_ID, RFID_READ_TAG, 300, TIMER_PERIODIC);
    timer_set(TASK_RFID_ID, RFID_REINIT, TIME_RESET_INIT, TIMER_PERIODIC);
}

void rfid_read_tag() {

    /* Check tag */
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

        printf("[task_rfid] TAG available! UID is: ");
        uint8_t UID[4]; 
        buzzer_alarm(100);

        /* Extract UID */
        for (byte i = 0; i < mfrc522.uid.size && i < 4; i++) {
            UID[i] = mfrc522.uid.uidByte[i];
            printf("0x%02X ", UID[i]);
        }
        printf("\n");

        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();

        /* logic task */
        task_post_common_msg(TASK_LOGIC_ID, LOGIC_VERIFY_UID_TAG, UID, sizeof(UID));
    }
}
