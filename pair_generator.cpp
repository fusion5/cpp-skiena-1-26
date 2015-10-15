#include "pair_generator.h"

void generate_pairs () {
		// Generate pairs as input for the main program...
		
		srand(time(0));
		
		int x;
		int y;
		
		for (int i = 0; i < 1000; i++) {
				x = rand() % 1000 + 1;
				y = rand() % 1000 + 1;
				printf("%d %d \n", x, y);		
		}
}
