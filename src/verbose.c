#include <stdio.h>
#include "verbose.h"

static int verbose_enabled = 0;

void verbose_set_enabled(int enabled)
{
    verbose_enabled = enabled;
}

int verbose_is_enabled(void)
{
    return verbose_enabled;
}

void verbose_printf(const char *format, ...)
{
    if (!verbose_enabled)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void verbose_log(const char *tag, const char *format, ...)
{
    if (!verbose_enabled)
    {
        return;
    }

    printf("[%s] ", tag);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}