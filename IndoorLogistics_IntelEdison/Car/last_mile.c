
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

 Date 12/5/2015
 Name: Yue Du
 Description: system integrated

 **********/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <sys/stat.h>
#include <fcntl.h>
#include <mraa/pwm.h>
#include <mraa.h>
#include "testgetnearbeacon.h" //BLE Beacon 


#define MAXBUFSIZ 1024
#define CENTER 0.066f

void speed_control(mraa_pwm_context, mraa_pwm_context, float);
void readCharAry(mraa_uart_context, char* data);
void error(const char *msg);
void httpGET(int num, char* response);
void parsestring(char* theString, char* returnString);
void parseTurnByTurn(char* fullString, char* turnByTurn);
void parsechunks (char* getString, char*chunkOfFive, int status);
void parseBeaconID(char* chunkOfFive, char*BeaconID);
void parseTurn(char* chunkOfFive, char* turn);
int parsestatus(char* getString);

int main(){
 	char response[4096];
 	int status;
 	char * theString = (char*) malloc(128);
	do
	{
		memset(response,0,sizeof(response));
    	httpGET(1001, response);
    	printf("response=%s\n",response);      //Returned message from server
    	parsestring(response, theString);      //all the strings in the {""}
    	printf("theString=%s\n",theString);
    	int status = parsestatus(theString);
        //sleep(5); // ping every 5 seconds
    }
    while(status == 0);

    char* turnByTurn = (char*) malloc(64);
    char* chunkOf5 = (char*) malloc(5);
    char* beaconId = (char*) malloc(4);
    char* turn = (char*) malloc(1);

    parseTurnByTurn(theString, turnByTurn);
    printf("Turn_by_Turn directions: %s\n",turnByTurn);
    char* currentChar = turnByTurn;
    printf("**** Mission Arranged ****\n");
/**********************************************************************/
/**********************************************************************/
/* Begin. */
	mraa_uart_context uart;
	uart = mraa_uart_init(0);
	mraa_uart_set_baudrate(uart, 115200);
	char buffer[] = "hhh";
	char flush[]="xxxxxxxxxxxx";
	char str[] = "HELLO";
	if(uart == NULL) 
    {
        fprintf(stderr, "UART failed to setup\n");
        printf("UART failed");
		return 1;
    }
	printf("firstavail:%d\n",mraa_uart_data_available(uart,0));
	if(mraa_uart_data_available(uart,0))
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
    if (speed_pwm_in1 == NULL || speed_pwm_in2 == NULL || turn_pwm == NULL) 
    {
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

    int spd = 0;
    char direction = 'C';
    char pre_direction = 'C';
    char cross_clr = 'B';
    char pre_clr = 'B';
    float angle = CENTER;
	while(1)
    {
	    spd = 50;
        mraa_pwm_write(turn_pwm, angle);
        speed_control(speed_pwm_in1, speed_pwm_in2, spd);
        printf("speed: %d",spd);
        usleep(250000);
        pre_direction = direction;
        pre_clr = cross_clr;
    
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
	    if(buffer[1] == '\n')
        {
		    buffer[0] = 'h';
		    buffer[1] = 'h';
        }
        printf("Inbound Buffer: %c %c %c \n",buffer[0], buffer[1],buffer[2]);
        direction = buffer[0];
        cross_clr = buffer[1];

	    int sign = 0;
	    if(cross_clr == 'M')
	    {
            //HTTP GET and parse at the first M
            if(pre_clr == 'M')
            {
                if(turn[0] == 'F')
                {
                    angle = CENTER;
                    mraa_pwm_write(turn_pwm, angle);
                    speed_control(speed_pwm_in1, speed_pwm_in2, spd);
                    printf("speed: %d",spd);
                    sleep(1);
                }
                if(turn[0] == 'B')
                {
                    angle = CENTER;
                    mraa_pwm_write(turn_pwm, angle);
                    speed_control(speed_pwm_in1, speed_pwm_in2, -spd);
                    printf("speed: %d",spd);
                    sleep(1);
                }
                if(turn[0] == 'L')
                {
                    angle = CENTER - 0.01f;
                    mraa_pwm_write(turn_pwm, angle);
                    speed_control(speed_pwm_in1, speed_pwm_in2, 54);
                    printf("speed: %d",54);
                    usleep(500000);
                }
                if(turn[0] == 'R')
                {
                    angle = CENTER + 0.01f;
                    mraa_pwm_write(turn_pwm, angle);
                    speed_control(speed_pwm_in1, speed_pwm_in2, 54);
                    printf("speed: %d",54);
                    usleep(500000);
                }
                mraa_uart_write(uart, str, 1);
                while(!mraa_uart_data_available(uart, 10)){};
                mraa_uart_read(uart, buffer, 1);
                while(buffer[0] == '\n') mraa_uart_read(uart, buffer, 1);
                if(buffer[0] == '\0') direction = 'C';
                mraa_uart_read(uart, buffer + 1, 1);
                cross_clr = buffer[1];
            }
            else{
                sign = 1;
                speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
                sleep(1);
                char* nearestBeaconID = (char*)malloc(5);
                getStrongestBeacon(nearestBeaconID);
                printf("the nearestBeaconID is: %s\n", nearestBeaconID);

                parsechunks(currentChar, chunkOf5, status);
                parseBeaconID(chunkOf5,beaconId);
                parseTurn(chunkOf5, turn);
                printf("chunkof5:%s, beaconid:%s, turn:%s\n", chunkOf5, beaconId, turn);
                if(*(currentChar + 5) != '\0') currentChar = currentChar+5;
                if(strcmp(beaconId, nearestBeaconID) != 0)
                {
                    printf("Error: Missmatched BeaconID! Mission Abort.\n");
                    return 0;
                }
		        if(turn[0] == 'S')
                {
                    speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
                    sleep(2);
                    break;
                }
                if(turn[0] == 'F')
                {
                    angle = CENTER;
                    mraa_pwm_write(turn_pwm, angle);
                    usleep(15000);
                    speed_control(speed_pwm_in1, speed_pwm_in2, spd);
                    printf("speed: %d",spd);
                    sleep(3);
                    speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
                }
                if(turn[0] == 'B')
                {
                    angle = CENTER;
                    mraa_pwm_write(turn_pwm, angle);
                    usleep(15000);
                    speed_control(speed_pwm_in1, speed_pwm_in2, -spd);
                    printf("speed: %d",spd);
                    sleep(3);
                    speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);   
                }
                if(turn[0] == 'L')
                {
                    angle = CENTER - 0.0149f;
                    mraa_pwm_write(turn_pwm, angle);
                    usleep(15000);
                    speed_control(speed_pwm_in1, speed_pwm_in2, 54);
                    printf("speed: %d",spd);
                    usleep(5500000);
                    speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
                }
                if(turn[0] == 'R')
                {
                    angle = CENTER + 0.015f;
                    mraa_pwm_write(turn_pwm, angle);
                    usleep(15000);
                    speed_control(speed_pwm_in1, speed_pwm_in2, 54);
                    printf("speed: %d",spd);
                    usleep(5500000);
                    speed_control(speed_pwm_in1, speed_pwm_in2, 0.0f);
                }
                mraa_uart_write(uart, str, 1);
                while(!mraa_uart_data_available(uart, 10)){};
                mraa_uart_read(uart, buffer, 1);
                while(buffer[0] == '\n')
                {
                    mraa_uart_read(uart, buffer, 1);
                }
                if(buffer[0] == '\0') direction = 'C';
                mraa_uart_read(uart, buffer + 1, 1);
                cross_clr = buffer[1];
            }
	    }
	
        if(sign && (turn[0] == 'L' || turn[0] == 'R'))
        {
            if(turn[0] == 'L')
            {
                if (direction == 'C')   angle = CENTER - 0.013f; 
                if (direction == 'L')   angle = CENTER - 0.005f;
                if (direction == 'R')   angle = CENTER - 0.019f;
            }
            if(turn[0] == 'R')
            {
                if (direction == 'C')   angle = CENTER + 0.013f; 
                if (direction == 'L')   angle = CENTER + 0.005f;
                if (direction == 'R')   angle = CENTER + 0.019f;
            }
        }
	    else
        {
            if(direction == 'L')   angle = CENTER - 0.005f;
            if(direction == 'R')   angle = CENTER + 0.005f;
            if(direction == 'C')   angle = CENTER;
        }
    }
    sleep(1);
    printf("**** Mission accomplished ****)\n");
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
void error(const char *msg) { perror(msg); exit(0); }

void httpGET(int num, char* response)
{
    /* first what are we going to send and where are we going to send it? */
    int portno =        8080;
    char *host =        "169.232.190.131";
    char *message_fmt = "GET /database?Car_ID=%d HTTP/1.0\r\n\r\n";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char message[1024];

    //if (argc < 3) { puts("Parameters: <key> <UWVCMAFKPNB7SW0L>"); exit(0); }

    /* fill in the parameters */
    sprintf(message,message_fmt,num);
    printf("Request:\n%s\n",message);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    //memset(response,0,sizeof(response));
    total = 4096-1;
    received = 0;
    do {
        bytes = read(sockfd,response-received,total-received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    /* close the socket */
    close(sockfd);

    /* process response */
    //printf("Response:\n%s\n",response);

    return;
}

void parsestring(char* theString, char* returnString)   //parse{}out from the original string
{
    char* current = theString;
    while(current[0] != '{')  //not equal
    {
        current++;
    }
    if (current[0] == '{')
    {
        char* cur = current;
        int count=0;
        while(cur[0] != '}')
        {
            cur++;
            count++;
        }
        strncpy(returnString, current+1, count-1);
    }
}

int parsestatus(char* getString)
{
    if (getString == NULL)
    {
        printf("no input string");
        return 0;
    }
    else
    {
        char currentChar = getString[0];
        char* current = getString;
        int count_comma=0;
        while(count_comma<3)
        {
            if (current[0] == ',')
            {
                count_comma++;
                current++;
            }
            else
            {
                current++;
            }
        }
        if (count_comma ==3)
        {
            while(current[0]!=':')
            {
                current++;
            }
            char status= *(current+1);
            if (status == '1')
                return 1;
            else
                return 0;
        }
    }
}
void parseTurnByTurn(char* fullString, char* turnByTurn)
{
    if (fullString == NULL)
    {
        printf("no input string");
        return;
    }
    else
    {
        char* current = fullString;
        int count_comma=0;
        while(count_comma<4)
        {
            if (current[0] == ',')
            {
                count_comma++;
                current++;
            }
            else
            {
                current++;
            }
        }
        if (count_comma ==4)
        {
            while(current[0]!=':')
            {
                current++;
            }
            if (current[0]==':')
            {
                char* cur = current+2;
                int count=0;
                while(cur[0] != '\"')
                {
                    cur++;
                    count++;
                }
                strncpy(turnByTurn, current+2, count);
                strncpy(&turnByTurn[count], "\0", 1);
            }
            
        }
    }
}
void parsechunks (char* getString, char* chunkOfFive, int status)
{
    if (status ==0)
    {
        printf("no mission.");
        return;
    }

    strncpy(chunkOfFive, getString, 5);
    strncpy(&chunkOfFive[5], "\0", 1);
    return ; 
}

void parseBeaconID(char* chunkOfFive, char* beaconID)
{
    if (chunkOfFive == NULL)
    {
        printf("no input string");
        return ;
    }
    else
    {
        
        strncpy(beaconID, chunkOfFive, 4);
        strncpy (&beaconID[4], "\0",1);
        return ;
    }
}
void parseTurn(char* chunkOfFive, char* turn)
{
    if (chunkOfFive == NULL)
    {
        printf("no input string");
        return ;
    }
    else
    {
        strncpy(turn, chunkOfFive+4, 1);
        strncpy(&turn[1], "\0",1);
        return ;
    }
}
