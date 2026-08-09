#include "users_service.h"
#include "../data/users.h"
#include "../lib/token.h"


RegisterUserStatus register_user(char* email, char* username, char* password) {

    RegisterUserStatus result;
    UserDataResult data_result = insertUser(email, username, password);

    if (data_result.status == DataStatusSuccess) {
        result.status = CREATE_USER_SUCCESS;
        char* jwt = sign_jwt(data_result.data.id);
        result.result = jwt;
    } else if (data_result.status == DataStatusDuplicate) {
        result.status = CREATE_USER_DUPLICATE;
    } else {
        result.status = CREATE_USER_UNKNOWN;
    }

    return result;
}