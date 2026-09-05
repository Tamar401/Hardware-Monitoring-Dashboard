#include "led.h"

extern TIM_HandleTypeDef htim3; // מוגדר אוטומטית ב-main.c

void init_led(void) {
    // ה-PWM יאותחל אוטומטית, כאן אפשר להוסיף דברים נוספים אם צריך
}

void led_on(void) {
    turn_on_pwm(&htim3, 2000); // הפעלה מלאה לפי הערך בחוברת
}

void led_off(void) {
    turn_on_pwm(&htim3, 0);
}

void turn_on_pwm(TIM_HandleTypeDef* pwmTimer, uint16_t pulseWidth) {
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = pulseWidth;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(pwmTimer, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(pwmTimer, TIM_CHANNEL_1);
}
