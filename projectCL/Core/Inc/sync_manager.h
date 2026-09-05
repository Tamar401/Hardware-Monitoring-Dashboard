#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

// תורים
extern QueueHandle_t queueMeasuredDistances;
extern QueueHandle_t queueMessages;
extern QueueHandle_t queueLedCommands;

// מנעולים
extern SemaphoreHandle_t tempMutex;
extern SemaphoreHandle_t msgSemaphore;

extern float global_temperature;

void init_sync_objects(void);

#endif /* SYNC_MANAGER_H */
