#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define sleep(msec) usleep((msec) * 1000)

#define MOTOR_RIGHT    	OUTA
#define MOTOR_LEFT    	OUTB
#define MOTOR_C    		OUTC
#define MOTOR_D    		OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_2		IN2
#define SENSOR_3		IN3
#define SENSOR_4		IN4

#define MOTOR_BOTH     	(MOTOR_LEFT | MOTOR_RIGHT)

int max_hastighet;
POOL_T touchSensor;

int init();
void drive(float, int);
void stop(int);
void turn(char, int);
void rotate(char, int);

int main(void){  
    //Init failed, "dirty" exit
    if (!init()){
        printf("Shutting down due to an error...\n");
        sleep(3000);
        return 1;
    }

    touchSensor = sensor_search(LEGO_EV3_TOUCH);
	touch_set_mode_touch(touchSensor);

	//HANDLEDNING 2
    printf("Press sensor to begin routine.\n");
    sleep(100);
    while(!sensor_get_value(0, touchSensor, 0));

    //The routine
    drive(1, 3000);
    stop(500);
    drive(-1, 3000);
    stop(500);
    drive(1, 300);
    turn('R', 500);
    turn('L', 1000);
    turn('R', 500);
    stop(200);
    rotate('R', 1500);
    rotate('L', 1500);

    printf("Routine done. Press sensor to exit...\n");
    sleep(100);
    //SLUT HANDLEDNING 2

    //Shutdown robot by pressing the touch sensor
	while(!sensor_get_value(0, touchSensor, 0));
	brick_uninit();
	printf("Shutting down...\n");
    sleep(3000);
    return 0;
}

int init(){
    printf("Initializing...\n");

    if (!brick_init()){
        printf("ERROR: Unable to initialize brick.\n");
        sleep(100);
        return 0;
    }

    if (tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )){
        max_hastighet = tacho_get_max_speed(MOTOR_LEFT, 0);
        tacho_reset(MOTOR_BOTH);
        printf("Initialization successful!\n"
               "******** Welcome! ********\n");
        sleep(100);
        return 1;
    } 
    else{
        brick_uninit();
        printf("ERROR: No motors connected.\n"
        "Connect left motor to port A and right motor to port B.\n");
        sleep(100);
        return 0;
    }
}

void drive(float speed, int runtime_msec){
    printf("Driving\n");
    tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * speed * 0.5);
    tacho_run_forever(MOTOR_BOTH);
    sleep(runtime_msec);
}

void stop(int sleep_time_msec){
    printf("Stopping\n");
    tacho_stop(MOTOR_BOTH);
    sleep(sleep_time_msec);
}

void turn(char direction, int runtime_msec){
    switch(direction){
        case ('R'):
            printf("Turning right\n");
            tacho_stop(MOTOR_RIGHT);
            break;
        case ('L'):
            printf("Turning left\n");
            tacho_stop(MOTOR_LEFT);
            break;
        default:
            printf("Incorrect direction given in function turn(). Valid directions: 'R' or 'L'.\n");
            return;
    }
    sleep(runtime_msec);
    tacho_run_forever(MOTOR_BOTH);
}

void rotate(char direction, int runtime_msec){
    switch(direction){
        case ('R'):
            printf("Rotating right\n");
            // Set speed for individual motors
            tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * 0.5);
            tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet * -0.5);
            // Start both motors
            tacho_run_forever(MOTOR_BOTH);
            break;
        case ('L'):
            printf("Rotating left\n");
            // Set speed for individual motors
            tacho_set_speed_sp(MOTOR_RIGHT, max_hastighet * 0.5);
            tacho_set_speed_sp(MOTOR_LEFT, max_hastighet * -0.5);
            // Start both motors
            tacho_run_forever(MOTOR_BOTH);
            break;
        default:
            printf("Incorrect direction given in function rotate(). Valid directions: 'R' or 'L'.\n");
            return;
    }
    sleep(runtime_msec);
    tacho_stop(MOTOR_BOTH);
}