#include "main.h"

#ifndef LIB_HTTP_HELPERS_H
#define LIB_HTTP_HELPERS_H
char* get_bearer_token(http_s* h);

typedef struct {
    char* value;
    int is_present;
} OptionalValue;
#endif