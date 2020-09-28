#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint64_t num_fibbs;
void *fibby();


int main(int argc, char *argv[]) {

	// Make the thread
	pthread_t tid;
	pthread_attr_t attr;

	// argument input checks
	if (argc != 2) {
		fprintf(stderr, "usage: fibby.c <integer value>\n");
		return -1;
	}
	if (atoi(argv[1]) < 1) {
		fprintf(stderr, "%d must be 1 or greater", atoi(argv[1]));
		return -1;		
	}

	// pthread prep and creation
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, fibby, argv[1]);
	pthread_join(tid, NULL);
	printf("hello\n");
	return 0;
}

void *fibby() {
	;
}
