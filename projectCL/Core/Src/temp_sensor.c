#include "temp_sensor.h"

extern ADC_HandleTypeDef hadc; // ב-L152 לרוב זה נקרא hadc

void init_temperature_sensor(void) {
    // מאותחל ב-main.c
}

float get_temperature(void) {
    uint32_t adc_value = 0;
    float temp = 0.0;

    HAL_ADC_Start(&hadc);
    if (HAL_ADC_PollForConversion(&hadc, 100) == HAL_OK) {
        adc_value = HAL_ADC_GetValue(&hadc);
        // הנוסחה מהחוברת:
        temp = (float)adc_value * (3300.0f / 4096.0f) / 10.0f;
    }
    HAL_ADC_Stop(&hadc);

    return temp;
}
