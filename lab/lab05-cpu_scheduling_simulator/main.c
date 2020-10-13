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
#include <string.h>

uint64_t check_method();
uint64_t read_file();
uint64_t check_tq();
void print_processes();

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
			break;
		case 3:
			if (!check_method(argv[2]))
				return -1;
			time_quantum = 1;
			break;
		case 4:
			if (!check_method(argv[2]))
				return -1;
			if (!check_tq(argv[3]))
				return -1;
			break;
		default:
			fprintf(stderr, "failed to fill default arguments\n");
			return -1;
	}
	//parse filename
	if (!read_file(argv[1])) {
		return -1;
	}
	return 0;
}

uint64_t check_method(const char *meth) {
	if (strcmp("FCFS", meth) == 0) {
		method = 1;
	} else if (strcmp("RR", meth) == 0) {
		method = 2;
	} else if (strcmp("SJF", meth) == 0) {
		method = 3;
	} else {
		fprintf(stderr, "method must be FCFS, RR, or SJF\n");
		return 0;
	}
	return 1;
}

// should do a check to make sure its actually integer
uint64_t check_tq(const char *tq) {
	time_quantum = atoi(tq);
	if (!time_quantum) {
		fprintf(stderr, "time quantum must be a number\n");
	}
	return time_quantum;
}
uint64_t read_file(const char* filename) {
	FILE * read;
	size_t lines;

	read = fopen(filename, "r");
	if (read == NULL) {
		fprintf(stderr, "error reading file %s\n", filename);
		return 0;
	}
	fscanf(read, "%zu", &lines);

	for (size_t i = 0; i < lines; i++) {
		fscanf(read, "%"SCNu32, &processes[i]->pid);
		fscanf(read, "%"SCNu64, &processes[i]->arrival_time);
		fscanf(read, "%"SCNu64, &processes[i]->burst_time);
	}
	print_processes(&lines);

	fclose(read);
	return 1;
}

void print_processes(const size_t *count) {
	printf("\t\t%-8s%-8s%-8s\n", "pid", "arrival_time", "burst_time");
	for(size_t i = 0; i < *count; i++) {
		printf("\t\t%-8" PRIu32 "%-8" PRIu64 "%-8" PRIu64 "\n", 
			processes[i]->pid, 
			processes[i]->arrival_time,
			processes[i]->burst_time
		);
	}
}


