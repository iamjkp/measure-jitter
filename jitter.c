#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "jitter.h"

#define NSECS_PER_SEC 1000000000

#define timespec_normalize(t) {\
   if ((t) ->tv_nsec >= NSECS_PER_SEC) { \
       (t) ->tv_nsec -= NSECS_PER_SEC; \
       (t) ->tv_sec++; \
   } else if ((t) ->tv_nsec < 0) { \
       (t) ->tv_nsec += NSECS_PER_SEC; \
       (t) ->tv_sec -- ; \
   } \
}

#define timespec_sub(t1, t2) do { \
   (t1) ->tv_nsec -= (t2) ->tv_nsec; \
   (t1) ->tv_sec  -= (t2) ->tv_sec; \
   timespec_normalize(t1); \
} while (0)

void initJitter(jitter_t *jitter)
{
	memset(jitter->samples, 0, sizeof(long) * JITTER_SAMPLE_COUNT);
	clock_gettime(CLOCK_REALTIME, &jitter->start_time);
}

void deinitJitter(jitter_t *jitter)
{
	// Nothing now
}

void measureJitter(jitter_t *jitter)
{
	struct timespec current, current_origin;

	clock_gettime(CLOCK_REALTIME, &current);

	memcpy(&current_origin, &current, sizeof(struct timespec));
	
	timespec_sub(&current, &jitter->start_time);	

	memcpy(jitter->samples, jitter->samples+1, sizeof(long) * (JITTER_SAMPLE_COUNT-1));

	jitter->samples[JITTER_SAMPLE_COUNT-1] = (current.tv_nsec/1000) + (current.tv_nsec/100)%10; 

	memcpy(&jitter->start_time, &current_origin, sizeof(struct timespec));
}

void printJitter(jitter_t *jitter)
{
	int i = 0;
	for (i = JITTER_SAMPLE_COUNT-1 ; i >= 0 ; i--) {
		if (jitter->samples[i] > 0) {
			printf("%ld ", jitter->samples[i]);
		}
	}	
}

void writeJitter(jitter_t *jitter, char *path)
{
	FILE *f = NULL;
	int i = 0;

	f = fopen(path, "w");

	if (f != NULL) {
		for (i = JITTER_SAMPLE_COUNT-1 ; i >= 0 ; i--) {
			if (jitter->samples[i] > 0) {
				fprintf(f, "%ld\n", jitter->samples[i]);
			}
		}
		fclose(f);
	}
}
