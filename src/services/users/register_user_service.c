#include "users_services.h"
#include "../../data/users.h"
#include "../../lib/token.h"
#include "../../lib/mappers.h"
#include <string.h>

RegisterUserServiceResult register_user(PGconn *conn, char* email, char* username, char* password) {
    FIO_LOG_DEBUG("register_user: email: %s, user: %s, password: %s", email, username, password);

    RegisterUserServiceResult result;
    DataResult data_result = insert_user(conn, email, username, password);
    UserData *user_data = data_result.data;

    if (data_result.status == DATA_SUCCESS) {
        result.status = SERVICE_SUCCESS;
        char* jwt = sign_jwt(user_data->id);
        result.data = jwt;
    } else if (data_result.status == DATA_DUPLICATE) {
        result.status = SERVICE_DUPLICATE;
        set_error(result.error, data_result.error.property, "has already been taken");
    }

    return result;
}