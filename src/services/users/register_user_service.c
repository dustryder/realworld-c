#include "users_services.h"
#include "../../data/users.h"
#include "../../lib/token.h"
#include <string.h>

RegisterUserStatus register_user(char* email, char* username, char* password) {

    RegisterUserStatus result;
    UserDataResult data_result = insert_user(email, username, password);

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