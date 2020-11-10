#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>


void *barber();
void *customer();
int32_t check_waiting_room();

size_t seats;
size_t customer_count;

pthread_mutex_t bell_lock;
pthread_cond_t bell;

pthread_mutex_t q_lock;

typedef struct {
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
    bool occupied;
} Seat;

Seat **waiting_room;


int main(int argc, char *argv[]) {
    // handle params
	if (argc != 3) {
		fprintf(stderr, "usage: barber.c <seats> <customers>\n");
		return -1;
	}
	seats = atoi(argv[1]);
	if (seats < 1) {
		fprintf(stderr, "Number of seats (%" PRIu64 ") must be greater than 1\n", seats);	
		return -1;		
	}
	if (seats > 5) {
		fprintf(stderr, "Number of seats (%" PRIu64 ") must be less than 6\n", seats);
		return -1;
	}
    customer_count = atoi(argv[2]);
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
    pthread_mutex_init(&bell_lock, NULL);
    pthread_cond_init(&bell, NULL);
    pthread_create(&barber_tid, NULL, barber, 0);
    printf("Barber thread created\n");

    printf("Creating the waiting room\n");
    waiting_room = calloc(seats, sizeof *waiting_room);
    for(size_t i = 0; i < seats; i++) {
        waiting_room[i] = calloc(1, sizeof(Seat));
    }
    printf("Waiting room ready\n");

    printf("Entering Main Loop\n");
    for(size_t i = 0; i < customer_count; i++) {
        sleep(2);
        printf("===Creating thread for customer %zu\n", i);
        pthread_t cus_tid;
        pthread_create(&cus_tid, NULL, customer, 0);
    }

    pthread_mutex_lock(&bell_lock);
    pthread_cond_signal(&bell);
    pthread_mutex_unlock(&bell_lock);
    
    pthread_join(barber_tid, NULL);
    return 0;
}

void *barber(void *param) {
    printf(" - hello from the barber thread\n");
    pthread_cond_t *wc;
    pthread_mutex_t *wl;
    /*for(int i = 0; i < 5; i++) {
        pthread_mutex_lock(&bell_lock);
        pthread_cond_wait(&bell, &bell_lock);
        printf(" - acknowlint32_t
        pthread_mutex_lock(&q_lock);
        wc = waiting_room[0]->cond;
        wl = waiting_room[0]->lock;
        pthread_mutex_unlock(&q_lock);

        pthread_mutex_lock(wl);
        printf(" - signaling customer\n");
        pthread_cond_signal(wc);
        pthread_mutex_unlock(wl);

        sleep(1);

        printf(" - completing haircut\n");
        pthread_mutex_lock(wl);
        pthread_cond_signal(wc);
        pthread_mutex_unlock(wl);
        printf(" - customer kicked\n");

    }*/
    sleep(30);
    printf(" - goodbye from the barber thread\n");
    pthread_exit(0);
}

void *customer(void *param) {
    pthread_mutex_t waiting_lock;
    pthread_cond_t waiting_cond;
    pthread_cond_init(&waiting_cond, NULL);
    pthread_mutex_init(&waiting_lock, NULL);
    
    printf("\tentered the shop");

    // grab a seat in the waiting room
    int32_t empty_seat = check_waiting_room();
    if(empty_seat == -1) {
        printf("\tno available seats, leaving\n");
        pthread_mutex_destroy(&waiting_lock);
        pthread_cond_destroy(&waiting_cond);
        pthread_exit(NULL);
    }

    pthread_mutex_lock(&q_lock);
    waiting_room[empty_seat]->cond = &waiting_cond;
    waiting_room[empty_seat]->lock = &waiting_lock;
    waiting_room[empty_seat]->occupied = true;
    pthread_mutex_unlock(&q_lock);
    printf("\toccupied seat #%i\n", empty_seat);

    /*// signal to the barber that you're ready
    pthread_mutex_lock(&waiting_lock);
    pthread_mutex_lock(&bell_lock);
    printf("\tin Q and waiting for barber\n");
    pthread_cond_signal(&bell);
    pthread_mutex_unlock(&bell_lock);
    pthread_cond_wait(&waiting_cond, &waiting_lock);
    pthread_mutex_unlock(&waiting_lock);

    // you've been called, watch the completion cond
    printf("\tbarber called me OwO?\n");
    pthread_mutex_lock(&waiting_lock);
    printf("\tsnib snib\n");
    pthread_cond_wait(&waiting_cond, &waiting_lock);
    printf("\tmy hair's been cut!\n");
    pthread_mutex_unlock(&waiting_lock);
    */
    // finish and die
    pthread_mutex_destroy(&waiting_lock);
    pthread_cond_destroy(&waiting_cond);
    pthread_exit(NULL);
}


int32_t check_waiting_room() {
    pthread_mutex_lock(&q_lock);
    for (size_t i = 0; i < seats; i++) {
        if (!waiting_room[i]->occupied) {
            pthread_mutex_unlock(&q_lock);
            return (int32_t) i;
        }
    }
    pthread_mutex_unlock(&q_lock);
    return -1;
}
