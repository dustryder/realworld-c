#include "string_helpers.h"
#include <string.h>
#include <stdlib.h>

char* datetimestamp_to_date(char* datetimestamp) {
    char *buffer = malloc(sizeof(char) * 12);

    strncpy(buffer, datetimestamp, 10);
    buffer[10] = 'T';
    buffer[11] = '\0';

    return buffer;
}

char* datetimestamp_to_datetimestamp(char* datetimestamp) {
    char *buffer = malloc(sizeof(char) * 20);

    strncpy(buffer, datetimestamp, 19);
    buffer[10] = 'T';
    buffer[19] = '\0';
    
    return buffer;
}

