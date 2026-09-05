#include "sync_manager.h"

QueueHandle_t queueMeasuredDistances;
QueueHandle_t queueMessages;
QueueHandle_t queueLedCommands;
SemaphoreHandle_t tempMutex;
SemaphoreHandle_t msgSemaphore;

float global_temperature = 0.0;

void init_sync_objects(void) {
    queueMeasuredDistances = xQueueCreate(10, sizeof(float));
    queueMessages = xQueueCreate(10, sizeof(char) * 50);

    // מבנה שמכיל את מצב הלד (0=כבוי, 1=דולק, 2=מהבהב) וזמן הבהוב
    queueLedCommands = xQueueCreate(5, sizeof(uint32_t) * 2);

    tempMutex = xSemaphoreCreateMutex();
    msgSemaphore = xSemaphoreCreateCounting(10, 0);
}
