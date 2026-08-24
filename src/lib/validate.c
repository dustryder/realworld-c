#include "validate.h"

void is_required(OptionalValue value, ErrorValue *error, size_t *error_count, char* key) {
    if (!value.is_present) {
        error->property = key;
        error->message = "must be present";
        (*error_count)++;
    }
}

void not_null_or_empty(OptionalValue value, ErrorValue *error, size_t *error_count, char* key) {
    if (value.is_present && (value.value == NULL || strlen(value.value) == 0)) {
        error->property = key;
        error->message = "can't be blank";
        (*error_count)++;
    }
}