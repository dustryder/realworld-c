#include "users_services.h"
#include "../../data/users.h"
#include "../../lib/token.h"
#include <string.h>

LoginUserResult login(PGconn *conn, char* email, char* password) {

    LoginUserResult result;
    DataResult data_result = get_user_by_email(conn, email);
    UserData *user_data = data_result.data;

    if (data_result.status == DATA_SUCCESS) {

        if (strcmp(password, user_data->password) == 0) {
            result.status = SERVICE_SUCCESS;
            result.result = map_data_to_user(data_result.data);
        } else {
            result.status = SERVICE_UNAUTHORIZED;
            result.error.property = "credentials";
            result.error.error = "invalid";
        }

    } else if (data_result.status == DATA_NOT_FOUND) {
        result.status = SERVICE_NOT_FOUND;
    }

    return result;
}