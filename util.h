#include <stddef.h>

void show_timer_resolution(void);
long get_clock(void);
void millisleep(long milliseconds);
void dump(long records[], size_t total);
long average(long records[], size_t total);
long median(long records[], size_t total);
