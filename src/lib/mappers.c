#include "mappers.h"

void set_error(ErrorValue *error, char* property, char* message) {
    error->property = property;
    error->message = message;
}