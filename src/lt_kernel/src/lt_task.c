/**
 ******************************************************************************
 * @author: GaoDen
 * @date:   12/02/2025
 * @brief: event-driven service built on top FreeRTOS
 ******************************************************************************
**/

#include "lt_task.h"

#include "lt_config.h"
#include "lt_log.h"
#include "lt_message.h"

static lt_sys_thread_t* task_list = (lt_sys_thread_t*)0;
static SemaphoreHandle_t sem_task_started = NULL;
static uint8_t task_table_size = 0;

void lt_kernel_banner() {
    LOG("\n\n\n");
	LOG(" __    ____    __  __\n");
	LOG("(  )  (_  _)  (__)/ / \n");
	LOG("/  \\__  )(    |  _( )\n");
	LOG("\\____/ (__)   (__) \\__\\\n");
	LOG("\n");
	LOG("Kernel version: %s\n", LITE_THREAD_KERNEL_VERSION);
	LOG("Source: %s\n", "GaoDen");
	LOG("Author: %s\n", "InfinityVoid");
	LOG("\n");
}

void task_init(lt_sys_thread_t* task_table) {
    /* lite-thread kernel banner */
    lt_kernel_banner();

	/* assign task table */
	task_list = task_table;
	while (task_list[task_table_size].pthread != PTHREAD_NULL) {
        task_table_size++;
    }

    /* semaphore init */
	sem_task_started = xSemaphoreCreateCounting(task_table_size, task_table_size);
	SYS_ASSERT(sem_task_started != NULL);

    /* task init */
	LOG("task init information:\n");
	for (uint8_t i = 0; i < task_table_size; i++){
		/* message queue init */
		if (task_list[i].queue_size > 0) {
			task_list[i].task_queue = xQueueCreate(task_list[i].queue_size, sizeof(lt_msg_t));
			if (task_list[i].task_queue == NULL) {
				LOG("%s queue create failed\n", task_list[i].task_info);
			}
		}

		/* task list init */
		BaseType_t status = xTaskCreate(task_list[i].pthread, task_list[i].task_info, task_list[i].stack_size, &task_list[i].task_id, task_list[i].task_priority, NULL);
		if (status == pdFAIL) {
			LOG("%s create failed\n", task_list[i].task_info);
		}

		LOG("task_name: %-16s task_id: %-4d stack_size: %-6ld task_priority: %-2d\n",
			task_list[i].task_info,
			task_list[i].task_id,
			task_list[i].stack_size,
			task_list[i].task_priority);
	}
	LOG("\n\n");
}

void waiting_active_object_ready() {
	LT_LOGK("[kernel_info] wait all task started\n");

	SYS_ASSERT(pdFAIL != xSemaphoreTake(sem_task_started, portMAX_DELAY));

	while (uxSemaphoreGetCount(sem_task_started) > 0) {
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

uint8_t task_post_common_msg(uint8_t des_task_id, uint8_t signal, uint8_t* data, uint32_t size) {
	LT_LOGK("[kernel_info] thread posting message: \n");
	LT_LOGK("[kernel_info] des_task_id: %d signal: %d data_len: %ld\n", des_task_id, signal, size);

	/* posting message */
	lt_msg_t msg;
	msg.type = COMMON_MSG_TYPE;
	msg.signal = signal;
	msg.data = NULL;
	msg.len = size;

	SYS_ASSERT(des_task_id < task_table_size);

	if (size) {
		msg.data = pvPortMalloc(size);
		SYS_ASSERT(msg.data != NULL);
		memcpy(msg.data, data, size);
	}

	SYS_ASSERT(xQueueSend(task_list[des_task_id].task_queue, &msg, portMAX_DELAY) != pdFAIL);

	return LT_THREAD_OK;
}

uint8_t task_post_pure_msg(uint8_t des_task_id, uint8_t signal) {
	LT_LOGK("[kernel_info] thread posting message: \n");
	LT_LOGK("[kernel_info] des_task_id: %d signal: %d\n", des_task_id, signal);

	/* posting message */
	lt_msg_t msg;
	msg.type = PURE_MSG_TYPE;
	msg.signal = signal;
	msg.data = NULL;
	msg.len = 0;

	SYS_ASSERT(des_task_id < task_table_size);
	SYS_ASSERT(xQueueSend(task_list[des_task_id].task_queue, &msg, portMAX_DELAY) != pdFAIL);

	return LT_THREAD_OK;
}

uint8_t task_post_common_msg_from_isr(uint8_t des_task_id, uint8_t signal, uint8_t* data, uint32_t size) {
	LT_LOGK("[kernel_info] thread posting message from isr: \n");
	LT_LOGK("[kernel_info] des_task_id: %d signal: %d data_len: %ld\n", des_task_id, signal, size);

	/* posting message */
	lt_msg_t msg;
	msg.type = COMMON_MSG_TYPE;
	msg.signal = signal;
	msg.data = NULL;
	msg.len = size;

	SYS_ASSERT(des_task_id < task_table_size);

	if (size) {
		msg.data = pvPortMalloc(size);
		SYS_ASSERT(msg.data != NULL);
		memcpy(msg.data, data, size);
	}

	BaseType_t higher_priority_task_woken = pdFALSE;
	SYS_ASSERT(xQueueSendFromISR(task_list[des_task_id].task_queue, &msg, &higher_priority_task_woken) != pdFAIL);
	portYIELD_FROM_ISR(higher_priority_task_woken);

	return LT_THREAD_OK;
}

uint8_t task_post_pure_msg_from_isr(uint8_t des_task_id, uint8_t signal) {
	SYS_ASSERT(task_post_common_msg_from_isr(des_task_id, signal, NULL, 0) == LT_THREAD_OK);
	return LT_THREAD_OK;
}

uint8_t task_rev_msg(uint8_t task_id, lt_msg_t* msg) {
	
	SYS_ASSERT(xQueueReceive(task_list[task_id].task_queue, msg, portMAX_DELAY) != pdFAIL);

	LT_LOGK("[kernel_info] thread received message: \n");
	LT_LOGK("[kernel_info] task_id: %d signal: %d data_len: %ld\n", task_id, msg->signal, msg->len);

	return LT_THREAD_OK;
}

void task_free_msg(void *data) {
	if (data != NULL) {
		vPortFree(data);
	}
}

uint8_t* get_data_common_msg(lt_msg_t* msg) {
	SYS_ASSERT(msg->type == COMMON_MSG_TYPE);
	return msg->data;
}
