#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "util.h"

#define NUM_REPEAT 1000000
#define SLEEP_AFTER_WRITE 1 // milli seconds

int fds[2];
long time_records[NUM_REPEAT];

void *
writer(void *arguments) {
	for (int i = 0; i < NUM_REPEAT; i++) {
		long t = get_clock();
		write(fds[1], &t, sizeof(t));
		millisleep(SLEEP_AFTER_WRITE);
	}

	return NULL;
}

void *
reader(void *arguments) {
	for (int i = 0; i < NUM_REPEAT; i++) {
		long t_sent;
		read(fds[0], &t_sent, sizeof(t_sent));
		time_records[i] = get_clock() - t_sent;
	}

	return NULL;
}

// https://stackoverflow.com/a/7463383
int
main() {
	pipe(fds);

	pthread_t reader_thread;
	pthread_create(&reader_thread, NULL, reader, NULL);

	millisleep(10);

	pthread_t writer_thread;
	pthread_create(&writer_thread, NULL, writer, NULL);

	pthread_join(writer_thread, NULL);
	pthread_join(reader_thread, NULL);

	dump(time_records, NUM_REPEAT);
	printf("mean context switch time:  %ld\n", average(time_records, NUM_REPEAT));
	printf("median context switch time:  %ld\n", median(time_records, NUM_REPEAT));

	return 0;
}
