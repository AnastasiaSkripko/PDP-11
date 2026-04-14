#pragma once

#define ERROR    1
#define INFO     2
#define TRACE    3
#define DEBUG    4

void logger(int level, const char *format, ...);
int set_log_level(int level);