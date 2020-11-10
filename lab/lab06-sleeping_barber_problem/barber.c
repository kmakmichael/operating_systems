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
bool customers_waiting();
bool customer_at(size_t seat);

size_t seats;
size_t customer_count;

pthread_mutex_t bell_lock;
pthread_cond_t bell;

pthread_mutex_t shop_open_lock;
bool shop_open;

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
    pthread_mutex_init(&q_lock, NULL);
    pthread_mutex_init(&shop_open_lock, NULL);
    shop_open = true;

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
        printf("===Creating thread for customer %zu\n", i);
        pthread_t cus_tid;
        pthread_create(&cus_tid, NULL, customer, 0);
        sleep(1);
    }

    pthread_mutex_lock(&bell_lock);
    shop_open = false;
    pthread_cond_signal(&bell);
    pthread_mutex_unlock(&bell_lock);
    
    pthread_join(barber_tid, NULL);
    sleep(1);
    printf("===END EXECUTION\n");
    return 0;
}

void *barber(void *param) {
    printf(" - hello from the barber thread\n");
    pthread_cond_t *wc;
    pthread_mutex_t *wl;
    size_t current_seat;

    pthread_mutex_lock(&shop_open_lock);
    while(shop_open) {
        // sleep if there's no customers
        pthread_mutex_unlock(&shop_open_lock);
        pthread_mutex_lock(&bell_lock);
        pthread_cond_wait(&bell, &bell_lock);
        printf(" - waking up\n");
        pthread_mutex_unlock(&bell_lock);

        while(customers_waiting()) {
            pthread_mutex_lock(&q_lock);
            if(waiting_room[current_seat]->occupied) {
                pthread_mutex_unlock(&q_lock);
                // read customer info
                pthread_mutex_lock(&q_lock);
                wc = waiting_room[current_seat]->cond;
                wl = waiting_room[current_seat]->lock;
                pthread_mutex_unlock(&q_lock);

                // signal the customer to exit a seat
                pthread_mutex_lock(wl);
                pthread_cond_signal(wc);
                pthread_mutex_unlock(wl);

                sleep(3);

                pthread_mutex_lock(wl);
                pthread_cond_signal(wc);
                pthread_mutex_unlock(wl);
            }
            pthread_mutex_unlock(&q_lock);
            ++current_seat;
            if(current_seat >= seats) {
                current_seat = 0;
            }
        }
        printf(" - sleeping once more\n");
        // reset current_seat, the next customer will occupy seat 0
        current_seat = 0;
    }
    pthread_mutex_unlock(&shop_open_lock);
    printf(" - goodbye from the barber thread\n");
    pthread_exit(0);
}

void *customer(void *param) {
    pthread_mutex_t waiting_lock;
    pthread_cond_t waiting_cond;
    pthread_cond_init(&waiting_cond, NULL);
    pthread_mutex_init(&waiting_lock, NULL);

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

    // signal to the barber that you're ready
    pthread_mutex_lock(&waiting_lock);
    pthread_mutex_lock(&bell_lock);
    printf("\tin Q at seat %i and waiting for barber\n", empty_seat);
    pthread_cond_signal(&bell);
    pthread_mutex_unlock(&bell_lock);
    pthread_cond_wait(&waiting_cond, &waiting_lock);
    pthread_mutex_unlock(&waiting_lock);

    // free up the waiting-room seat
    pthread_mutex_lock(&q_lock);
    printf("\tfreed up seat %i\n", empty_seat);
    waiting_room[empty_seat]->occupied = false;
    pthread_mutex_unlock(&q_lock);

    // wait for the haircut to finish
    pthread_mutex_lock(&waiting_lock);
    pthread_cond_wait(&waiting_cond, &waiting_lock);
    pthread_mutex_unlock(&waiting_lock);
    
    // exit the shop
    printf("\tleaving the store\n");
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

bool customers_waiting() {
    bool waiting = false;
    pthread_mutex_lock(&q_lock);
    for (size_t i = 0; i < seats; i++) {
        waiting |= waiting_room[i]->occupied;
    }
    pthread_mutex_unlock(&q_lock);
    return waiting;
}
