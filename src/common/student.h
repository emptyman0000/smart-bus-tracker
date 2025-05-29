/**
******************************************************************************
* @author: InfinityVoid
* @date:   05/05/2025
* @brief: Driver information initialization
* @file: student.h
******************************************************************************
**/

#ifndef __STUDENT_H__
#define __STUDENT_H__

#include <stdio.h>
#include <stdint.h>

#include "student_1.h"
#include "student_2.h"
#include "student_3.h"
#include "student_4.h"
#include "student_5.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CHECKIN_SCHOOL,
    CHECKOUT_SCHOOL,
    CHECKIN_HOME,
    CHECKOUT_HOME,
    /* special state*/
    UNKNOWN,
    ABSENT,
} student_state_t;

typedef struct {
    const unsigned short* data; /* Pointer to image data */
    uint16_t width;             /* Image width */
    uint16_t height;            /* Image height */
} image_t;

typedef struct {
    uint8_t id;
    const char* name;
    student_state_t state;
} student_info_t;

typedef struct {
    uint8_t id;
    const char* name;
    student_state_t state;
    image_t image;
} student_display_t;

typedef struct {
    uint8_t id;
    const char* name;
    const char* parent_phone;
    uint8_t uid[4]; 
    student_state_t state;
    image_t image;              /* Image data */
} student_t;

extern uint8_t STUDENT_TOTAL;

/* public utility functions */
const char* student_state_string(student_state_t state);

extern void student_list_init();
extern void student_reset_all_state(); 
extern student_t* student_list_entry();
extern void student_log_data(student_t* student);

#ifdef __cplusplus
}
#endif

#endif /* __STUDENT_H__ */
