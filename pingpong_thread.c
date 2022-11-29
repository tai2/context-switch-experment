#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "util.h"

#define NUM_SWITCH 2000000

struct thread_args {
	int read_fd;
	int write_fd;
};

void *
counter(void *arguments) {
	struct thread_args* args = (struct thread_args *)arguments;

	long t0 = get_clock();
	int count = 0;
	while (count < NUM_SWITCH) {
		read(args->read_fd, &count, sizeof(count));
		count += 1;
		write(args->write_fd, &count, sizeof(count));
	}

	if (count == NUM_SWITCH) {
		long time_spent_micro_sec = (get_clock() - t0) / 1000;
		printf("%ld microseconds\n", time_spent_micro_sec);
	}

	return NULL;
}

int
main() {
	int pipe_ab[2], pipe_ba[2];
	pipe(pipe_ab);
	pipe(pipe_ba);

	struct thread_args args_a = {
		pipe_ba[0], pipe_ab[1]
	};
	pthread_t thread_a;
	pthread_create(&thread_a, NULL, counter, &args_a);

	struct thread_args args_b = {
		pipe_ab[0], pipe_ba[1]
	};
	pthread_t thread_b;
	pthread_create(&thread_b, NULL, counter, &args_b);

	// Wait to ensure threads blocks on reads
	millisleep(10);

	int count = 0;
	write(args_b.write_fd, &count, sizeof(count));

	pthread_join(thread_a, NULL);
	pthread_join(thread_b, NULL);

	return 0;
}
