#include "../../data/users.h"
#include "../../lib/http_helpers.h"
#ifndef USER_SERVICES_H
#define USER_SERVICES_H
typedef struct {
    char* email;
    char* token;
    char* username;
    char* bio;
    char* image;
} UserServiceResultData;

typedef struct {
    ServiceStatus status;
    UserServiceResultData data;
    ErrorValue error;
} UserServiceResult;

typedef struct {
    ServiceStatus status;
    char* data;
    ErrorValue error;
} RegisterUserServiceResult;

RegisterUserServiceResult register_user(PGconn *conn, char* username, char* email, char* password);
UserServiceResult login(PGconn *conn, char* email, char* password);
UserServiceResult get_user_by_id(PGconn *conn, int id);
UserServiceResult update_user(PGconn *conn, int id, OptionalValue email, OptionalValue password, OptionalValue username, OptionalValue bio, OptionalValue image);

UserServiceResultData map_data_to_user(UserData *data_result);
void free_UserServiceResultData(UserServiceResultData *data);
#endif