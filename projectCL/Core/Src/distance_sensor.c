#include "distance_sensor.h"

extern TIM_HandleTypeDef htim2;

// הגדרת הפינים - לשנות אם בחרת משהו אחר בממשק
#define TRIG_PORT GPIOA
#define TRIG_PIN GPIO_PIN_8
#define ECHO_PORT GPIOA
#define ECHO_PIN GPIO_PIN_9
#define TIMER_GAP 10
#define SPEED_OF_SOUND 0.0343

void init_distance_sensor(void) {
    HAL_TIM_Base_Start(&htim2);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

float read_distance(void) {
    uint32_t pMillis;
    uint32_t time_start = 0, time_end = 0;
    float distance = 0.0;

    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while (__HAL_TIM_GET_COUNTER(&htim2) < TIMER_GAP);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);

    pMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && pMillis + TIMER_GAP > HAL_GetTick());
    time_start = __HAL_TIM_GET_COUNTER(&htim2);

    pMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && pMillis + 50 > HAL_GetTick());
    time_end = __HAL_TIM_GET_COUNTER(&htim2);

    distance = (float)(time_end - time_start) * SPEED_OF_SOUND;
    return distance;
}
