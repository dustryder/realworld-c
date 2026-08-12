#include "users_service.h"
#include "../data/users.h"
#include "../lib/token.h"
#include <string.h>


RegisterUserStatus register_user(char* email, char* username, char* password) {

    RegisterUserStatus result;
    UserDataResult data_result = insertUser(email, username, password);

    if (data_result.status == DATA_SUCCESS) {
        result.status = CREATE_USER_SUCCESS;
        char* jwt = sign_jwt(data_result.data.id);
        result.result = jwt;
    } else if (data_result.status == DATA_DUPLICATE) {
        result.status = CREATE_USER_DUPLICATE;
    } else {
        result.status = CREATE_USER_UNKNOWN;
    }

    return result;
}

LoginUserData map_data_to_result(UserData data_result) {
    LoginUserData data;
    char* jwt = sign_jwt(data_result.id);

    data.username = data_result.username;
    data.email = data_result.email;
    data.token = jwt;
    data.bio = NULL;
    data.image = NULL;

    return data;
}

GetUserData map_data_to_get_result(UserData data_result) {
    GetUserData data;
    char* jwt = sign_jwt(data_result.id);

    data.username = data_result.username;
    data.email = data_result.email;
    data.token = jwt;
    data.bio = NULL;
    data.image = NULL;

    return data;
}

LoginUserResult login(char* email, char* password) {

    LoginUserResult result;
    UserDataResult data_result = get_user_by_email(email);

    if (data_result.status == DATA_SUCCESS) {

        if (strcmp(password, data_result.data.password) == 0) {
            result.status = LOGIN_USER_SUCCESS;
            result.result = map_data_to_result(data_result.data);
        } else {
            result.status = LOGIN_USER_BAD_PASSWORD;
        }

    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = LOGIN_USER_UNKNOWN;
    }

    return result;
}

GetUserByIdResult get_user_by_id(int id) {
    GetUserByIdResult result;
    UserDataResult data_result = get_user_data_by_id(id);

    if (data_result.status == DATA_SUCCESS) {
        result.status = GET_USER_SUCCESS;
        result.result = map_data_to_get_result(data_result.data);
    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = GET_USER_UNKNOWN;
    }

    return result;
}

