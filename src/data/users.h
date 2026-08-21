#include <stddef.h>
#include "../lib/http_helpers.h"
#include "../lib/db.h"

#ifndef USER_DATA_H
#define USER_DATA_H

typedef struct {
    int id;
    char* email;
    char* username;
    char* password;
    char* bio;
    char* image;
} UserData;

void resolve_user_constraints(ErrorValue *value);

DataResult insert_user(char* email, char* username, char* password);
DataResult update_user_data(int id, UpdateValue *update_values, size_t value_count);
DataResult get_user_data_by_username(char* username);
DataResult get_user_by_email(char* email);
DataResult get_user_data_by_id(PGconn *conn, int id);
#endif