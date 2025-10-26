#ifndef VERBOSE_H
#define VERBOSE_H

#include <stdarg.h>

void verbose_set_enabled(int enabled);

int verbose_is_enabled(void);

void verbose_printf(const char *format, ...);

void verbose_log(const char *tag, const char *format, ...);

#endif // VERBOSE_H