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

UserServiceResultData map_data_to_user(UserData data_result);

typedef enum {
    CREATE_USER_SUCCESS,
    CREATE_USER_DUPLICATE,
    CREATE_USER_UNKNOWN
} CreateUserStatus;

typedef struct {
    CreateUserStatus status;
    char* result;
} RegisterUserStatus;

RegisterUserStatus register_user(char* username, char* email, char* password);


typedef enum {
    LOGIN_USER_SUCCESS,
    LOGIN_USER_BAD_PASSWORD,
    LOGIN_USER_UNKNOWN,
} LoginUserStatus;

typedef struct {
    LoginUserStatus status;
    UserServiceResultData result;
} LoginUserResult;

LoginUserResult login(char* email, char* password);


typedef enum {
    GET_USER_SUCCESS,
    GET_USER_UNKNOWN,
} GetUserStatus;

typedef struct {
    GetUserStatus status;
    UserServiceResultData result;
} GetUserByIdResult;

GetUserByIdResult get_user_by_id(int id);

typedef enum {
    UPDATE_USER_SUCCESS,
    UPDATE_USER_FAILURE,
} UpdateUserStatus;

typedef struct {
    UpdateUserStatus status;
    UserServiceResultData result;
} UpdateUserResult;

UpdateUserResult update_user(int id, OptionalValue email, OptionalValue password, OptionalValue username, OptionalValue bio, OptionalValue image);
#endif