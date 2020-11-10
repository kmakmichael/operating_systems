#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // handle params
	if (argc != 3) {
		fprintf(stderr, "usage: barber.c <seats> <customers>\n");
		return -1;
	}
	size_t seats = atoi(argv[1]);
	if (seats < 1) {
		fprintf(stderr, "Number of seats (%" PRIu64 ") must be greater than 1\n", seats);	
		return -1;		
	}
	if (seats > 5) {
		fprintf(stderr, "Number of seats (%" PRIu64 ") must be less than 6\n", seats);
		return -1;
	}
    size_t customer_count = atoi(argv[2]);
    if (customer_count < 1) {
		fprintf(stderr, "Number of customers (%" PRIu64 ") must be greater than 1\n", seats);	
		return -1;		
	}
	if (customer_count > 5) {
		fprintf(stderr, "Number of customers (%" PRIu64 ") must be less than 256\n", seats);
		return -1;
	}
    printf("Creating barber thread...\n");

    printf("Barber thread created\n");


    printf("Entering Main Loop\n");
    for(size_t i = 0; i < customer_count; i++) {
        sleep(1);
        printf("Creating thread for customer %zu\n", i);
    }

    return 0;
}
