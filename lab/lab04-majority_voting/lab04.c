#include <stdio.h>
#include <stdint.h>

int main() {
	uint16_t num_voters;
	uint16_t max_rounds;

	printf("Number of voters? ");
	scanf("%5hu", &num_voters);
	printf("Rounds of voting? ");
	scanf("%5hu", &max_rounds);
	
	return 0;
}
