#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "main.h"

void create_tasks(void);

void task_measure_distance(void* args);
void task_measure_temperature(void* args);
void task_alarm(void* args);
void task_blink_led(void* args);
void task_print_message(void* args);

#endif /* TASK_MANAGER_H */
