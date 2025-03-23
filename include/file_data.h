// #pragma once
#ifndef FILE_DATA_H
#define FILE_DATA_H
#include <stdio.h>

static const char* FILE_NAME = "../data/data_base.txt";

long size_text_file(FILE* file_read);
char* create_buffer(FILE* file_read);
size_t n_string(char* commands_buffer, long size_command);
const char** create_string_buffer(char* text_buffer, FILE* file_read);

#endif // FILE_DATA_H
