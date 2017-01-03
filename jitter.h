#ifndef _JITTER_H_
#define _JITTER_H_

#define JITTER_SAMPLE_COUNT 10

typedef struct _jitter_t {
	struct timespec start_time;
	struct timespec next_time;
	long samples[JITTER_SAMPLE_COUNT];
	
} jitter_t;

void initJitter(jitter_t *jitter);
void deinitJitter(jitter_t *jitter);
void measureJitter(jitter_t *jitter);
void printJitter(jitter_t *jitter);
void writeJitter(jitter_t *jitter, char *path);

#endif
