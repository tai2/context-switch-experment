#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "util.h"

#define NUM_REPEAT 1000000
#define SLEEP_AFTER_WRITE 1 // milli seconds

int fds[2];
long time_records[NUM_REPEAT];

void
writer() {
	for (int i = 0; i < NUM_REPEAT; i++) {
		long t = get_clock();
		write(fds[1], &t, sizeof(t));
		millisleep(SLEEP_AFTER_WRITE);
	}
}

void
reader() {
	for (int i = 0; i < NUM_REPEAT; i++) {
		long t_sent;
		read(fds[0], &t_sent, sizeof(t_sent));
		time_records[i] = get_clock() - t_sent;
	}
}

int
main() {
	pipe(fds);

	pid_t pid = fork();
	if (pid == 0) {
		millisleep(10);
		writer();
		return 0;
	}

	reader();

	int status;
	waitpid(pid, &status, 0);

	dump(time_records, NUM_REPEAT);
	printf("mean context switch time:  %ld\n", average(time_records, NUM_REPEAT));
	printf("median context switch time:  %ld\n", median(time_records, NUM_REPEAT));

	return 0;
}
