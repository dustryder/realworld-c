#include <stddef.h>
#include "../../lib/http_helpers.h"

#ifndef USER_DATA_H
#define USER_DATA_H
typedef enum {
    DATA_SUCCESS,
    DATA_DUPLICATE,
    DATA_CONSTRAINT,
    DATA_NOT_FOUND,
    DATA_UNKNOWN,
} DataStatus;

typedef struct {
    int id;
    char* email;
    char* username;
    char* password;
    char* bio;
    char* image;
} UserData;

typedef struct {
    DataStatus status;
    UserData data;
    ErrorValue error;
} UserDataResult;

typedef struct {
    char* key;
    char* value;
} UpdateValue;

UserDataResult insert_user(char* email, char* username, char* password);
UserDataResult update_user_data(int id, UpdateValue *update_values, size_t value_count);
UserDataResult get_user_by_email(char* email);
UserDataResult get_user_data_by_id(int id);
#endif