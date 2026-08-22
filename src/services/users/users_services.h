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

UserServiceResultData map_data_to_user(UserData *data_result);

typedef struct {
    ServiceStatus status;
    char* result;
    ErrorValue error;
} RegisterUserStatus;

RegisterUserStatus register_user(PGconn *conn, char* username, char* email, char* password);

typedef struct {
    ServiceStatus status;
    UserServiceResultData result;
    ErrorValue error;
} LoginUserResult;

LoginUserResult login(PGconn *conn, char* email, char* password);

typedef struct {
    ServiceStatus status;
    UserServiceResultData result;
} GetUserByIdResult;

GetUserByIdResult get_user_by_id(PGconn *conn, int id);

typedef struct {
    ServiceStatus status;
    UserServiceResultData result;
} UpdateUserResult;

UpdateUserResult update_user(PGconn *conn, int id, OptionalValue email, OptionalValue password, OptionalValue username, OptionalValue bio, OptionalValue image);
#endif