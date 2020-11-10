#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>


void *barber();

pthread_mutex_t barber_mutex;
pthread_cond_t bell;

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

    // program data
    pthread_t barber_tid;



    printf("Creating barber thread...\n");
    pthread_mutex_init(&barber_mutex, NULL);
    pthread_cond_init(&bell, NULL);
    pthread_create(&barber_tid, NULL, barber, 0);
    printf("Barber thread created\n");


    printf("Entering Main Loop\n");
    for(size_t i = 0; i < customer_count; i++) {
        sleep(1);
        printf("Creating thread for customer %zu\n", i);
    }

    pthread_mutex_lock(&barber_mutex);
    pthread_cond_signal(&bell);
    pthread_mutex_unlock(&barber_mutex);
    
    pthread_join(barber_tid, NULL);
    return 0;
}

void *barber(void *param) {
    printf("hello from the barber thread\n");
    pthread_mutex_lock(&barber_mutex);
    pthread_cond_wait(&bell, &barber_mutex);
    printf("goodbye from the barber thread\n");
    pthread_exit(0);
}
