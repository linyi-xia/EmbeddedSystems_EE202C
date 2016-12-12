
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



 **********/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <mraa/pwm.h>
#include <mraa.h>
#include "testgetnearbeacon.h"

#define MAXBUFSIZ 1024
#define CENTER 0.066f

void speed_control(mraa_pwm_context, mraa_pwm_context, float);
void readCharAry(mraa_uart_context, char* data);

int main(){
 








/**********************************************************************/
/**********************************************************************/
/* Begin. */
	mraa_uart_context uart;
	uart = mraa_uart_init(0);
	mraa_uart_set_baudrate(uart, 115200);
	char buffer[] = "hhh";
	char flush[]="xxxxxxxxxxxx";
	char str[] = "HELLO";
	if (uart == NULL) {
        	fprintf(stderr, "UART failed to setup\n");
        	printf("UART failed");
		return 1;
    	}
	printf("firstavail:%d\n",mraa_uart_data_available(uart,0));
	while (mraa_uart_data_available(uart,0))
	{
		mraa_uart_read(uart, flush, sizeof(uart));
		printf("Flush: %c %c %c %c %c %c %c %c",flush[0], flush[1], flush[2],flush[3],flush[4],flush[5],flush[6],flush[7]);
		usleep(150);
	}
	printf("available: %d",mraa_uart_data_available(uart,0));
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
        int spd = 0;
        char direction = 'C';
	char cross_clr = 'B';
        float angle = 0.0f;
	while (1)
        {
	//sleep(2);
        //readCharAry(uart,flush);
	spd = 50;
	mraa_uart_read(uart, buffer, 1);
	if(buffer[0]=='\n')
	{
		printf("new line ");	
		mraa_uart_read(uart, buffer, 1);
		if(buffer[0]=='\n')
		{
			printf("new line ");
                	mraa_uart_read(uart, buffer, 1);
		}
	}
	mraa_uart_read(uart, buffer+1, 1);
	if(buffer[1]=='\n')
        {
		buffer[0] = 'h';
		buffer[1] = 'h';
        }
	int sign = 0;
	if(cross_clr == 'M')	
	{
		speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
		sleep(1);
		char* nearestBeaconID = (char*)malloc(5);
		getStrongestBeacon(nearestBeaconID);
		printf("the nearestBeaconID is: %s\n", nearestBeaconID);
		sign = 1;
		angle = CENTER + 0.015f;
        mraa_pwm_write(turn_pwm, angle);
        usleep(15000);
        speed_control(speed_pwm_in1, speed_pwm_in2, 54);
        printf("speed: %d",spd);
        usleep(5500000);
                speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
		//sleep(1);
		mraa_uart_write(uart, str, 1);
		//while(!mraa_uart_data_available(uart, 10)){};
        mraa_uart_read(uart, buffer, 1);
        while(buffer[0] == '\n')
		{
			mraa_uart_read(uart, buffer, 1);
		}
		if(buffer[0] == '\0') direction = 'C';
                mraa_uart_read(uart, buffer + 1, 1);
                cross_clr = buffer[1];
	}
	printf("buff:%c %c %c \n",buffer[0], buffer[1],buffer[2]);
	if(!sign){
        if (direction == 'L')   angle = CENTER - 0.005f;
        if (direction == 'R')   angle = CENTER + 0.005f;
        if (direction == 'C')   angle = CENTER;}
	else
	{
		if (direction == 'C')	angle = CENTER +0.013f; 
		if (direction == 'L')   angle = CENTER +0.005f;
        	if (direction == 'R')   angle = CENTER + 0.019f;
	}
	mraa_pwm_write(turn_pwm, angle);
        speed_control(speed_pwm_in1, speed_pwm_in2, spd);
	printf("speed: %d",spd);
	usleep(250000);

	direction = buffer[0];
	cross_clr = buffer[1];
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
void readCharAry(mraa_uart_context uart, char* data)
{
	int size = sizeof(data);
	printf("Flushing size of %d",size);
	int i=0;
	for(i=0; i<size; i++)
	{	
		mraa_uart_read(uart, data+i,1);
		printf("%c",data[i]);
		if(data[i]=='\0')	break;
	}
	if(mraa_uart_data_available)
	{
		mraa_uart_read(uart, data,1);
		mraa_uart_read(uart, data+1,1);
		mraa_uart_read(uart, data+2,1);
		mraa_uart_read(uart, data+3,1);

		printf("data avail: %c %c %c %c",data[0],data[1],data[2],data[3]);
	}
	printf("|");
	return;
}

