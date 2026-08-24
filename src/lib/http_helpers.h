#include "main.h"
#include "type.h"

#ifndef LIB_HTTP_HELPERS_H
#define LIB_HTTP_HELPERS_H

void add_field(UpdateValue *values, size_t *count, char *key, OptionalValue value);
char *get_bearer_token(http_s* h);
char *parse_path_param(FIOBJ *params, char *key);
int parse_path_param_number(FIOBJ *params, char *key);
int parse_request_user(FIOBJ *params);
OptionalValue parse_optional_string(FIOBJ obj, char* key);
OptionalArray parse_optional_array(FIOBJ obj, char* key);
char *create_failure_body_from_error(ErrorValue error);
char *create_failure_body_from_errors(ErrorValue* errors, size_t error_count);
char *create_empty_response();

#endif