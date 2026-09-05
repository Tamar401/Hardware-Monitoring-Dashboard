#include "alarm.h"
#include "led.h"
#include "sync_manager.h"

void init_alarm(void) {
    init_led();
}

void alarm_temperature(float temp) {
    LedCommand_t command;

    if (temp > 35.0) { // חם - הבהוב מהר
        command.state = 2;
        command.blink_delay = 100;
    } else if (temp < 10.0) { // קר - הבהוב לאט
        command.state = 2;
        command.blink_delay = 1000;
    } else { // תקין - דולק קבוע
        command.state = 1;
        command.blink_delay = 0;
    }

    xQueueSend(queueLedCommands, &command, 0);
}
