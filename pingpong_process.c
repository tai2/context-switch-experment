#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "util.h"

#define NUM_SWITCH 2000000

void
counter(int read_fd, int write_fd) {
	long t0 = get_clock();
	int count = 0;
	while (count < NUM_SWITCH) {
		read(read_fd, &count, sizeof(count));
		count += 1;
		write(write_fd, &count, sizeof(count));
	}

	if (count == NUM_SWITCH) {
		long time_spent_micro_sec = (get_clock() - t0) / 1000;
		printf("%ld microseconds\n", time_spent_micro_sec);
	}
}

int
main() {
	int pipe_ab[2], pipe_ba[2];
	pipe(pipe_ab);
	pipe(pipe_ba);

	pid_t pid_a = fork();
	if (pid_a == 0) {
		counter(pipe_ba[0], pipe_ab[1]);
		return 0;
	}

	pid_t pid_b = fork();
	if (pid_b == 0) {
		counter(pipe_ab[0], pipe_ba[1]);
		return 0;
	}

	// Wait to ensure child processess blocks on reads
	millisleep(10);

	int count = 0;
	write(pipe_ba[1], &count, sizeof(count));

	int status;
	waitpid(pid_a, &status, 0);
	waitpid(pid_b, &status, 0);

	return 0;
}
