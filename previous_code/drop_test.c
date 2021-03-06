#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define MOTOR_DROP      OUTC
#define SENSOR_TOUCH    IN2

int drop_speed;
POOL_T touchSensor;

int init();
void drop();

int main(void){  
    //Init failed, "dirty" exit
    if (!init()){
        printf("Shutting down due to an error...\n");
        sleep_ms(3000);
        return 1;
    }

    touchSensor = sensor_search(LEGO_EV3_TOUCH);
	touch_set_mode_touch(touchSensor);

    printf("Press sensor to begin routine.\n");
    sleep_ms(100);
    while(!sensor_get_value(0, touchSensor, 0));

    //The routine
    drop();

    printf("Routine done. Press sensor to exit...\n");
    sleep_ms(100);

    //Shutdown robot by pressing the touch sensor
	while(!sensor_get_value(0, touchSensor, 0));
	brick_uninit();
	printf("Shutting down...\n");
    sleep_ms(1500);
    return 0;
}

int init(){
    printf("Initializing...\n");

    if (!brick_init()){
        printf("ERROR: Unable to initialize brick.\n");
        sleep_ms(100);
        return 0;
    }

    if (tacho_is_plugged(MOTOR_DROP, TACHO_TYPE__NONE_)){
        drop_speed = tacho_get_max_speed(MOTOR_DROP, 0) * 0.1;
        tacho_reset(MOTOR_DROP);
        printf("Initialization successful!\n"
               "******** Welcome! ********\n");
        sleep_ms(100);
        return 1;
    } 
    else{
        brick_uninit();
        printf("ERROR: No motor connected.\n"
        "Connect the drop motor to port C.\n");
        sleep_ms(100);
        return 0;
    }
}

void drop(){
    printf("Dropping book\n");
    tacho_set_speed_sp(MOTOR_DROP, drop_speed);
    tacho_run_forever(MOTOR_DROP);
    sleep_ms(1000);
    tacho_stop(MOTOR_DROP);

    printf("Resetting position\n");
    tacho_set_speed_sp(MOTOR_DROP, drop_speed * -1);
    tacho_run_forever(MOTOR_DROP);
    sleep_ms(1000);
    tacho_stop(MOTOR_DROP);
}