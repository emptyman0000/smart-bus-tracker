/**
******************************************************************************
* @author: InfinityVoid
* @date:   05/05/2025
* @brief: Driver information initialization
* @file: student.c
******************************************************************************
**/

#include "student.h"


static student_t* p_student_list;

/* Init list of students */
student_t student_list[15] = {
    { 1, "Hoang_Khanh", "+84372385670", {0x63, 0xCC, 0xB2, 0x93}, CHECKOUT_HOME, {.data = student_1, .width = 220, .height = 220} },
    { 2, "Duy_Khanh"  , "+84838469498", {0xF3, 0x6B, 0x65, 0xFC}, CHECKOUT_HOME, {.data = student_2, .width = 220,  .height = 220} },
    { 3, "Van_Tin"    , "+84369538476", {0x42, 0x09, 0x7F, 0x4C}, CHECKOUT_HOME, {.data = student_3, .width = 220, .height = 220} },
    { 4, "Quoc_Trung" , "+84123421342", {0x2A, 0x80, 0x67, 0x1A}, CHECKOUT_HOME, {.data = student_4, .width = 220, .height = 220} },
    { 5, "Viet_Dung",   "+84345435434", {0xB3, 0x97, 0xC3, 0xED}, CHECKOUT_HOME, {.data = student_5, .width = 220, .height = 220} },
};

uint8_t STUDENT_TOTAL = sizeof(student_list) / sizeof(student_list[0]);

const char* student_state_string(student_state_t state) {

    const char* state_string = "";

    switch (state) {
        case CHECKIN_SCHOOL:   state_string = "CHECKIN_SCHOOL";    break;
        case CHECKOUT_SCHOOL:  state_string = "CHECKOUT_SCHOOL";   break;
        case CHECKIN_HOME:     state_string = "CHECKIN_HOME";      break;
        case CHECKOUT_HOME:    state_string = "CHECKOUT_HOME";     break;
        default:               state_string = "UNKNOWN_STATE";     break;
    }
    return state_string;
}

void student_list_init() {
    p_student_list = &student_list[0];
}

void student_reset_all_state() { 
    p_student_list = &student_list[0];

    for (uint8_t i = 0; i < STUDENT_TOTAL; i++) {
        student_list[i].state = CHECKOUT_HOME;
    }
}

student_t* student_list_entry() {
    return p_student_list;
}

void student_log_data(student_t* student) {
    printf("Logging student data:\n");
    printf("Name: %s | Parent_NumberP: %s | UID: [0x%02X 0x%02X 0x%02X 0x%02X] | State: %s\n",
        student->name,
        student->parent_phone,
        student->uid[0], student->uid[1],student->uid[2], student->uid[3],
        student_state_string(student->state));
}
