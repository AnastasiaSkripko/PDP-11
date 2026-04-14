#include <stdio.h>
#include <stdarg.h>

#define ERROR    1
#define INFO     2
#define TRACE    3
#define DEBUG    4

int log_level = INFO; // пороговый уровень логирования

// функция логирования, в которой указан уровень логирования level и сообщение, которое нужно вывести, 
// если этот уровень <= установленному пороговому уровню логирования log_level
void logger(int level, const char *format, ...) {
    if (level <= log_level) {
        va_list ap;
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
    }
}

// устанавливает пороговый уровень логирования и возвращает предыдущий уровень логирования
int set_log_level(int level) {
    int old_log = log_level;    // сохраняем предыдущий уровень логирования
    log_level = level;          // устанавливаем новый уровень логирования 
    return old_log;
}