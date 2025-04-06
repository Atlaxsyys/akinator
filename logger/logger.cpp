#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#include "logger.h"

Logger_t* logger_constructor(const char* filename, log_level min_level)
{
    Logger_t* logger = (Logger_t*) calloc(1, sizeof(Logger_t));

    logger->file = fopen(filename, "a");

    logger->min_level = min_level;

    return logger;
}

void logger_destructor(Logger_t* logger)
{
    if (logger)
    {
        if (logger->file)
        {
            fclose(logger->file);
        }

        free(logger);
    }
}

const char* log_level_to_str(log_level level)
{
    switch (level)
    {
        case DEBUG:    return "DEBUG";
        case  INFO:    return "INFO";
        case ERROR:    return "ERROR";
        default:       return "UNKNOWN";
    }
}

void log_message(Logger_t* logger, log_level level, const char* file, int line, const char* format, ...)
{
    time_t current = time(NULL);

    char time_str[SIZE_TIME_STR] = {};

    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&current));

    fprintf(logger->file, "\n       ===== New Session Started =====\n");

    fprintf(logger->file, "[%s] [%s] [%s:%d] ", time_str, log_level_to_str(level), file, line);


    va_list args = {};
    va_start(args, format);
    vfprintf(logger->file, format, args);
    va_end(args);

    fprintf(logger->file, "\n");
    fflush(logger->file);
}