
#include <stdio.h>
#include <mraa.h>

int main()
{
	printf("0");
	mraa_uart_context uart;
	uart = mraa_uart_init(0);
	printf("b");
	mraa_uart_set_baudrate(uart, 115200);
	printf("c");

    if (uart == NULL) {
        fprintf(stderr, "UART failed to setup\n");
        printf("UART failed");
	return 1;
    }

	char buffer[] = "x";
	printf("buffer: ");

	while(1){
    	mraa_uart_read(uart, buffer, 1);
	printf("buff:%c %c \n",buffer[0]);
	sleep(0.5);

    }
    mraa_uart_stop(uart);

    mraa_deinit();

    return 0;
}

