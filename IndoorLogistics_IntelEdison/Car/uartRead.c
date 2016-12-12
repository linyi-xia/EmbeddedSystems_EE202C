#include "stdio.h"
//! [Interesting]
#include "mraa.h"

int main(int argc, char** argv)
{
    mraa_uart_context uart;
    uart = mraa_uart_init(0);
	mraa_uart_set_baudrate(uart, 9600);
    if (uart == NULL) {
        fprintf(stderr, "UART failed to setup\n");
        printf("UART fialed");
	return EXIT_FAILURE;
    }

    char buffer[] = "Hello Mraa!";
//    mraa_uart_write(uart, buffer, sizeof(buffer));
	printf("buffer: ");
    char* current;
	while(1){
	printf("%c",buffer[1]);
    	mraa_uart_read(uart, buffer, 9);
	printf("buff:%c %c %c %c %c %c %c %c %c\n",buffer[0], buffer[1], 
buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], 
buffer[8] );
	sleep(1);

    }
//    mraa_uart_stop(uart);

    mraa_deinit();

    return EXIT_SUCCESS;
}
