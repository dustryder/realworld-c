
#ifndef LIB_STRING_HELPERS_H
#define LIB_STRING_HELPERS_H
char* datetimestamp_to_datetimestamp(char* datetimestamp);
char* datetimestamp_to_date(char* datetimestamp);
char* number_to_string(int num);
char* sluggify(char* title, int title_count);
#endif