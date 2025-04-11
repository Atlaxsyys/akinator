#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "logger.h"
#include "colour.h"

static Logger_t* logger = nullptr;

Logger_t* logger_constructor(const char* filename, log_level min_level)
{
    Logger_t* new_logger = (Logger_t*) calloc(1, sizeof(Logger_t));

    new_logger->file = fopen(filename, "a");

    if(! new_logger->file) fprintf(stderr, RED_TEXT("DAMIRIRR"));
    
    new_logger->min_level = min_level;

    fprintf(new_logger->file, "\n       ===== New Session Started =====\n");

    set_logger(new_logger);

    return new_logger;
}

void logger_destructor(Logger_t* logger_to_destroy)
{
    if (logger_to_destroy)
    {
        if (logger_to_destroy->file)
        {
            fclose(logger_to_destroy->file);
        }

        free(logger_to_destroy);
    }

    set_logger(NULL);
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
    assert(logger);

    time_t current = time(NULL);

    char time_str[SIZE_TIME_STR] = {};

    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&current));

    fprintf(logger->file, "[%s] [%s] [%s:%d] ", time_str, log_level_to_str(level), file, line);

    va_list args = {};
    va_start(args, format);
    vfprintf(logger->file, format, args);
    va_end(args);

    fprintf(logger->file, "\n");
    fflush(logger->file);
}

Logger_t* get_logger(void)
{
    return logger;
}

void set_logger(Logger_t* new_logger)
{
    logger = new_logger;
}