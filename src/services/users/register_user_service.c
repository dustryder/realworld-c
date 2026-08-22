#include "users_services.h"
#include "../../data/users.h"
#include "../../lib/token.h"
#include <string.h>

RegisterUserStatus register_user(PGconn *conn, char* email, char* username, char* password) {
    FIO_LOG_DEBUG("register_user: email: %s, user: %s, password: %s", email, username, password);

    RegisterUserStatus result;
    DataResult data_result = insert_user(conn, email, username, password);
    UserData *user_data = data_result.data;

    if (data_result.status == DATA_SUCCESS) {
        result.status = CREATE_USER_SUCCESS;
        char* jwt = sign_jwt(user_data->id);
        result.result = jwt;
    } else if (data_result.status == DATA_DUPLICATE) {
        result.status = CREATE_USER_DUPLICATE;
        result.error.property = data_result.error.property;
        result.error.error = "has already been taken";
    } else {
        result.status = CREATE_USER_UNKNOWN;
    }

    return result;
}