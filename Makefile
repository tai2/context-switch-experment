switch_thread: switch_thread.c util.o util.h
	gcc -O2 switch_thread.c util.c -o switch_thread

pingpong_thread: pingpong_thread.c util.o util.h
	gcc -O2 pingpong_thread.c util.c -o pingpong_thread

switch_process: switch_process.c util.o util.h
	gcc -O2 switch_process.c util.c -o switch_process

pingpong_process: pingpong_process.c util.o util.h
	gcc -O2 pingpong_process.c util.c -o pingpong_process

