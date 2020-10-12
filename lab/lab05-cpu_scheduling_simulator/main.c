/*
 *  Lab 05: CPU Scheduling Sim
 *  COMP 173: Operating Systems (Fall 2020)
 *  Michael Kmak
 *
 */

#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <stdlib.h>

typedef struct __processdata {
	pid_t pid;
	uint64_t arrival_time;
	uint64_t burst_time;
} processdata;

processdata processes [20][3];
uint64_t method; //FCFS: 1, RR: 2, SJF: 3
uint64_t time_quantum;

int main(int argc, char *argv[]) {
	// handle params
	if (argc < 2 || argc > 4) {
			fprintf(stderr, "usage: main.c input_file [FCFS|RR|SJF] [time quantum]\n");
			return -1;
	}
	// Fill out default arguments
	switch (argc) {
		case 2:
			method = 1;
		case 3:
			time_quantum = 1;
			break;
		case 4:
			printf("artv3 is %s", argv[3]);
			if (argv[3] == "FCFS") {
				method = 1;
			} else if (argv[3] == "RR") {
				method = 2;
			} else if (argv[3] == "SJF") {
				method = 3;
			} else {
				fprintf(stderr, "method must be FCFS, RR, or SJF");
			}
			printf("method is %" PRIu64 "\n", method);
			break;
		default:
			fprintf(stderr, "failed to fill default arguments");
			return -1;
	}

	return 0;
}

void read_file(const char* filename) {

}


