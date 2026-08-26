#include "string_helpers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

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

char* number_to_string(int num) {
    int len = snprintf(NULL, 0, "%d", num);
    char *buffer = malloc((size_t)len + 1);
    sprintf(buffer, "%d", num);
    return buffer;
}

char* sluggify(char* title, int title_count) {
    int suffix_length = snprintf(NULL, 0, "-%d", title_count);
    char* result_string = malloc((strlen(title) * sizeof(char)) + suffix_length + 1);
    int result_string_pointer = 0;

    size_t title_length = strlen(title);

    for (size_t i = 0; i < title_length; i++) {
        char current_character = title[i];

        if (isalnum(current_character)) {
            result_string[result_string_pointer] = tolower(current_character);
            result_string_pointer += 1;
        } else if (current_character == ' ') {
            result_string[result_string_pointer] = '-';
            result_string_pointer += 1;
        }
    }

    result_string[result_string_pointer] = '\0';

    sprintf(result_string + strlen(result_string), "-%d", title_count);

    return result_string;
}
