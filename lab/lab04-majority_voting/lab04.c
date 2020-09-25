#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
	uint16_t num_voters = 0;
	pid_t pid;

	// parameter input
	while(1) {
		printf("Number of voters? ");
		scanf("%5hu", &num_voters);
		if(num_voters > 2)
			break;
		printf("There must be between 2 and 65536 voters\n");
	}

	// pipe & child creation
	// calloc(num_voters, sizeof(int [2]);
	return 0;
}
