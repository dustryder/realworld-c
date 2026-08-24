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

DataResult insert_user(PGconn *conn, char* email, char* username, char* password);
DataResult update_user_data(PGconn *conn, int id, UpdateValue *update_values, size_t value_count);
DataResult get_user_data_by_username(PGconn *conn, char* username);
DataResult get_user_by_email(PGconn *conn, char* email);
DataResult get_user_data_by_id(PGconn *conn, int id);

void free_UserData(UserData *data);
#endif