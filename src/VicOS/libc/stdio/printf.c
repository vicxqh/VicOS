#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "mini-printf.h"

static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

static const int BUFFER_LENTH = 256;

int printf(const char* restrict format, ...){
    int ret;
    va_list va;
    va_start(va, format);
    char buffer[BUFFER_LENTH];
    memset(buffer, 0, BUFFER_LENTH);
    ret = mini_vsnprintf(buffer, BUFFER_LENTH, format, va);
    va_end(va);
    buffer[BUFFER_LENTH - 1] = '\0';
    print(buffer, strlen(buffer));
    return ret;
}


