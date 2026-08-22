
#ifndef LIB_TYPE_H
#define LIB_TYPE_H
typedef struct {
    char* property;
    char* error;
} ErrorValue;

typedef struct {
    char* value;
    int is_present;
} OptionalValue;

typedef struct {
    char** value;
    int value_count;
    int is_present;
} OptionalArray;

typedef struct {
    char* key;
    char* value;
} UpdateValue;

typedef enum {
    SUCCESS,
    NOT_FOUND,
    UNAUTHORIZED,
    UNKNOWN
} ServiceStatus;
#endif