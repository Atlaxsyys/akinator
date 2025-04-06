#pragma once

#include <stdio.h>

typedef enum {
    DEBUG = 1,
    INFO  = 2,
    ERROR = 3
} log_level;

typedef struct {
    FILE* file;
    log_level min_level;
} Logger_t;

const int SIZE_TIME_STR = 20;

Logger_t* logger_constructor(const char* filename, log_level min_level);
void logger_destructor(Logger_t* logger);
const char* log_level_to_str(log_level level);
void log_message(Logger_t* logger, log_level level, const char* file, int line, const char* format, ...);

#define LOG_DEBUG(logger, format, ...) log_message(logger, DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_INFO(logger, format, ... ) log_message(logger, INFO , __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(logger, format, ...) log_message(logger, ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
