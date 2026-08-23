
#ifndef LIB_TYPE_H
#define LIB_TYPE_H
typedef struct {
    char* property;
    char* message;
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
    SERVICE_SUCCESS,
    SERVICE_NOT_FOUND,
    SERVICE_UNAUTHORIZED,
    SERVICE_DUPLICATE,
    SERVICE_UNKNOWN
} ServiceStatus;
#endif