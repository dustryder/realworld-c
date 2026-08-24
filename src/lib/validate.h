#include "type.h"
#include <stddef.h>

#ifndef LIB_VALIDATE_H
#define LIB_VALIDATE_H
void is_required(OptionalValue value, ErrorValue *error, size_t *error_count, char* key);
void not_null_or_empty(OptionalValue value, ErrorValue *error, size_t *error_count, char* key);
#endif