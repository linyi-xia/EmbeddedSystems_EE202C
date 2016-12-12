
/*
 * Intel(R) Edison-based Lego(R) car
 * Demo 1 - Basic Operation
 *
 * Description: This demonstrates the basic operations of the car.
 *              The program asks for the turn, which can be a left turn, right turn, or straight.
 *              Then, it asks for the speed.
 *              100 is the maximum speed forward.
 *              0 is "stop".
 *              -100 is the maximum speed backward.
 *
 * Author: In Hwan "Chris" Baek
 * E-mail: chris.inhwan.baek@gmail.com
 * UCLA Wireless Health Institute
 */

 /**********
 Date: 11/9/2015
 Name: Linyi Xia
 Description:   The turn and speed arrays is the turn by turn direction. 

 Date: 11/20/2015
 Name: Linyi Xia
 Description:   Added functionalities of line following


 **********/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <mraa/pwm.h>
#include <mraa.h>

#define MAXBUFSIZ 1024
#define CENTER 0.068f

void speed_control(mraa_pwm_context, mraa_pwm_context, float);

int main(){
	mraa_uart_context uart;
	uart = mraa_uart_init(0);
	mraa_uart_set_baudrate(uart, 115200);
	char buffer[] = "x";
	if (uart == NULL) {
        	fprintf(stderr, "UART failed to setup\n");
        	printf("UART failed");
		return 1;
    	}
        char speed_user_input[MAXBUFSIZ];
        char turn_user_input[MAXBUFSIZ];
        mraa_pwm_context speed_pwm_in1, speed_pwm_in2, turn_pwm;
        speed_pwm_in1 = mraa_pwm_init(3);
        speed_pwm_in2 = mraa_pwm_init(5);
        turn_pwm = mraa_pwm_init(6);
        if (speed_pwm_in1 == NULL || speed_pwm_in2 == NULL || turn_pwm == NULL) {
                fprintf(stderr, "Failed to initialized.\n");
                return 1;
        }

        mraa_pwm_period_us(speed_pwm_in1,870); //1150Hz
        mraa_pwm_enable(speed_pwm_in1, 1);
        mraa_pwm_period_us(speed_pwm_in2,870);
        mraa_pwm_enable(speed_pwm_in2, 1);

        mraa_pwm_period_ms(turn_pwm,20);
        mraa_pwm_enable(turn_pwm, 1);

        mraa_pwm_write(turn_pwm, CENTER);
        mraa_pwm_write(speed_pwm_in1, 1.0f);
        mraa_pwm_write(speed_pwm_in2, 1.0f);

        int n = 5;
        char turn[5]={'L','C','R','L','C'};
        int speed[5] = {60,65,70,75,80};
        int spd = 0;
        char direction = 'C';
        float angle = 0.0f;
        int i=0;
        //for (i=0; i<n; i++)
	while (1)
        {
        spd = 0;
	spd = 80;
        //spd = speed[i];
        //direction = turn[i];
	mraa_uart_read(uart, buffer, 1);
	printf("buff:%c %c \n",buffer[0]);
        if (direction == 'L')   angle = CENTER -0.015f;
        if (direction == 'R')   angle = CENTER + 0.015f;
        if (direction == 'C')   angle = CENTER;
                mraa_pwm_write(turn_pwm, angle);
                usleep(100000);
                speed_control(speed_pwm_in1, speed_pwm_in2, spd);
                sleep(0.5);
                speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
		direction = buffer[0];

        }
        return 0;
}

void speed_control(mraa_pwm_context in1, mraa_pwm_context in2, float speed) {
        speed = speed/100;
        if (speed >= 0) {
                mraa_pwm_write(in2, 1.0f);
                mraa_pwm_write(in1, 1.0f - speed);
        }
        else if (speed < 0) {
                mraa_pwm_write(in1, 1.0f);
                mraa_pwm_write(in2, 1.0f + speed);
        }
}
