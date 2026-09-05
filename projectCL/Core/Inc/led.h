#ifndef LED_H
#define LED_H

#include "main.h"

typedef struct {
    uint8_t state;        // 0=Off, 1=On, 2=Blink
    uint32_t blink_delay; // זמן המתנה בהבהוב
} LedCommand_t;

void init_led(void);
void led_on(void);
void led_off(void);
void turn_on_pwm(TIM_HandleTypeDef* pwmTimer, uint16_t pulseWidth);

#endif /* LED_H */
