#include <stdio.h>
#include <time.h>
#include "util.h"

void
show_timer_resolution(void) {
	struct timespec tp;
	clock_getres(CLOCK_MONOTONIC_RAW, &tp);
	printf("resolution: (%ld,%ld)\n", tp.tv_sec, tp.tv_nsec);
}

long
get_clock(void) {
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
	return tp.tv_sec * 1000 * 1000 * 1000 + tp.tv_nsec;
}

void
millisleep(long milliseconds) {
	struct timespec tp;
	tp.tv_sec = milliseconds / 1000;
	tp.tv_nsec = (milliseconds % 1000) * 1000 * 1000;
	nanosleep(&tp, NULL);
}

void
dump(long records[], size_t total) {
	for (int i = 0; i < total; i++) {
		printf("%04d: %ld\n", i + 1, records[i]);
	}
}

long
average(long records[], size_t total) {
	long sum = 0;
	for (int i = 0; i < total; i++) {
		sum += records[i];
	}

	return sum / total;
}

long
median(long records[], size_t total) {
	return records[total / 2];
}
