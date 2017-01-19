#include <stdio.h>
#include "jitter.h"

int main(void) 
{
	jitter_t jitter;

	initJitter(&jitter);
	
	while(1) {
		measureJitter(&jitter);
		sleep(1);
	}

	return 0;
}
