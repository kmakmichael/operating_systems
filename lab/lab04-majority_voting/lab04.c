#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
	uint16_t num_voters = 0;
	pid_t pid = 0;
	int **pipes;

	// parameter input
	while(1) {
		printf("Number of voters? ");
		scanf("%5hu", &num_voters);
		if(num_voters > 2)
			break;
		printf("There must be between 2 and 65536 voters\n");
	}

	// pipe & child creation
	pipes = calloc(num_voters, sizeof(int *));
	for (int i = 0; i < num_voters; i++)
	{
		if (pid == 0) {
			pipes[i] = calloc(2, sizeof(int));
			pipe(pipes[i]);
			pid = fork();
			if (pid < 0) {
				printf("fork failed!");
				return 0;
			}
	}
		
	} if (pid > 0) {
		printf("hi, I'm a child process!\n");
	}

	//execvp() or free(pipes)
	return 0;
}
