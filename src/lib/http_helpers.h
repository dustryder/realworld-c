#include "main.h"

#ifndef LIB_HTTP_HELPERS_H
#define LIB_HTTP_HELPERS_H

typedef struct {
    char* property;
    char* error;
} ErrorValue;

typedef struct {
    char* value;
    int is_present;
} OptionalValue;


char *get_bearer_token(http_s* h);
char *parse_path_param(FIOBJ *params, char *key);
int parse_request_user(FIOBJ *params);
char *create_failure_body_from_errors(ErrorValue* errors, size_t error_count);

#endif