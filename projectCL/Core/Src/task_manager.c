#include "task_manager.h"
#include "sync_manager.h"
#include "distance_sensor.h"
#include "temp_sensor.h"
#include "alarm.h"
#include "led.h"
#include <stdio.h>
#include <string.h>

// ייבוא ממשק התקשורת - ודאי ש-huart2 הוא ה-UART הנכון בלוח שלך
extern UART_HandleTypeDef huart2; 

TaskHandle_t h_task_dist;
TaskHandle_t h_task_temp;
TaskHandle_t h_task_alarm;
TaskHandle_t h_task_led;
TaskHandle_t h_task_print;
TaskHandle_t h_task_telemetry;

// חתימת הפונקציה החדשה
void task_telemetry(void* args);

void create_tasks(void) {
    xTaskCreate(task_measure_distance, "Dist", 128, NULL, 2, &h_task_dist);
    xTaskCreate(task_measure_temperature, "Temp", 128, NULL, 3, &h_task_temp);
    xTaskCreate(task_alarm, "Alarm", 128, NULL, 4, &h_task_alarm);
    xTaskCreate(task_blink_led, "Led", 128, NULL, 4, &h_task_led);
    xTaskCreate(task_print_message, "Print", 128, NULL, 1, &h_task_print);
    xTaskCreate(task_telemetry, "Telemetry", 256, NULL, 2, &h_task_telemetry);
}

void task_measure_distance(void* args) {
    float dist = 0;
    while(1) {
        dist = read_distance();
        xQueueSend(queueMeasuredDistances, &dist, portMAX_DELAY);
        vTaskDelay(500); // פעם בחצי שניה
    }
}

void task_measure_temperature(void* args) {
    float temp = 0;
    while(1) {
        temp = get_temperature();
        if(xSemaphoreTake(tempMutex, portMAX_DELAY) == pdTRUE) {
            global_temperature = temp;
            xSemaphoreGive(tempMutex);
        }
        vTaskDelay(1000); // פעם בשניה
    }
}

void task_alarm(void* args) {
    float dist = 0;
    float current_temp = 0;

    while(1) {
        if(xQueueReceive(queueMeasuredDistances, &dist, portMAX_DELAY) == pdTRUE) {
            if(dist > 0 && dist < 20.0) { // עצם קרוב
                if(xSemaphoreTake(tempMutex, portMAX_DELAY) == pdTRUE) {
                    current_temp = global_temperature;
                    xSemaphoreGive(tempMutex);
                }

                alarm_temperature(current_temp);

                char msg[50];
                snprintf(msg, sizeof(msg), "Dist: %.2f, Temp: %.2f\n", dist, current_temp);
                xQueueSend(queueMessages, &msg, 0);
                xSemaphoreGive(msgSemaphore);
            }
        }
    }
}

void task_blink_led(void* args) {
    LedCommand_t cmd;
    cmd.state = 0;

    while(1) {
        xQueueReceive(queueLedCommands, &cmd, 10);

        if (cmd.state == 1) {
            led_on();
            vTaskDelay(100);
        } else if (cmd.state == 2) {
            led_on();
            vTaskDelay(cmd.blink_delay);
            led_off();
            vTaskDelay(cmd.blink_delay);
        } else {
            led_off();
            vTaskDelay(100);
        }
    }
}

void task_print_message(void* args) {
    char msg[50];
    while(1) {
        if(xSemaphoreTake(msgSemaphore, portMAX_DELAY) == pdTRUE) {
            if(xQueueReceive(queueMessages, &msg, 0) == pdTRUE) {
                printf("%s", msg);
            }
        }
    }
}

void task_telemetry(void* args) {
    char txBuffer[50];
    float current_dist = 0;
    float current_temp = 0;

    while(1) {
        // שימוש ב-xQueuePeek כדי לא "לגנוב" את הנתון ממשימת האזעקה
        if (xQueuePeek(queueMeasuredDistances, &current_dist, 0) == pdTRUE) {
            
            if(xSemaphoreTake(tempMutex, portMAX_DELAY) == pdTRUE) {
                current_temp = global_temperature;
                xSemaphoreGive(tempMutex);
            }

            // אריזת הנתונים לפורמט הנדרש בשרת
            sprintf(txBuffer, "DIST:%d,ADC:%d\r\n", (int)current_dist, (int)current_temp);

            // שידור המידע
            HAL_UART_Transmit(&huart2, (uint8_t*)txBuffer, strlen(txBuffer), HAL_MAX_DELAY);
        }
        
        // המתנה של 100 מילישניות = קצב עדכון של 10Hz לממשק הגרפי
        vTaskDelay(100); 
    }
}